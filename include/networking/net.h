/**
 *  \file   net.h
 *  \author Jason Fernandez
 *  \date   12/17/2021
 *
 *  Copyright 2021 Jason Fernandez
 *
 *  https://github.com/jfern2011/networking
 */

#ifndef NETWORKING_NET_H_
#define NETWORKING_NET_H_

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace jfern {
namespace internal {
template <typename T, std::size_t I>
struct byte_swap {
    static constexpr T calculate(const T& data) noexcept;
};

template <typename T>
struct byte_swap<T, 1u> {
    static constexpr T calculate(const T& data) noexcept;
};

template <typename T>
struct byte_swap<T, 2u> {
    static constexpr T calculate(const T& data) noexcept;
};

template <typename T>
struct byte_swap<T, 4u> {
    static constexpr T calculate(const T& data) noexcept;
};

template <typename T>
struct byte_swap<T, 8u> {
    static constexpr T calculate(const T& data) noexcept;
};

template <typename T, std::size_t I>
constexpr T byte_swap<T, I>::calculate(const T& data) noexcept {
    static_assert(I == 0, "byte_swap: invalid size");
}

/**
 * Reverse the bytes of a single-byte value
 *
 * @param[in] data The element whose bytes to reverse
 *
 * @return \a data with its bytes reversed
 */
template <typename T>
constexpr T byte_swap<T, 1u>::calculate(const T& data) noexcept {
    return data;
}

/**
 * Reverse the bytes of a two-byte value
 *
 * @param[in] data The element whose bytes to reverse
 *
 * @return \a data with its bytes reversed
 */
template <typename T>
constexpr T byte_swap<T, 2u>::calculate(const T& data) noexcept {
    static_assert(std::is_integral<T>::value);
    return ((data >> 8) & 0xff) | ((data << 8) & 0xff00);
}

/**
 * Reverse the bytes of a four-byte value
 *
 * @param[in] data The element whose bytes to reverse
 *
 * @return \a data with its bytes reversed
 */
template <typename T>
constexpr T byte_swap<T, 4u>::calculate(const T& data) noexcept {
    static_assert(std::is_integral<T>::value);

    T x(data);

    x = (x & 0x0000ffff) << 16 | (x & 0xffff0000u) >> 16;
    x = (x & 0x00ff00ff) << 8  | (x & 0xff00ff00u) >> 8;

    return x;
}

/**
 * Reverse the bytes of an eight-byte value
 *
 * @param[in] data The element whose bytes to reverse
 *
 * @return \a data with its bytes reversed
 */
template <typename T>
constexpr T byte_swap<T, 8u>::calculate(const T& data) noexcept {
    static_assert(std::is_integral<T>::value);

    T x(data);

    x = (x & 0x00000000ffffffff) << 32 | (x & 0xffffffff00000000u) >> 32;
    x = (x & 0x0000ffff0000ffff) << 16 | (x & 0xffff0000ffff0000u) >> 16;
    x = (x & 0x00ff00ff00ff00ff) << 8  | (x & 0xff00ff00ff00ff00u) >> 8;

    return x;
}

}  // namespace internal

/**
 * Reverse the bytes of a generic type
 *
 * @param[in] data The element whose bytes to reverse
 *
 * @return \a data with its bytes reversed
 */
template <typename T>
constexpr T byte_swap(const T& data) noexcept {
    return internal::byte_swap<T, sizeof(T)>::calculate(data);
}

/**
 * Check the endianness of this system
 *
 * @return True if the current system is big endian
 */
constexpr bool is_big_endian() {
    constexpr int n = 1;
    return *reinterpret_cast<const std::uint8_t*>(&n) == 1;
}

/**
 * Convert the given data from network to host byte order
 *
 * @param[in] data The data to convert
 *
 * @return \a data with its bytes reversed if this architecture is NOT
 *         big endian
 */
template <typename T>
constexpr T to_host_order(const T& data) noexcept {
    return is_big_endian() ? data : byte_swap<T>(data);
}

/**
 * Convert the given data from host to network byte order
 *
 * @param[in] data The data to convert
 *
 * @return \a data with its bytes reversed if this architecture is NOT
 *         big endian
 */
template <typename T>
constexpr T to_network_order(const T& data) noexcept {
    return to_host_order(data);
}

}  // namespace jfern

#endif  // NETWORKING_NET_H_
