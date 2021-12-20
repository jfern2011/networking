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
 * @brief Wrapper to the POSIX fcntl() function
 * 
 * @tparam T Types of any variadic arguments passed to fcntl()
 *
 * @param fd  The file descriptor to operate on
 * @param cmd The command to perform on \a fd
 * @param args Additional command arguments, if required
 *
 * @return A command-specific return code. On error, returns -1 and sets errno
 */
template <typename... T>
int posix_fcntl(int fd, int cmd, T&&... args) {
    return ::fcntl(fd, cmd, std::forward<T>(args)...);
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

}  // namespace jfern