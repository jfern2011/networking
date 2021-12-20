/**
 *  \file   data_buffer.h
 *  \author Jason Fernandez
 *  \date   09/26/2021
 *
 *  Copyright 2021 Jason Fernandez
 *
 *  https://github.com/jfern2011/io_tools
 */

#ifndef DATA_BUFFER_H_
#define DATA_BUFFER_H_

#include <array>
#include <cstddef>
#include <cstdint>

#include "jfern/net.h"

namespace jfern {
/**
 * Implements a fixed-size buffer
 * 
 * @details A DataBuffer encapsulates a contiguous sequence of bytes which can
 *          be operated on using file stream semantics. Data can be written to
 *          or read from the buffer with the desired endianness. An internal
 *          pointer keeps track of the current buffer offset, enabling
 *          operations such as seek(), rewind(), and tell()
 * 
 * @tparam N The size of the buffer in bytes
 */
template <std::size_t N>
class DataBuffer final {
public:
    explicit DataBuffer(bool big_endian);

    DataBuffer(const DataBuffer& buffer)            = default;
    DataBuffer(DataBuffer&& buffer)                 = default;
    DataBuffer& operator=(const DataBuffer& buffer) = default;
    DataBuffer& operator=(DataBuffer&& buffer)      = default;

    ~DataBuffer() = default;

    const std::uint8_t* Data() const noexcept;

    void Rewind()  noexcept;

    bool Seek(long int delta) noexcept;

    bool SeekAbsolute(std::size_t new_offset) noexcept;

    std::size_t Size() const noexcept;

    std::size_t Tell() const noexcept;

    template <typename T>
    bool Read(T* data) noexcept;

    template <typename T>
    bool Write(const T& data) noexcept;

    bool Read(std::uint8_t* data, std::size_t nbytes) noexcept;

    bool Write(const std::uint8_t* data, std::size_t nbytes) noexcept;

private:
    /**
     * The underlying buffer
     */
    std::array<std::uint8_t, N> buf_;

    /**
     * True if this is a big-endian system
     */
    bool big_endian_;

    /**
     * The current buffer offset, in bytes
     */
    std::size_t offset_;
};

/**
 * Constructor
 *
 * @param[in] big_endian If true, data in the underlying buffer is in big
 *                       endian format
 */
template <std::size_t N>
DataBuffer<N>::DataBuffer(bool big_endian)
    : big_endian_(big_endian), offset_(0) {
}

/**
 * Returns a pointer to the underlying storage
 *
 * @return A pointer to the underlying storage
 */
template <std::size_t N>
const std::uint8_t* DataBuffer<N>::Data() const noexcept {
    return buf_.data();
}

/**
 * Reset the current buffer pointer to point to the start of the underlying
 * buffer
 */
template <std::size_t N>
void DataBuffer<N>::Rewind() noexcept {
    offset_ = 0;
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
bool DataBuffer<N>::Seek(long int delta) noexcept {
    if (delta < 0) {
        const auto udelta = static_cast<std::size_t>(-delta);
        if (udelta > offset_) return false;
    } else {
        const auto udelta = static_cast<std::size_t>(delta);
        if (udelta > N || offset_ + udelta > N)
            return false;
    }
    
    offset_ += delta;
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
bool DataBuffer<N>::SeekAbsolute(std::size_t new_offset) noexcept {
    if (new_offset > N) return false;
    
    offset_ = new_offset;
    return true;
}

/**
 * Get the size of the underlying buffer
 *
 * @return The buffer size, in bytes
 */
template <std::size_t N>
std::size_t DataBuffer<N>::Size() const noexcept {
    return N;
}

/**
 * Get the current position of the buffer pointer as an offset from the start
 * of the buffer
 *
 * @return The current buffer offset, in bytes
 */
template <std::size_t N>
std::size_t DataBuffer<N>::Tell() const noexcept {
    return offset_;
}

/**
 * Read an element from the data buffer. If an endian swap is required,
 */
template <typename T>
template <std::size_t N>
bool DataBuffer<N>::Read(T* data) {
    const std::size_t new_pos = offset_ + sizeof(T);
    if (new_pos > N) return false;

    
}

template <typename T>
    bool Write(const T& data) noexcept;

    bool Read(std::uint8_t* data, std::size_t nbytes) noexcept;

    bool Write(const std::uint8_t* data, std::size_t nbytes) noexcept;

}  // namespace jfern

#endif  // DATA_BUFFER_H_
