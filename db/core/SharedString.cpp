// Copyright 2023 Turing Biosystems Ltd.

#include "SharedString.h"

using namespace db;

SharedString::SharedString(ID strID, const std::string& str)
    : _id(strID), _str(str)
{
}

SharedString::~SharedString() {
}

bool SharedString::isEqual(const SharedString* other) const {
    return _str == other->_str;
}
