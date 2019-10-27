#pragma once

#include <cstdint>
#include <type_traits>

template<typename E>
constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type 
{
   return static_cast<typename std::underlying_type<E>::type>(e);
}

enum class PlatformEndianness
{
    BigEndian,
    LittleEndian
};

constexpr PlatformEndianness DetectEndianness()
{
    union {
        uint32_t i;
        char c[4];
    } testEndian = {0x01020304};

    return (testEndian.c[0] == 1)? PlatformEndianness::BigEndian:PlatformEndianness::LittleEndian;
}

template <typename T>
T SwapEndian(const T &val) 
{
    union U 
    {
        typename std::remove_const<T>::type val;
        std::array<std::uint8_t, sizeof(T)> raw;
    } src, dst;

    src.val = val;
    std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
    return dst.val;
}