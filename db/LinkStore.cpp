// Copyright 2023 Turing Biosystems Ltd.

#include "LinkStore.h"

using namespace db;

LinkStore::LinkStore(std::size_t size)
    : _linkSlots(size)
{
}

LinkStore::~LinkStore() {
}

void LinkStore::addLink(Link* link, std::size_t index) {
    _linkSlots[index].push_back(link);
}
