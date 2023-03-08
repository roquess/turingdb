// Copyright 2023 Turing Biosystems Ltd.

#include "DB.h"

#include "Network.h"
#include "ValueType.h"
#include "Link.h"
#include "ObjectType.h"
#include "LinkType.h"

using namespace db;

DB::DB()
{
    _int = new ValueType(ValueType::VK_INT);
    _unsigned = new ValueType(ValueType::VK_UNSIGNED);
    _bool = new ValueType(ValueType::VK_BOOL);
    _decimal = new ValueType(ValueType::VK_DECIMAL);
    _string = new ValueType(ValueType::VK_STRING);
}

DB::~DB() {
    delete _int;
    delete _unsigned;
    delete _bool;
    delete _decimal;
    delete _string;

    for (Network* net : _networks) {
        delete net;
    }

    for (Link* link : _links) {
        delete link;
    }

    for (ObjectType* objType : _objTypes) {
        delete objType;
    }

    for (LinkType* linkType : _linkTypes) {
        delete linkType;
    }
}

DB* DB::create() {
    DB* db = new DB();
    return db;
}

StringRef DB::getString(const std::string& str) {
    return _strIndex.getString(str);
}

void DB::addNetwork(Network* net) {
    _networks.push_back(net);
}

void DB::addLink(Link* link) {
    _links.push_back(link);
}

void DB::addObjectType(ObjectType* obj) {
    _objTypes.push_back(obj);
}

void DB::addLinkType(LinkType* link) {
    _linkTypes.push_back(link);
}
