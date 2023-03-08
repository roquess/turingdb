// Copyright 2023 Turing Biosystems Ltd.

#ifndef _DB_VALUE_TYPE_
#define _DB_VALUE_TYPE_

namespace db {

class DB;

class ValueType {
public:
    enum ValueKind {
        VK_INT,
        VK_UNSIGNED,
        VK_BOOL,
        VK_DECIMAL,
        VK_STRING
    };

    ValueType(ValueKind);

    ValueKind getKind() const { return _kind; }
    bool isInt() const { return false; }
    bool isUnsigned() const { return false; }
    bool isBool() const { return false; }
    bool isDecimal() const { return false; }
    bool isString() const { return false; }

private:
    ValueKind _kind;
};

}

#endif
