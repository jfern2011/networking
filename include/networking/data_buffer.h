/**
 *  \file   data_buffer.h
 *  \author Jason Fernandez
 *  \date   09/26/2021
 *
 *  Copyright 2021 Jason Fernandez
 *
 *  https://github.com/jfern2011/networking
 */

#ifndef NETWORKING_DATA_BUFFER_H_
#define NETWORKING_DATA_BUFFER_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "networking/net.h"

namespace jfern {
/**
 * Implements a fixed-size buffer
 * 
 * @details A data_buffer encapsulates a contiguous sequence of bytes which can
 *          be operated on using file stream semantics. Data can be written
 *          to or read from the buffer with the desired endianness. An internal
 *          pointer keeps track of the current buffer offset, enabling
 *          operations such as seek(), rewind(), and tell()
 * 
 * @tparam N The size of the buffer in bytes
 */
template <std::size_t N>
class data_buffer {
public:
    data_buffer();

    data_buffer(const data_buffer& buffer)            = default;
    data_buffer(data_buffer&& buffer)                 = default;
    data_buffer& operator=(const data_buffer& buffer) = default;
    data_buffer& operator=(data_buffer&& buffer)      = default;

    virtual ~data_buffer() = 0;

    const std::uint8_t* data() const noexcept;

    void rewind() noexcept;

    bool seek(long int delta) noexcept;

    bool seek_absolute(std::size_t new_offset) noexcept;

    std::size_t size() const noexcept;

    std::size_t tell() const noexcept;

protected:
    /**
     * The underlying buffer
     */
    std::array<std::uint8_t, N> m_buf;

    /**
     * The current buffer offset, in bytes
     */
    std::size_t m_offset;
};

/**
 * @brief A data_buffer which supports read operations
 */
template <std::size_t N>
class input_buffer final : public data_buffer<N> {
public:
    input_buffer();

    input_buffer(const input_buffer& buffer)            = default;
    input_buffer(input_buffer&& buffer)                 = default;
    input_buffer& operator=(const input_buffer& buffer) = default;
    input_buffer& operator=(input_buffer&& buffer)      = default;

    ~input_buffer() = default;

    template <typename T>
    bool read(T* data, bool bswap) noexcept;

    bool read(std::uint8_t* data, std::size_t nbytes) noexcept;
};

/**
 * @brief A data_buffer which supports write operations
 */
template <std::size_t N>
class output_buffer final : public data_buffer<N> {
public:
    output_buffer();

    output_buffer(const output_buffer& buffer)            = default;
    output_buffer(output_buffer&& buffer)                 = default;
    output_buffer& operator=(const output_buffer& buffer) = default;
    output_buffer& operator=(output_buffer&& buffer)      = default;

    ~output_buffer() = default;

    template <typename T>
    bool write(const T& data, bool bswap) noexcept;

