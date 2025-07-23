#pragma once

#include <variant>

#include "attribution/ExprData.h"

namespace db {
    using DeclData = std::variant<ExprData>;
}
