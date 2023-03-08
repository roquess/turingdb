// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_PROPERTY_STORE_
#define _DB_PROPERTY_STORE_

#include <vector>

#include "Value.h"

namespace db {

class PropertyStore {
public:
    using Values = std::vector<Value>;

    PropertyStore(std::size_t size);
    ~PropertyStore();

    Value getProperty(std::size_t index) const { return _entries[index]; }

    void setProperty(std::size_t index, Value val) {
        _entries[index] = val;
    }

private:
    Values _entries;
};

}

#endif
