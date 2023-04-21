// Copyright 2023 Turing Biosystems Ltd.

#include "StringIndex.h"

using namespace db;

StringIndex::StringIndex()
{
}

StringIndex::~StringIndex() {
}

StringRef StringIndex::getString(const std::string& str) {
    if (str.empty()) {
        return StringRef();
    }

    const auto it = _strMap.find(str);
    if (it != _strMap.end()) {
        return StringRef(it->second);
    }

    const std::size_t newID = _strMap.size();
    SharedString* sharedStr = new SharedString(newID, str);
    _strMap[str] = sharedStr;
    return StringRef(sharedStr);
}

