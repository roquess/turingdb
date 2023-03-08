// Copyright 2023 Turing Biosystems Ltd.

#include "LinkType.h"

#include "DB.h"
#include "ObjectType.h"

using namespace db;

LinkType::LinkType(StringRef name,
                   ObjectType* sourceType,
                   ObjectType* targetType,
                   std::size_t sourceIndex,
                   std::size_t targetIndex)
    : _name(name),
    _sourceType(sourceType),
    _targetType(targetType),
    _sourceIndex(sourceIndex),
    _targetIndex(targetIndex)
{
}

LinkType::~LinkType() {
}

LinkType* LinkType::create(DB* db,
                           StringRef name,
                           ObjectType* source,
                           ObjectType* target) {
    const std::size_t sourceIndex = source->getInLinkTypes().size();
    const std::size_t targetIndex = target->getOutLinkTypes().size();
    LinkType* linkType = new LinkType(name, source, target, sourceIndex, targetIndex);
    db->addLinkType(linkType);

    source->addOutLinkType(linkType);
    target->addInLinkType(linkType);

    return linkType;
}

void LinkType::addProperty(PropertyType* prop) {
    _propTypes.push_back(prop);
}
