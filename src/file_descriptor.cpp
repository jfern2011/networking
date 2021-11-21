/**
 *  \file   file_descriptor.cpp
 *  \author Jason Fernandez
 *  \date   10/30/2021
 *
 *  https://github.com/jfern2011/networking
 */

#include "file_descriptor.h"

#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

namespace jfern {
namespace file_descriptor {
/**
 * Close a file descriptor
 *
 * @param[in] fd The file descriptor to close
 *
 * @return True on success. On error, errno is set
 */
bool close(int fd) noexcept {
    return ::close(fd) == 0;
}

/**
 * Poll for file descriptor events
 *
 * @param[in] fd      The file descriptor to poll
 * @param[in] events  A bitmask of the events to wait on
 * @param[in] timeout Wait for at most this many milliseconds before returning
 * 
 * @return The returned events, or -1 on error
 */
int poll(int fd, short events, int timeout) noexcept {
    struct ::pollfd pfd; pfd.events = events;
    pfd.fd = fd;

    const int res = ::poll(&pfd, 1, timeout);
    if (res < 0) return -1;
    if (res > 0) return pfd.revents;

    return 0;
}

/**
 * Set blocking behavior on the given file descriptor
 * 
 * @param[in] fd     The file descriptor
 * @param[in] enable True to enable blocking, false otherwise
 * 
 * @return True on success. On error, errno is set
 */
bool set_blocking(int fd, bool enable) noexcept {
    const int flags = ::fcntl(fd, F_GETFL, 0);
    if (flags < 0) return false;

    if (!enable) {
        if (::fcntl(fd, F_SETFL, flags |  (O_NONBLOCK)) < 0)
            return false;
    } else {
        if (::fcntl(fd, F_SETFL, flags & (~O_NONBLOCK)) < 0)
            return false;
    }

    return true;
}

/**
 * Close a file descriptor
 *
 * @param[in] fd       The file descriptor to close
 * @param[in] fd_mutex A mutex shared among threads to avoid simultaneous
 *                     system calls on the same file descriptor
 *
 * @return True on success. On error, errno is set
 */
bool close(int fd, std::mutex* fd_mutex) noexcept {
    const std::lock_guard<std::mutex> lock(*fd_mutex);
    return close(fd);
}

/**
 * Poll for file descriptor events
 *
 * @param[in] fd       The file descriptor to poll
 * @param[in] events   A bitmask of the events to wait on
 * @param[in] timeout  Wait for at most this many milliseconds before returning
 * @param[in] fd_mutex A mutex shared among threads to avoid simultaneous
 *                     system calls on the same file descriptor
 * 
 * @return The returned events, or -1 on error
 */
int poll(int fd, short events, int timeout, std::mutex* fd_mutex) noexcept {
    const std::lock_guard<std::mutex> lock(*fd_mutex);
    return poll(fd, events, timeout);
}

/**
 * Set blocking behavior on the given file descriptor
 * 
 * @param[in] fd       The file descriptor
 * @param[in] enable   True to enable blocking, false otherwise
 * @param[in] fd_mutex A mutex shared among threads to avoid simultaneous
 *                     system calls on the same file descriptor
 * 
 * @return True on success. On error, errno is set
 */
bool set_blocking(int fd, bool enable, std::mutex* fd_mutex) noexcept {
    const std::lock_guard<std::mutex> lock(*fd_mutex);
    return set_blocking(fd, enable);
}

}  // namespace file_descriptor
}  // namespace jfern
