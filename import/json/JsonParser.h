#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>

namespace db {
    class DB;
}

class JsonParser {
public:
    enum class Format{
        Neo4j4_NodeProperties = 0,
        Neo4j4_Nodes,
        Neo4j4_EdgeProperties,
        Neo4j4_Edges,
    };

    JsonParser();
    JsonParser(db::DB* db);

    bool parse(const std::string& path, Format format);
    db::DB* getDB();

private:
    db::DB* _db{nullptr};
    std::unordered_map<size_t, size_t> _nodeIdMap;

    bool parseNeo4j4NodeProperties(const std::string& path);
    bool parseNeo4j4Nodes(const std::string& path);
    bool parseNeo4j4EdgeProperties(const std::string& path);
    bool parseNeo4j4Edges(const std::string& path);
};
