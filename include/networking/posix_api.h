/**
 *  \file   posix_api.h
 *  \author Jason Fernandez
 *  \date   12/19/2021
 *
 *  https://github.com/jfern2011/networking
 */

#ifndef NETWORKING_POSIX_API_H_
#define NETWORKING_POSIX_API_H_

#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <cstddef>
#include <cstdint>

namespace jfern {
int posix_close(int);
template <typename... T> int posix_fcntl(int, int, T&&...);
int posix_poll(struct pollfd[], nfds_t, int);
ssize_t posix_read(int, std::uint8_t*, std::size_t);
ssize_t posix_write(int, const std::uint8_t*, std::size_t);

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

}  // namespace jfern

#endif  // NETWORKING_POSIX_API_H_
