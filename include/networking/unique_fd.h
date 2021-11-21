/**
 *  \file   unique_fd.h
 *  \author Jason Fernandez
 *  \date   10/30/2021
 *
 *  https://github.com/jfern2011/networking
 */

#ifndef NETWORKING_UNIQUE_FD_H_
#define NETWORKING_UNIQUE_FD_H_

#include "fd_interface.h"

namespace jfern {
/**
 * @class unique_fd
 *
 * Manages a single file descriptor, employing semantics similar to
 * std::unique_ptr. An unique_fd cannot be copied, but ownership of the file
 * descriptor it manages can be transferred to another instance. The file
 * descriptor is closed once the owning instance is destroyed
 */
class unique_fd final : public fd_interface {
public:
    unique_fd();

    explicit unique_fd(int fd);

    unique_fd(const unique_fd& fd)            = delete;
    unique_fd(unique_fd&& fd);
    unique_fd& operator=(const unique_fd& fd) = delete;
    unique_fd& operator=(unique_fd&& fd);

    ~unique_fd();

    explicit operator bool() const noexcept override;

    int get() const noexcept override;

    bool is_blocking() const noexcept override;

    int poll(short events) noexcept override;

    int poll(short events,
             fd_interface::event_handler_t handler) override;

    int poll(short events, int timeout) noexcept override;

    int poll(short events, int timeout,
             fd_interface::event_handler_t handler) override;

    int release();

    bool reset(int fd) noexcept override;

    bool set_blocking(bool enable) noexcept override;

    void swap(unique_fd& fd) noexcept;

private:
    /** True if blocking behavior is enabled */
    bool m_blocking;

    /** The actual file descriptor */
    int m_fd;
};

}  // namespace jfern

#endif  // NETWORKING_UNIQUE_FD_H_
