#ifndef JFERN_NET_H_
#define JFERN_NET_H_

#include <cstdint>

namespace jfern {
namespace internal {
/**
 * Reverse the bytes of a 16, 32, or 64-bit integer. For all other types, this
 * is a no-op
 * 
 * @param[in] data The element whose bytes to reverse
 * 
 * @return \a data with its bytes reversed
 */
template <typename T>
constexpr T ByteSwap(const T& data) noexcept {
    return data;
}
/**
 * Byte swap a 16-bit unsigned integer
 *
 * @param[in] data The integer whose bytes to reverse
 *
 * @return The byte-swapped value
 */
template <>
constexpr std::uint16_t ByteSwap<std::uint16_t>(std::uint16_t data) noexcept {
    return (data >> 8) | (data << 8);
}

/**
 * Byte swap a 16-bit signed integer
 *
 * @param[in] data The integer whose bytes to reverse
 *
 * @return The byte-swapped value
 */
template <>
constexpr std::int16_t ByteSwap<std::int16_t>(std::int16_t data) noexcept {
    return ((data >> 8) & 0xff) | (data << 8);
}

/**
 * Byte swap a 32-bit unsigned integer
 *
 * @param[in] data The integer whose bytes to reverse
 *
 * @return The byte-swapped value
 */
template <>
constexpr std::uint32_t ByteSwap<std::uint32_t>(std::uint32_t data) noexcept {
    std::uint32
    return (data >> 24) |
           (data << 24) |
           ((data >> 8) & 0xff00) |
           ((data << 8) & 0xff0000);
}

/**
 * Byte swap a 32-bit signed integer
 *
 * @param[in] data The integer whose bytes to reverse
 *
 * @return The byte-swapped value
 */
template <>
constexpr std::int32_t ByteSwap<std::int32_t>(std::int32_t data) noexcept {
    std::uint32_t arg = data;
    std::uint8_t* ptr = reinterpret_cast<std::uint8_t*>(&arg);

    Swap(ptr[0], ptr[3]);
    Swap(ptr[1], ptr[2]);

    return arg;
}

}  // namespace internal

enum class NetError {
    success
};

template <typename T>
constexpr T EndianSwap(const T& data) noexcept {
    return data;
}

template <>
constexpr std::uint16_t EndianSwap<std::uint16_t>(const std::uint16_t& data)
    noexcept {
    return (data >> 8) | (data << 8);
}
template <>
constexpr std::int16_t EndianSwap<std::int16_t>(const std::int16_t& data)
    noexcept {
    return ((data >> 8) & 0xff) | (data << 8);
}
template <>
constexpr std::uint32_t EndianSwap<std::uint32_t>(const std::uint32_t& data)
    noexcept {
    return (data >> 24) | (data << 24) | ((data >> 8) & 0xff00) |
           ((data << 8) & 0xff0000);
}
template <>
constexpr std::int32_t EndianSwap<std::int32_t>(const std::int32_t& data)
    noexcept {
    return ((data >> 24) & 0xff) | (data << 24) | ((data >> 8) & 0xff00) |
           ((data << 8) & 0xff0000);
}
template <>
constexpr std::uint64_t EndianSwap<std::uint64_t>(const std::uint64_t& data)
    noexcept {
    return (data >> 56) | (data << 56) | ((data >> 48) & 0xff00) | ((data << 48) & 0xff000000000000) | ((data >> 40) & 0xff000000) | ((data << 40) & 0xff00000000)
}
template <>
constexpr std::int64_t EndianSwap<std::int64_t>(const std::int64_t& data)
    noexcept {
    return ((data >> 24) & 0xff) | (data << 24) | ((data >> 8) & 0xff00) |
           ((data << 8) & 0xff0000);
}

/**
 * Check the endianness of this system
 * 
 * @return True if the current system is big endian
 */
constexpr bool IsBigEndian() {
    constexpr int n = 1;
    return *reinterpret_cast<const char*>(&n) == 1;
}

}  // namespace jfern

#endif  // JFERN_NET_H_
