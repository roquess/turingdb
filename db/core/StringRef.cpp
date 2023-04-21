// Copyright 2023 Turing Biosystems Ltd.

#include "StringRef.h"

using namespace db;

StringRef::StringRef(const SharedString* sharedStr)
    : _sharedStr(sharedStr)
{
}

std::size_t StringRef::size() const {
    if (!_sharedStr) {
        return 0;
    } else {
        return _sharedStr->getString().size();
    }
}

StringRef::iterator StringRef::begin() const {
    if (!_sharedStr) {
        return end();
    }
    return _sharedStr->getString().c_str();
}

StringRef::iterator StringRef::end() const {
    if (!_sharedStr) {
        return nullptr;
    }
    const std::string& str = _sharedStr->getString();
    return str.c_str()+str.size();
}
