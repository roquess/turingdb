#include "JsonParser.h"
#include "BioAssert.h"
#include "BioLog.h"
#include "DB.h"
#include "Edge.h"
#include "EdgeType.h"
#include "MsgCommon.h"
#include "MsgImport.h"
#include "Network.h"
#include "Node.h"
#include "NodeType.h"
#include "PropertyType.h"
#include "Writeback.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <regex>

const static std::unordered_map<std::string, db::ValueType> neo4jTypes = {
    {"String", db::ValueType{db::ValueType::ValueKind::VK_STRING}},
    {"StringArray", db::ValueType{db::ValueType::ValueKind::VK_INVALID}},
    {"Double", db::ValueType{db::ValueType::ValueKind::VK_DECIMAL}},
    {"Boolean", db::ValueType{db::ValueType::ValueKind::VK_BOOL}},
    {"LocalDate", db::ValueType{db::ValueType::ValueKind::VK_INVALID}},
    {"Integer", db::ValueType{db::ValueType::ValueKind::VK_INT}},
    {"Long", db::ValueType{db::ValueType::ValueKind::VK_DECIMAL}},
    {"Date", db::ValueType{db::ValueType::ValueKind::VK_INVALID}}};

JsonParser::JsonParser()
    : _db(db::DB::create()) {
}

JsonParser::JsonParser(db::DB* db)
    : _db(db) {
}

bool JsonParser::parse(const std::string& data, Format format) {
    try {
        switch (format) {

        case Format::Neo4j4_NodeProperties:
            return parseNeo4j4NodeProperties(data);

        case Format::Neo4j4_Nodes:
            return parseNeo4j4Nodes(data);

        case Format::Neo4j4_EdgeProperties:
            return parseNeo4j4EdgeProperties(data);

        case Format::Neo4j4_Edges:
            return parseNeo4j4Edges(data);
        }

    } catch (const std::exception& e) {
        Log::BioLog::echo(e.what());
        // Log::BioLog::log(msg::ERROR_FAILED_TO_PARSE_JSON() << path.string());
        return false;
    }

    return false;
}

db::DB* JsonParser::getDB() {
    return _db;
}

bool JsonParser::parseNeo4j4NodeProperties(const std::string& data) {
    db::Writeback wb(_db);
    db::StringRef netName = _db->getString("Neo4jNetwork");
    db::Network* net = wb.createNetwork(netName);
    if (!net) {
        net = _db->getNetwork(netName);
    }
    Log::BioLog::log(msg::INFO_READING_NODE_PROPERTIES_FROM_NEO4J_DUMP());

    const auto json = nlohmann::json::parse(data);
    const auto& results = json["results"].front()["data"];
    db::StringRef propName;
    db::StringRef ntName;
    db::NodeType* nt{nullptr};

    for (const auto& record : results) {
        const auto& row = record["row"];
        ntName = _db->getString(row.at(0));

        nt = _db->getNodeType(ntName);
        if (!nt) {
            nt = wb.createNodeType(ntName);
        }

        propName = _db->getString(row.at(2));
        bioassert(neo4jTypes.find(row.at(3).at(0)) != neo4jTypes.end());
        wb.addPropertyType(nt, propName,
                           neo4jTypes.find(row.at(3).at(0))->second);
    }

    return true;
}

bool JsonParser::parseNeo4j4Nodes(const std::string& data) {
    db::Writeback wb(_db);
    db::StringRef netName = _db->getString("Neo4jNetwork");
    db::Network* net = wb.createNetwork(netName);
    if (!net) {
        net = _db->getNetwork(netName);
    }

    Log::BioLog::log(msg::INFO_READING_NODES_FROM_NEO4J_DUMP());
    const auto json = nlohmann::json::parse(data);
    const auto& results = json["results"].front()["data"];
    db::StringRef ntName;
    db::StringRef nodeName;
    db::NodeType* nt{nullptr};
    db::StringRef propName;
    db::Property prop;
    db::PropertyType* propType{nullptr};
    std::string label;

    // for each row
    for (const auto& record : results) {
        const auto& row = record["row"];
        label = "";

        // Getting NodeType
        for (const std::string ntAliasStr : row.at(0)) {
            label += ":`" + ntAliasStr + "`";
        }
        ntName = _db->getString(label);

        nt = _db->getNodeType(ntName);
        if (!nt) {
            // If the NodeType does not exist, it means it was not created while
            // gathering node properties
            nt = wb.createNodeType(ntName);
        }
        bioassert(nt);

        // Creating node
        size_t nodeId = row.at(1).get<uint64_t>();
        _nodeIdMap.emplace(nodeId, net->getNodeCount());
        nodeName = _db->getString(std::to_string(nodeId));
        db::Node* n = wb.createNode(net, nt, nodeName);

        // For each <PropertyTypeName, value> pair
        for (const auto& [key, val] : row.at(2).items()) {
            propName = _db->getString(key);
            propType = nt->getPropertyType(propName);

            if (!propType)
                continue;

            switch (propType->getValueType().getKind()) {

            case db::ValueType::VK_STRING: {
                bool res =
                    wb.setProperty(n, {propType, db::Value::createString(val)});
                bioassert(res);
                break;
            }

            case db::ValueType::VK_INT: {
                bool res =
                    wb.setProperty(n, {propType, db::Value::createInt(val)});
                bioassert(res);
                break;
            }

            case db::ValueType::VK_DECIMAL: {
                bool res =
                    wb.setProperty(n, {propType, db::Value::createDouble(val)});
                bioassert(res);
                break;
            }

            case db::ValueType::VK_BOOL: {
                bool res =
                    wb.setProperty(n, {propType, db::Value::createBool(val)});
                bioassert(res);
                break;
            }

            case db::ValueType::VK_INVALID: {
                // Something went wrong
                Log::BioLog::echo("Invalid prop: " + propName);
                break;
            }

            case db::ValueType::VK_UNSIGNED: {
                // Something went wrong
                Log::BioLog::echo("Invalid prop: " + propName);
                break;
            }

            case db::ValueType::VK_STRING_REF: {
                // Something went wrong
                Log::BioLog::echo("Invalid prop: " + propName);
                break;
            }
            }
        }
    }

    return true;
}

