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

#include <cstdint>

namespace jfern {
/**
 * Reverse the bytes of a generic type
 *
 * @param[in] data The element whose bytes to reverse
 *
 * @return \a data with its bytes reversed
 */
template <typename T>
constexpr T byte_swap(const T& data) noexcept {
    T output(data);

    constexpr unsigned int size = sizeof(T);
    for (unsigned int i = 0; i < size / 2; i++) {
        std::uint8_t* op1 = reinterpret_cast<std::uint8_t*>(&output[i]);
        std::uint8_t* op2 = reinterpret_cast<std::uint8_t*>(&output[size-i-1]);

        std::uint8_t temp = *op1;
        *op1 = *op2;
        *op2 = temp;
    }

    return output;
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
