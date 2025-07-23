#pragma once

#include "EnumToString.h"

namespace db {

enum class VariableType : uint8_t {
    Node = 0,
    Edge,

    Null,
    Integer,
    Double,
    String,
    Char,
    Bool,
    List,
    Map,

    Invalid,

    _SIZE,
};

using VariableTypeName = EnumToString<VariableType>::Create<
    EnumStringPair<VariableType::Node, "Node">,
    EnumStringPair<VariableType::Edge, "Edge">,
    EnumStringPair<VariableType::Null, "Null">,
    EnumStringPair<VariableType::Integer, "Integer">,
    EnumStringPair<VariableType::Double, "Double">,
    EnumStringPair<VariableType::String, "String">,
    EnumStringPair<VariableType::Char, "Char">,
    EnumStringPair<VariableType::Bool, "Bool">,
    EnumStringPair<VariableType::List, "List">,
    EnumStringPair<VariableType::Map, "Map">,
    EnumStringPair<VariableType::Invalid, "Invalid">>;

}
