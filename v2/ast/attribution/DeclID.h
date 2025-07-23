#pragma once

#include <spdlog/fmt/bundled/ostream.h>
#include <cstdint>

namespace db {

class DeclID {
public:
    using ValueType = int32_t;

    DeclID() = default;
    ~DeclID() = default;

    DeclID(ValueType v)
        : _value(v)
    {
    }

    DeclID(const DeclID&) = default;
    DeclID& operator=(const DeclID&) = default;
    DeclID(DeclID&&) = default;
    DeclID& operator=(DeclID&&) = default;

    ValueType value() const {
        return _value;
    }

    bool valid() const {
        return _value >= 0;
    }

private:
    ValueType _value {-1};
};

}

namespace std {

template <>
struct hash<db::DeclID> {
    size_t operator()(const db::DeclID& id) const {
        return id.value();
    }
};

inline string to_string(db::DeclID id) {
    return to_string(id.value());
}

inline ostream& operator<<(ostream& os, db::DeclID id) {
    return os << id.value();
}

}

template <>
struct fmt::formatter<db::DeclID> : ostream_formatter {};
