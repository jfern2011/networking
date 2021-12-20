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

namespace jfern {
int posix_close(int);
template <typename... T> int posix_fcntl(int, int, T&&...);
int posix_poll(struct pollfd[], nfds_t, int);

}  // namespace jfern

#endif  // NETWORKING_POSIX_API_H_
