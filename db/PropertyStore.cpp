// Copyright 2023 Turing Biosystems Ltd.

#include "PropertyStore.h"

using namespace db;

PropertyStore::PropertyStore(std::size_t size)
    : _entries(size)
{
}

PropertyStore::~PropertyStore() {
}
