#include "Neo4JSchemaExtract.h"

#include <fstream>

#include <nlohmann/json.hpp>

#include "BioLog.h"
#include "MsgCommon.h"
#include "MsgImport.h"

using namespace Log;

Neo4JSchemaExtract::Neo4JSchemaExtract(const Path& nodesFile, const Path& relationsFile)
    : _nodesFile(nodesFile),
    _relationsFile(relationsFile)
{
}

Neo4JSchemaExtract::~Neo4JSchemaExtract() {
}

bool Neo4JSchemaExtract::extractNodes() {
    std::ifstream jsonFile(_nodesFile.string());
    if (!jsonFile.is_open()) {
        BioLog::log(msg::ERROR_FAILED_TO_OPEN_FOR_READ() << _nodesFile.string());
        return false;
    }

    try {
        BioLog::log(msg::INFO_READING_FILE() << _nodesFile.string());

        const auto doc = nlohmann::json::parse(jsonFile);

        //BioLog::log(msg::INFO_EXTRACTING_ENTITIES_FROM_NEO4J_DUMP());

        const auto& results = doc["results"];
        if (results.empty()) {
            return false;
        }

        const auto& data = results.front()["data"];

        std::string type;
        for (const auto& record : data) {
            const auto& row = record["row"];
            if (row.empty()) {
                continue;
            }

            const auto& obj = row.front();
            type = obj["type"];

            if (_attributes.find(type) == _attributes.end()) {

            }

            for (const auto& entry : obj.items()) {
                _attributes[type].insert(entry.key());
            }
        }
    } catch (const std::exception& e) {
        BioLog::echo(e.what());
        BioLog::log(msg::ERROR_FAILED_TO_PARSE_JSON() << _nodesFile.string());
        return false;
    }

    return true;
}

bool Neo4JSchemaExtract::extractRelations() {
    std::ifstream jsonFile(_relationsFile.string());
    if (!jsonFile.is_open()) {
        BioLog::log(msg::ERROR_FAILED_TO_OPEN_FOR_READ() << _relationsFile.string());
        return false;
    }

    try {
        BioLog::log(msg::INFO_READING_FILE() << _relationsFile.string());

        const auto doc = nlohmann::json::parse(jsonFile);

        //BioLog::log(msg::INFO_EXTRACTING_ENTITIES_FROM_NEO4J_DUMP());

        const auto& results = doc["results"];
        if (results.empty()) {
            return false;
        }

        const auto& data = results.front()["data"];

        std::string source;
        std::string dest;
        std::string relation;
        for (const auto& record : data) {
            const auto& row = record["row"];
            if (row.size() != 5) {
                continue;
            }

            source = row[1].get<std::string>();
            dest = row[4].get<std::string>();
            relation = row[2].get<std::string>();
            if (source.empty() || dest.empty() || relation.empty()) {
                continue;
            }

            _relations[source][relation].emplace(dest);
        }
    } catch (const std::exception& e) {
        BioLog::log(msg::ERROR_FAILED_TO_PARSE_JSON() << _nodesFile.string());
        return false;
    }

    return true;
}
