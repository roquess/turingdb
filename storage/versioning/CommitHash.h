#pragma once

#include <charconv>
#include <spdlog/fmt/ostr.h>
#include <cstdint>
#include <limits>
#include <string>

#include "BasicResult.h"

namespace db {

class CommitHash {
public:
    using ValueType = uint64_t;

    constexpr CommitHash() = default;
    constexpr ~CommitHash() = default;

    constexpr CommitHash(const CommitHash&) = default;
    constexpr CommitHash(CommitHash&&) noexcept = default;
    constexpr CommitHash& operator=(const CommitHash&) = default;
    constexpr CommitHash& operator=(CommitHash&&) noexcept = default;

    constexpr explicit CommitHash(ValueType v)
        : _value(v)
    {
    }

    constexpr CommitHash& operator=(ValueType v) {
        _value = v;
        return *this;
    }

    [[nodiscard]] constexpr ValueType get() const { return _value; }
    [[nodiscard]] constexpr explicit operator ValueType() const { return _value; }

    [[nodiscard]] static CommitHash create();

    [[nodiscard]] static consteval CommitHash head() { return CommitHash {}; }

    [[nodiscard]] bool operator==(const CommitHash& other) const {
        return _value == other._value;
    }

    [[nodiscard]] bool operator!=(const CommitHash& other) const {
        return !(*this == other);
    }

    [[nodiscard]] bool operator<(const CommitHash& other) const {
        return _value < other._value;
    }

    [[nodiscard]] bool operator<=(const CommitHash& other) const {
        return _value <= other._value;
    }

    [[nodiscard]] bool operator>=(const CommitHash& other) const {
        return _value >= other._value;
    }

    [[nodiscard]] bool operator>(const CommitHash& other) const {
        return _value > other._value;
    }

    [[nodiscard]] static BasicResult<CommitHash, std::string_view> fromString(std::string_view str) {
        CommitHash::ValueType hashValue = CommitHash::head().get();
        if (str != "head") {
            const char* begin = str.data();
            const char* end = str.data() + str.size();
            const auto res = std::from_chars(begin, end, hashValue, 16);

            if (res.ec == std::errc::result_out_of_range) {
                return BadResult<std::string_view>("Too large hash value");
            } else if (res.ec == std::errc::invalid_argument) {
                return BadResult<std::string_view>("Invalid hash value");
            } else if (res.ptr != end) {
                return BadResult<std::string_view>("String contains invalid characters");
            }

            return CommitHash(hashValue);
        }

        return CommitHash::head();
    }

private:
    ValueType _value = std::numeric_limits<ValueType>::max();
};

}

template <>
struct std::hash<db::CommitHash> {
    size_t operator()(const db::CommitHash& h) const {
        return h.get();
    }
};

namespace std {

inline string to_string(db::CommitHash h) {
    return to_string(h.get());
}

template <typename T>
ostream& operator<<(ostream& os, db::CommitHash h) {
    return os << h.get();
}

}

template <>
struct fmt::formatter<db::CommitHash> : ostream_formatter {};