bool JsonParser::parseNeo4j4EdgeProperties(const std::string& data) {
    db::Writeback wb(_db);
    db::StringRef netName = _db->getString("Neo4jNetwork");
    db::Network* net = wb.createNetwork(netName);
    if (!net) {
        net = _db->getNetwork(netName);
    }
    Log::BioLog::log(msg::INFO_READING_EDGE_PROPERTIES_FROM_NEO4J_DUMP());

    const auto json = nlohmann::json::parse(data);
    const auto& results = json["results"].front()["data"];
    db::StringRef propName;
    db::StringRef etName;
    db::EdgeType* et{nullptr};
    db::NodeType* sourceNt{nullptr};
    db::NodeType* targetNt{nullptr};
    std::string sourceLabel;
    std::string targetLabel;

    // Each row is 1 property + 1 source type + 1 target type. Thus at each row,
    // we might have to create a new edge type, add a source/target to an
    // existing one, or add a property
    for (const auto& record : results) {
        const auto& row = record["row"];
        etName = _db->getString(row.at(0));
        sourceLabel = "";
        targetLabel = "";

        // Getting source NodeType
        for (const std::string ntAliasStr : row.at(1)) {
            sourceLabel += ":`" + ntAliasStr + "`";
        }

        sourceNt = _db->getNodeType(_db->getString(sourceLabel));
        if (!sourceNt) {
            // Something went wrong
            return false;
        }

        // Getting target NodeType
        for (const std::string ntAliasStr : row.at(2)) {
            targetLabel += ":`" + ntAliasStr + "`";
        }

        targetNt = _db->getNodeType(_db->getString(targetLabel));
        if (!targetNt) {
            // Something went wrong
            return false;
        }

        et = _db->getEdgeType(etName);
        if (et) {
            wb.addSourceNodeType(et, sourceNt);
            wb.addTargetNodeType(et, targetNt);

            wb.addTargetNodeType(et, sourceNt);
            wb.addSourceNodeType(et, targetNt);
        } else {
            et = wb.createEdgeType(etName, std::vector{sourceNt, targetNt},
                                   std::vector{targetNt, sourceNt});
        }

        // If the edge type has no property, we skip this part
        if (row.at(3).is_null()) {
            continue;
        }
        propName = _db->getString(row.at(3));
        bioassert(neo4jTypes.find(row.at(4).at(0)) != neo4jTypes.end());
        wb.addPropertyType(et, propName,
                           neo4jTypes.find(row.at(4).at(0))->second);
    }

    return true;
}

bool JsonParser::parseNeo4j4Edges(const std::string& data) {
    db::Writeback wb(_db);
    db::StringRef netName = _db->getString("Neo4jNetwork");
    db::Network* net = wb.createNetwork(netName);
    if (!net) {
        net = _db->getNetwork(netName);
    }

    Log::BioLog::log(msg::INFO_READING_EDGES_FROM_NEO4J_DUMP());
    const auto json = nlohmann::json::parse(data);
    const auto& results = json["results"].front()["data"];
    db::StringRef etName;
    db::Node* n1{nullptr};
    db::Node* n2{nullptr};
    db::EdgeType* et{nullptr};
    db::StringRef propName;
    db::Property prop;
    db::PropertyType* propType{nullptr};
    std::string label;

    // for each row
    for (const auto& record : results) {
        const auto& row = record["row"];

        etName = _db->getString(":`" + row.at(0).get<std::string>() + "`");
        et = _db->getEdgeType(etName);
        bioassert(et);

        n1 = net->getNode(_nodeIdMap.at(row.at(1)));
        n2 = net->getNode(_nodeIdMap.at(row.at(2)));

        // Creating node
        db::Edge* e = wb.createEdge(et, n1, n2);

        // For each <PropertyTypeName, value> pair
        for (const auto& [key, val] : row.at(3).items()) {
            propName = _db->getString(key);
            propType = et->getPropertyType(propName);

            if (!propType)
                continue;

            switch (propType->getValueType().getKind()) {

            case db::ValueType::VK_STRING: {
                bool res =
                    wb.setProperty(e, {propType, db::Value::createString(val)});
                bioassert(res);
                break;
            }

            case db::ValueType::VK_INT: {
                bool res =
                    wb.setProperty(e, {propType, db::Value::createInt(val)});
                bioassert(res);
                break;
            }

            case db::ValueType::VK_DECIMAL: {
                bool res =
                    wb.setProperty(e, {propType, db::Value::createDouble(val)});
                bioassert(res);
                break;
            }

            case db::ValueType::VK_BOOL: {
                bool res =
                    wb.setProperty(e, {propType, db::Value::createBool(val)});
                bioassert(res);
                break;
            }

            case db::ValueType::VK_INVALID: {
                // Something went wrong
                Log::BioLog::echo("Invalid prop: " + propName);
                break;
            }

            case db::ValueType::VK_UNSIGNED: {
                // Something went wrong
                Log::BioLog::echo("Invalid prop: " + propName);
                break;
            }

            case db::ValueType::VK_STRING_REF: {
                // Something went wrong
                Log::BioLog::echo("Invalid prop: " + propName);
                break;
            }
            }
        }
    }

    return true;
}
