#pragma once

#include "EnumToString.h"

namespace db {

enum class ChangeOpType {
    NEW,
    SUBMIT,
    DELETE,
    LIST,

    _SIZE,
};

using ChangeOpTypeName = EnumToString<ChangeOpType>::Create<
    EnumStringPair<ChangeOpType::NEW, "NEW">,
    EnumStringPair<ChangeOpType::SUBMIT, "SUBMIT">,
    EnumStringPair<ChangeOpType::DELETE, "DELETE">,
    EnumStringPair<ChangeOpType::LIST, "LIST">>;


}
