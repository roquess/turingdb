#pragma once

#include <unordered_map>

#include "Writeback.h"

namespace db {
class DB;
class NodeType;

class TypeLibrary {
public:
    struct PropertyTypeDef {
        std::string name;
        ValueType type;
    };

    TypeLibrary(DB* db);
    virtual ~TypeLibrary() = default;
    virtual bool load() = 0;

protected:
    NodeType* createNodeType(std::string&& name,
                             std::initializer_list<PropertyTypeDef>&& propTypes = {});
    EdgeType* createEdgeType(const std::unordered_map<const char*, NodeType*>& nts,
                             std::string&& name,
                             std::initializer_list<const char*>&& sourceNames,
                             std::initializer_list<const char*>&& targetNames);

private:
    DB* _db {nullptr};
    Writeback _wb;
};

}
