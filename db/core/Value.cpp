// Copyright 2023 Turing Biosystems Ltd.

#include "Value.h"

using namespace db;

Value::Value()
{
}

Value::~Value() {
}

Value Value::createInt(int64_t data) {
    Value val;
    val._type = ValueType::VK_INT;
    val._data = data;
    return val;
}

Value Value::createUnsigned(uint64_t data) {
    Value val;
    val._type = ValueType::VK_UNSIGNED;
    val._data = data;
    return val;
}

Value Value::createBool(bool data) {
    Value val;
    val._type = ValueType::VK_BOOL;
    val._data = data;
    return val;
}

Value Value::createDouble(double data) {
    Value val;
    val._type = ValueType::VK_DECIMAL;
    val._data = data;
    return val;
}

Value Value::createStringRef(StringRef data) {
    Value val;
    val._type = ValueType::VK_STRING_REF;
    val._data = data;
    return val;
}

Value Value::createString(std::string&& data) {
    Value val;
    val._type = ValueType::VK_STRING;
    val._data = std::forward<std::string>(data);
    return val;
}
