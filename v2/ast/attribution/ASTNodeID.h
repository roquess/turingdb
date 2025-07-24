#pragma once

#include <spdlog/fmt/bundled/ostream.h>
#include <cstdint>

namespace db {

class ASTNodeID {
public:
    using ValueType = int32_t;

    ASTNodeID() = default;
    ~ASTNodeID() = default;

    ASTNodeID(ValueType v)
        : _value(v)
    {
    }

    ASTNodeID(const ASTNodeID&) = default;
    ASTNodeID& operator=(const ASTNodeID&) = default;
    ASTNodeID(ASTNodeID&&) = default;
    ASTNodeID& operator=(ASTNodeID&&) = default;

    ValueType value() const {
        return _value;
    }

    bool valid() const {
        return _value >= 0;
    }

    bool operator==(const ASTNodeID& other) const {
        return _value == other._value;
    }

    bool operator!=(const ASTNodeID& other) const {
        return !(*this == other);
    }

private:
    ValueType _value {-1};
};

}

namespace std {

template <>
struct hash<db::ASTNodeID> {
    size_t operator()(const db::ASTNodeID& id) const {
        return id.value();
    }
};

inline string to_string(db::ASTNodeID id) {
    return to_string(id.value());
}

inline ostream& operator<<(ostream& os, db::ASTNodeID id) {
    return os << id.value();
}

}

template <>
struct fmt::formatter<db::ASTNodeID> : ostream_formatter {};
