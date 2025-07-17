#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

namespace db {

class Expression;

class MapLiteral {
public:
    MapLiteral() = default;
    ~MapLiteral() = default;

    MapLiteral(const MapLiteral&) = default;
    MapLiteral(MapLiteral&&) = default;
    MapLiteral& operator=(const MapLiteral&) = default;
    MapLiteral& operator=(MapLiteral&&) = default;

    void set(const std::string& key, Expression* value) {
        _map[key] = value;
    }

private:
    std::unordered_map<std::string, Expression*> _map;
};

class Literal {
public:
    using ValueType = std::variant<bool, int64_t, double, std::string, char, MapLiteral, std::nullopt_t>;

    Literal() = default;

    explicit Literal(ValueType&& vt)
        : _value(std::move(vt)) {
    }

    template <typename T>
    bool is() const {
        return std::holds_alternative<T>(_value);
    }

    template <typename T>
    const T* as() const {
        return std::get_if<T>(&_value);
    }

private:
    ValueType _value;
};

}
