// Copyright 2023 Turing Biosystems Ltd.

#include "Link.h"

#include "PropertyType.h"
#include "DB.h"
#include "Object.h"
#include "Network.h"
#include "LinkType.h"

using namespace db;

Link::Link(LinkType* type, Object* source, Object* target)
    : _type(type),
    _source(source),
    _target(target)
{
}

Link::~Link() {
}

Link* Link::create(DB* db, LinkType* type, Object* source, Object* target) {
    Link* link = new Link(type, source, target);
    db->addLink(link);

    source->addOutLink(link);
    target->addInLink(link);

    Network* sourceNet = source->getNetwork();
    Network* targetNet = target->getNetwork();
    if (sourceNet == targetNet) {
        sourceNet->addInternalLink(link);
    } else {
        sourceNet->addOutLink(link);
        targetNet->addInLink(link);
    }

    return link;
}
