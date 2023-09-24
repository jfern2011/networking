/**
 *  \file   file_descriptor.h
 *  \author Jason Fernandez
 *  \date   10/30/2021
 *
 *  https://github.com/jfern2011/networking
 */

#ifndef NETWORKING_FILE_DESCRIPTOR_H_
#define NETWORKING_FILE_DESCRIPTOR_H_

#include <mutex>

namespace jfern {
namespace file_descriptor {
bool close(int fd)                          noexcept;
int poll(int fd, short events, int timeout) noexcept;
bool set_blocking(int fd, bool enable)      noexcept;

bool close(int fd, std::mutex* fd_mutex)                          noexcept;
int poll(int fd, short events, int timeout, std::mutex* fd_mutex) noexcept;
bool set_blocking(int fd, bool enable, std::mutex* fd_mutex)      noexcept;

}  // namespace file_descriptor
}  // namespace jfern

#endif  // NETWORKING_FILE_DESCRIPTOR_H_
