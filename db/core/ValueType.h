// Copyright 2023 Turing Biosystems Ltd.

#pragma once

namespace db {

class ValueType {
public:
    enum ValueKind {
        VK_INVALID = 0,
        VK_INT,
        VK_UNSIGNED,
        VK_BOOL,
        VK_DECIMAL,
        VK_STRING
    };

    ValueType(ValueKind valueKind);

    ValueKind getKind() const { return _kind; }
    bool isValid() const { return _kind != VK_INVALID; }
    bool isInt() const { return _kind == VK_INT; }
    bool isUnsigned() const { return _kind == VK_UNSIGNED; }
    bool isBool() const { return _kind == VK_BOOL; }
    bool isDecimal() const { return _kind == VK_DECIMAL; }
    bool isString() const { return _kind == VK_STRING; }

private:
    ValueKind _kind;
};

}
