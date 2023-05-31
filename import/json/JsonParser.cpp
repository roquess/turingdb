#include "JsonParser.h"
#include "BioLog.h"
#include "BioAssert.h"
#include "DB.h"
#include "MsgCommon.h"
#include "MsgImport.h"
#include "Node.h"
#include "NodeType.h"
#include "Writeback.h"

#include <fstream>
#include <nlohmann/json.hpp>

const static std::unordered_map<std::string, db::ValueType> neo4jTypes = {
    {"STRING", db::ValueType{db::ValueType::ValueKind::VK_STRING}},
    {"INTEGER", db::ValueType{db::ValueType::ValueKind::VK_INT}}};

JsonParser::JsonParser()
    : _db(db::DB::create())
{
}

JsonParser::JsonParser(db::DB* db)
    : _db(db) {
}

bool JsonParser::parse(const std::filesystem::path& path, Format format) {
    switch (format) {

    case Format::Neo4j4_Properties:
        return parseNeo4j4Properties(path);
        break;

    case Format::Neo4j4_Nodes:
        return parseNeo4j4Nodes(path);
        break;

    case Format::Neo4j4_Edges:
        return parseNeo4j4Edges(path);
        break;
    }

    return false;
}

db::DB* JsonParser::getDB() {
    return _db;
}

bool JsonParser::parseNeo4j4Properties(const std::filesystem::path& path) {
    std::ifstream jsonFile(path.string());

    if (!jsonFile.is_open()) {
        Log::BioLog::log(msg::ERROR_FAILED_TO_OPEN_FOR_READ() << path.string());
        return false;
    }

    db::Writeback wb(_db);
    db::Network* net = wb.createNetwork(_db->getString("Neo4jNetwork"));
    if (!net) {
        net = _db->getNetwork(_db->getString("Neo4jNetwork"));
    }

    try {
        Log::BioLog::log(msg::INFO_READING_FILE() << path.string());

        const auto json = nlohmann::json::parse(jsonFile);

        Log::BioLog::log(msg::INFO_EXTRACTING_ENTITIES_FROM_NEO4J_DUMP());

        const auto& results = json["results"];
        if (results.empty()) {
            return false;
        }

        const auto& data = results.front()["data"];

        for (const auto& record : data) {
            const auto& row = record["row"];
            if (row.empty()) {
                continue;
            }

            const auto& type = _db->getString(row.at(0).front());
            db::NodeType* nt = _db->getNodeType(type);
            if (!nt) {
                nt = wb.createNodeType(type);
            }

            const auto& obj = row.at(1);
            std::string typeName;
            for (const auto& [key, val] : obj.items()) {
                if (key != "synonyms" && key != "type") {
                    db::StringRef propName = _db->getString(key);
                    typeName = val;
                    bioassert(neo4jTypes.find(typeName) != neo4jTypes.end());
                    wb.addPropertyType(nt, propName, neo4jTypes.at(typeName));
                }
            }
        }

        return true;

    } catch (const std::exception& e) {
        Log::BioLog::echo(e.what());
        Log::BioLog::log(msg::ERROR_FAILED_TO_PARSE_JSON() << path.string());
        return false;
    }

    return true;
}

bool JsonParser::parseNeo4j4Nodes(const std::filesystem::path& path) {
    std::ifstream jsonFile(path.string());

    if (!jsonFile.is_open()) {
        Log::BioLog::log(msg::ERROR_FAILED_TO_OPEN_FOR_READ() << path.string());
        return false;
    }

    db::Writeback wb(_db);
    db::Network* net = wb.createNetwork(_db->getString("KnowledgeNetwork"));

    try {
        Log::BioLog::log(msg::INFO_READING_FILE() << path.string());

        const auto json = nlohmann::json::parse(jsonFile);

        Log::BioLog::log(msg::INFO_EXTRACTING_ENTITIES_FROM_NEO4J_DUMP());

        const auto& results = json["results"];
        if (results.empty()) {
            return false;
        }

        const auto& data = results.front()["data"];

        for (const auto& record : data) {
            const auto& row = record["row"];
            if (row.empty()) {
                continue;
            }

            const auto& type = _db->getString(row.at(1).front());
            db::NodeType* nt = _db->getNodeType(type);
            if (!nt) {
                nt = wb.createNodeType(type);
            }

            //db::ComponentType* compType = nt->getBaseComponent();
            [[maybe_unused]] db::Node* node = wb.createNode(net, nt);

            const auto& obj = row.at(0);
            for (const auto& [key, val] : obj.items()) {
                if (key != "synonyms" && key != "id" && key != "type") {
                    //db::StringRef propName = _db->getString(key);

                    //db::Property* prop = wb.addProperty(compType, propName, _db->getStringType());
                    //if (!prop) {
                    //    prop = compType->getProperty(propName);
                    //}
                    // Set the value here using sth like wb.setProperty(node, prop, value);
                }
            }
        }

        return true;

    } catch (const std::exception& e) {
        Log::BioLog::echo(e.what());
        Log::BioLog::log(msg::ERROR_FAILED_TO_PARSE_JSON() << path.string());
        return false;
    }

    return true;
}

bool JsonParser::parseNeo4j4Edges(const std::filesystem::path& path) {
    std::ifstream jsonFile(path.string());

    if (!jsonFile.is_open()) {
        Log::BioLog::log(msg::ERROR_FAILED_TO_OPEN_FOR_READ() << path.string());
        return false;
    }

    db::Writeback wb(_db);
    db::Network* net = wb.createNetwork(_db->getString("KnowledgeNetwork"));

    try {
        Log::BioLog::log(msg::INFO_READING_FILE() << path.string());

        const auto json = nlohmann::json::parse(jsonFile);

        Log::BioLog::log(msg::INFO_EXTRACTING_ENTITIES_FROM_NEO4J_DUMP());

        const auto& results = json["results"];
        if (results.empty()) {
            return false;
        }

        const auto& data = results.front()["data"];

        for (const auto& record : data) {
            const auto& row = record["row"];
            if (row.empty()) {
                continue;
            }

            const auto& type = _db->getString(row.at(1).front());
            db::NodeType* nt = _db->getNodeType(type);
            if (!nt) {
                nt = wb.createNodeType(type);
            }

            //db::ComponentType* compType = nt->getBaseComponent();
            [[maybe_unused]] db::Node* node = wb.createNode(net, nt);

            const auto& obj = row.at(0);
            for (const auto& [key, val] : obj.items()) {
                if (key != "synonyms" && key != "id" && key != "type") {
                    //db::StringRef propName = _db->getString(key);

                    //db::Property* prop = wb.addProperty(compType, propName, _db->getStringType());
                    //if (!prop) {
                    //    prop = compType->getProperty(propName);
                    //}
                    // Set the value here using sth like wb.setProperty(node, prop, value);
                }
            }
        }

        return true;

    } catch (const std::exception& e) {
        Log::BioLog::echo(e.what());
        Log::BioLog::log(msg::ERROR_FAILED_TO_PARSE_JSON() << path.string());
        return false;
    }

    return true;
}
