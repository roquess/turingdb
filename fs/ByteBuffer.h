#pragma once

#include <cstdint>
#include <vector>

namespace fs {

using Byte = uint8_t;
using ByteBuffer = std::vector<uint8_t>;

template <typename T>
concept CharPrimitive = std::same_as<T, char>
                     || std::same_as<T, signed char>
                     || std::same_as<T, unsigned char>
                     || std::same_as<T, wchar_t>
                     || std::same_as<T, char8_t>
                     || std::same_as<T, char16_t>
                     || std::same_as<T, char32_t>;

template <typename T>
concept TrivialPrimitive = (std::integral<T>
                            || std::floating_point<T>
                            || std::same_as<T, bool>)
                        && !CharPrimitive<T>;

}
