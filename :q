#include "TypeLibrary.h"
#include "DB.h"
#include "NodeType.h"

namespace db {

TypeLibrary::TypeLibrary(DB* db)
    : _db(db),
      _wb(db)
{
}

NodeType* TypeLibrary::createNodeType(std::string&& name,
                                      std::initializer_list<PropertyTypeDef>&& propTypes) {

    NodeType* nt = _wb.createNodeType(_db->getString(name));

    for (PropertyTypeDef propType : propTypes) {
        _wb.addPropertyType(nt, _db->getString(propType.name), propType.type);
    }

    return nt;
}

EdgeType* TypeLibrary::createEdgeType(const std::unordered_map<const char*, NodeType*>& nts,
                                      std::string&& name,
                                      std::initializer_list<const char*>&& sourceNames,
                                      std::initializer_list<const char*>&& targetNames) {

    std::vector<NodeType*> sources;
    std::vector<NodeType*> targets;

    sources.reserve(sourceNames.size());
    targets.reserve(targetNames.size());

    for (const char* sourceName : sourceNames) {
        sources.push_back(nts.at(sourceName));
    }

    for (const char* targetName : targetNames) {
        targets.push_back(nts.at(targetName));
    }

    return _wb.createEdgeType(_db->getString(name), sources, targets);
    return nullptr;
}

}
