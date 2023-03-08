// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_STRING_INDEX_
#define _DB_STRING_INDEX_

#include <string>
#include <unordered_map>

#include "StringRef.h"

namespace db {

class StringIndex {
public:
    StringIndex();
    ~StringIndex();

    StringRef getString(const std::string& str);

private:
    std::unordered_map<std::string, SharedString*> _strMap;
};

}

#endif
