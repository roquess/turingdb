#ifndef _NEO4J_SCHEMA_EXTRACT_
#define _NEO4J_SCHEMA_EXTRACT_

 #include <filesystem>
 #include <set>
 #include <string>
 #include <map>

class Neo4JSchemaExtract {
public:
    using Path = std::filesystem::path;
    using StringSet = std::set<std::string>;
    using ClassAttributesMap = std::map<std::string, StringSet>;
    using RelationMap = std::map<std::string, std::map<std::string, StringSet>>;

    Neo4JSchemaExtract(const Path& nodesFile, const Path& relationsFile);
    ~Neo4JSchemaExtract();

    bool extractNodes();
    bool extractRelations();

    const StringSet& getSchemaClasses() const { return _schemaClasses; }
    const ClassAttributesMap& getClassAttributes() const { return _attributes; }
    const RelationMap& getRelations() const { return _relations; }

private:
    Path _nodesFile;
    Path _relationsFile;
    StringSet _schemaClasses;
    ClassAttributesMap _attributes;
    RelationMap _relations;
};

#endif