    bool write(const std::uint8_t* data, std::size_t nbytes) noexcept;
};

/**
 * Constructor
 */
template <std::size_t N>
data_buffer<N>::data_buffer() : m_buf(), m_offset(0) {
}

/**
 * Returns a pointer to the underlying storage
 *
 * @return A pointer to the underlying storage
 */
template <std::size_t N>
const std::uint8_t* data_buffer<N>::data() const noexcept {
    return m_buf.data();
}

/**
 * Reset the current buffer pointer to point to the start of the underlying
 * buffer
 */
template <std::size_t N>
void data_buffer<N>::rewind() noexcept {
    m_offset = 0;
}

/**
 * Set the current buffer pointer to point to an offset from the current
 * location
 *
 * @param[in] delta The number of bytes relative to the current buffer offset.
 *                  Can be negative.
 *
 * @note If this method fails, the state of this object remains unchanged
 *
 * @return True if the desired location is within bounds
 */
template <std::size_t N>
bool data_buffer<N>::seek(long int delta) noexcept {
    if (delta < 0) {
        const auto udelta = static_cast<std::size_t>(-delta);
        if (udelta > m_offset) return false;
    } else {
        const auto udelta = static_cast<std::size_t>( delta);
        if (m_offset + udelta > N) return false;
    }
    
    m_offset += delta;
    return true;
}

/**
 * Set the current buffer pointer to point to an offset from the start of the
 * underlying buffer
 *
 * @param[in] delta The number of bytes from the buffer start
 *
 * @note If this method fails, the state of this object remains unchanged
 *
 * @return True if the desired location is within bounds
 */
template <std::size_t N>
bool data_buffer<N>::seek_absolute(std::size_t new_offset) noexcept {
    if (new_offset > N) return false;
    
    m_offset = new_offset;
    return true;
}

/**
 * Get the size of the underlying buffer
 *
 * @return The buffer size, in bytes
 */
template <std::size_t N>
std::size_t data_buffer<N>::size() const noexcept {
    return N;
}

/**
 * Get the current position of the buffer pointer as an offset from the start
 * of the buffer
 *
 * @return The current buffer offset, in bytes
 */
template <std::size_t N>
std::size_t data_buffer<N>::tell() const noexcept {
    return m_offset;
}

/**
 * Constructor
 */
template <std::size_t N>
input_buffer<N>::input_buffer() : data_buffer() {
}

/**
 * Read an element from the data buffer and advance the buffer pointer by
 * the number of bytes read (i.e. the size of the element)
 *
 * @param data  The data element that was read
 * @param bswap If true, byte swap \a data before returning
 *
 * @return True on success, or false if reading would overrun the buffer
 */
template <std::size_t N>
template <typename T>
bool input_buffer<N>::read(T* data, bool bswap) noexcept {
    const std::size_t new_pos = m_offset + sizeof(T);
    if (new_pos > N) return false;

    std::memcpy(data, &m_buf[ m_offset ], sizeof(T));

    if (bswap) *data = byte_swap<T>(*data);

    m_offset = new_pos;
    return true;
}

/**
 * Read from the data buffer and advance the buffer pointer by the number
 * of bytes read
 *
 * @param data   The buffer to read into
 * @param nbytes The number of bytes to read
 *
 * @return True on success, or false if reading would overrun the buffer
 */
template <std::size_t N>
bool input_buffer<N>::read(std::uint8_t* data, std::size_t nbytes) noexcept {
    const std::size_t new_pos = m_offset + nbytes;
    if (new_pos > N) return false;

    std::memcpy(data, &m_buf[ m_offset ], nbytes);

    m_offset = new_pos;
    return true;
}

/**
 * Constructor
 */
template <std::size_t N>
output_buffer<N>::output_buffer() : data_buffer() {
}

/**
 * Write an element to the data buffer and advance the buffer pointer by
 * the number of bytes written (i.e. the size of the element)
 *
 * @param data  The data element to write
 * @param bswap If true, byte swap \a data before writing
 *
 * @return True on success, or false if writing would overrun the buffer
 */
template <std::size_t N>
template <typename T>
bool output_buffer<N>::write(const T& data, bool bswap) noexcept {
    const std::size_t new_pos = m_offset + sizeof(T);
    if (new_pos > N) return false;

    T output = bswap ? byte_swap<T>(data) : data;

    std::memcpy(&m_buf[m_offset], &output, sizeof(T));

    m_offset = new_pos;
    return true;
}

/**
 * Write to the data buffer and advance the buffer pointer by the number
 * of bytes written
 *
 * @param data   The buffer to write into
 * @param nbytes The number of bytes to write
 *
 * @return True on success, or false if writing would overrun the buffer
 */
template <size_t N>
bool output_buffer<N>::write(const std::uint8_t* data,
                             std::size_t nbytes) noexcept {
    const std::size_t new_pos = m_offset + nbytes;
    if (new_pos > N) return false;

    std::memcpy(&m_buf[ m_offset ], data, nbytes);

    m_offset = new_pos;
    return true;
}

}  // namespace jfern

#endif  // NETWORKING_DATA_BUFFER_H_
