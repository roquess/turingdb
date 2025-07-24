#pragma once

#include "EnumToString.h"

namespace db {

enum class VariableType : uint8_t {
    Invalid = 0,

    NodePattern,
    EdgePattern,

    Null,
    Integer,
    Double,
    String,
    Char,
    Bool,
    List,
    Map,

    _SIZE,
};

using VariableTypeName = EnumToString<VariableType>::Create<
    EnumStringPair<VariableType::Invalid, "Invalid">,
    EnumStringPair<VariableType::NodePattern, "NodePattern">,
    EnumStringPair<VariableType::EdgePattern, "EdgePattern">,
    EnumStringPair<VariableType::Null, "Null">,
    EnumStringPair<VariableType::Integer, "Integer">,
    EnumStringPair<VariableType::Double, "Double">,
    EnumStringPair<VariableType::String, "String">,
    EnumStringPair<VariableType::Char, "Char">,
    EnumStringPair<VariableType::Bool, "Bool">,
    EnumStringPair<VariableType::List, "List">,
    EnumStringPair<VariableType::Map, "Map">>;
}
