/**
 *  \file   posix_api.cpp
 *  \author Jason Fernandez
 *  \date   12/19/2021
 *
 *  https://github.com/jfern2011/networking
 */

#include "networking/posix_api.h"

#include <utility>

namespace jfern {
/**
 * @brief Wrapper to the POSIX close() function
 *
 * @param fd The file descriptor to close
 *
 * @return Zero on success. On error, returns -1 and sets errno
 */
int posix_close(int fd) {
    return ::close(fd);
}

/**
 * @brief Wrapper to the POSIX poll() function
 *
 * @param pfds    The file descriptors being polled
 * @param nfds    The number of file descriptors
 * @param timeout The max number of milliseconds to wait for a file descriptor
 *                to become ready
 *
 * @return The number of pollfd structs that returned events. On error,
 *         returns -1 and sets errno
 */
int posix_poll(struct pollfd pfds[], nfds_t nfds, int timeout) {
    return ::poll(pfds, nfds, timeout);
}

/**
 * @brief Wrapper to the POSIX read() function
 * 
 * @param fd     The file descriptor to read from
 * @param buf    The buffer to place data into
 * @param nbytes The maximum number of bytes to attempt to read
 *
 * @return The number of bytes read. On error, returns -1 and sets errno
 */
ssize_t posix_read(int fd, std::uint8_t* buf, std::size_t nbytes) {
    return ::read(fd, buf, nbytes);
}

/**
 * @brief Wrapper to the POSIX write() function
 * 
 * @param fd     The file descriptor to write to
 * @param buf    The data to write to the file descriptor
 * @param nbytes Number of bytes to attempt to write
 *
 * @return The number of bytes actually written. On error, returns -1 and
 *         sets errno
 */
ssize_t posix_write(int fd, const std::uint8_t* buf, std::size_t nbytes) {
    return ::write(fd, buf, nbytes);
}

}  // namespace jfern
