// Copyright 2023 Turing Biosystems Ltd.

#pragma once

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
