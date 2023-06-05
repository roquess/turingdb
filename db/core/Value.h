// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <variant>
#include <stdint.h>

#include "ValueType.h"
#include "StringRef.h"

namespace db {

class Value {
public:
    Value();
    ~Value();

    static Value createInt(int64_t data);
    static Value createUnsigned(uint64_t data);
    static Value createBool(bool data);
    static Value createDouble(double data);
    static Value createStringRef(StringRef data);
    static Value createString(std::string&& data);

    ValueType getType() const { return _type; }

    bool isValid() const { return _type.isValid(); }

    int64_t getInt() const;
    uint64_t getUint() const;
    bool getBool() const;
    double getDouble() const;
    StringRef getStringRef() const;
    const std::string& getString() const;

private:
    ValueType _type {ValueType::VK_INVALID};
    std::variant<int64_t,
                 uint64_t,
                 bool,
                 double,
                 StringRef,
                 std::string> _data;
};

}
