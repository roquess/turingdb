#pragma once

#include "spdlog/fmt/bundled/core.h"
#include <cstdint>
#include <optional>
#include <string>
#include <variant>

namespace db {

class Literal {
public:
    using ValueType = std::variant<bool, int64_t, double, std::string, char, std::nullopt_t>;

    Literal() = default;

    explicit Literal(ValueType&& vt)
        : _value(std::move(vt)) {
        constexpr auto printVisit = [](auto&& v) {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, std::nullopt_t>) {
                fmt::print("Literal(nullopt)\n");
            } else {
                fmt::print("Literal('{}')\n", v);
            }
        };
        std::visit(printVisit, _value);
    }

private:
    ValueType _value;
};

}
