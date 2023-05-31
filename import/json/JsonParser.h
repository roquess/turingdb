#pragma once

#include <string>
#include <filesystem>

namespace db {
    class DB;
}

class JsonParser {
public:
    enum class Format{
        Neo4j4_Properties = 0,
        Neo4j4_Nodes,
        Neo4j4_Edges,
    };

    JsonParser();
    JsonParser(db::DB* db);

    bool parse(const std::filesystem::path& path, Format format);
    db::DB* getDB();

private:
    db::DB* _db{nullptr};

    bool parseNeo4j4Properties(const std::filesystem::path& path);
    bool parseNeo4j4Nodes(const std::filesystem::path& path);
    bool parseNeo4j4Edges(const std::filesystem::path& path);
};
