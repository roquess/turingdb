// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_STRING_REF_
#define _DB_STRING_REF_

#include <utility>

#include "SharedString.h"

namespace db {

class StringRef {
public:
    using iterator = const char*;

    struct Hash {
        std::size_t operator()(const StringRef& str) const {
            const SharedString* sharedStr = str._sharedStr;
            return std::hash<std::size_t>()(sharedStr ? sharedStr->getID() : 0);
        }
    };

    struct Comparator {
        bool operator()(StringRef lhs, StringRef rhs) const {
            if (!lhs._sharedStr) {
                return true;
            }
            if (!rhs._sharedStr) {
                return false;
            }
            return lhs._sharedStr->getID() < rhs._sharedStr->getID();
        }
    };

    StringRef() = default;
    StringRef(const SharedString* sharedStr);
    ~StringRef() = default;

    bool operator==(const StringRef& other) const { return _sharedStr == other._sharedStr; }

    bool empty() const { return _sharedStr == nullptr; }

    std::size_t size() const;

    iterator begin() const;
    iterator end() const;

    std::string toStdString() const {
        return _sharedStr ? _sharedStr->getString() : std::string();
    }

private:
    const SharedString* _sharedStr {nullptr};
};

}

#endif
