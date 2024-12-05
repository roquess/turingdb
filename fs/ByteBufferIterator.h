#pragma once

#include <span>
#include <string_view>

#include "ByteBuffer.h"

namespace fs {

class ByteBufferIterator {
public:
    explicit ByteBufferIterator(const ByteBuffer& buf)
        : _buf(buf) {
    }

    void reset() {
        _pos = 0;
    }

    template <TrivialPrimitive T>
    const T& get() {
        const auto* ptr = reinterpret_cast<const T*>(_buf.data() + _pos);
        _pos += sizeof(T);
        return *ptr;
    }

    template <TrivialPrimitive T>
    std::span<const T> get(size_t count) {
        const auto* ptr = reinterpret_cast<const T*>(_buf.data() + _pos);
        _pos += sizeof(T) * count;
        return std::span<const T> {ptr, count};
    }

    template <CharPrimitive T>
    std::basic_string_view<T> get(size_t charCount) {
        const auto* ptr = reinterpret_cast<const T*>(_buf.data() + _pos);
        _pos += sizeof(T) * charCount;
        return std::basic_string_view<T> {ptr, charCount};
    }

private:
    const ByteBuffer& _buf;
    size_t _pos {};
};

}
