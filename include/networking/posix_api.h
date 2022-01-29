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
ssize_t posix_read(int fd, std::uint8_t* buf, std::size_t nbytes);
ssize_t posix_write(int fd, const std::uint8_t* buf, std::size_t nbytes);

}  // namespace jfern

#endif  // NETWORKING_POSIX_API_H_
