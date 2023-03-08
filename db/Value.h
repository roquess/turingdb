// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_VALUE_
#define _DB_VALUE_

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
    static Value createString(StringRef data);

    bool isValid() const { return _valid; }

    int64_t getInt() const;
    uint64_t getUint() const;
    bool getBool() const;
    double getDouble() const;
    StringRef getString() const;

private:
    bool _valid {false};
    ValueType _type {ValueType::VK_INT};
    std::variant<int64_t,
                 uint64_t,
                 bool,
                 double,
                 StringRef> _data;
};

}

#endif
