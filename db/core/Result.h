#pragma once

#include "BasicResult.h"

namespace db {

enum class Error {
    DELETED_OBJECT
};

template <class Value>
using Result = BasicResult<Error, Value>;

}