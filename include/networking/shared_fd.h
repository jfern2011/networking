/**
 *  \file   shared_fd.h
 *  \author Jason Fernandez
 *  \date   11/05/2021
 *
 *  https://github.com/jfern2011/networking
 */

#ifndef NETWORKING_SHARED_FD_H_
#define NETWORKING_SHARED_FD_H_

#include <atomic>
#include <cstddef>
#include <mutex>

#include "fd_interface.h"
#include "fd_internal.h"

namespace jfern {
/**
 * @class shared_fd
 *
 * Manages a single file descriptor, employing semantics similar to
 * std::shared_ptr
 *
 * @details
 * A shared_fd retains ownership of a file descriptor until (1) the file
 * descriptor is explicitly released via a call to release(), (2) is replaced
 * via a call to reset(), or (3) the shared_fd is destructed. The file
 * descriptor is reference counted to ensure it is only ever closed once,
 * even when shared across multiple threads
 *
 * @note
 * For the purposes of concurrency - all operations on a shared_fd (including
 * the use of any of the polling interfaces) may be treated as atomic
 */
class shared_fd final : public fd_interface {
public:
    shared_fd();

    explicit shared_fd(int fd);

    shared_fd(const shared_fd& fd);
    shared_fd(shared_fd&& fd);
    shared_fd& operator=(const shared_fd& fd);
    shared_fd& operator=(shared_fd&& fd);

    ~shared_fd();

    explicit operator bool() const noexcept override;

    int get() const noexcept override;

    bool is_blocking() const noexcept override;

    int poll(short events) noexcept override;

    int poll(short events,
             fd_interface::event_handler_t handler) override;

    int poll(short events, int timeout) noexcept override;

    int poll(short events, int timeout,
             fd_interface::event_handler_t handler) override;

    bool reset(int fd) noexcept override;

    bool set_blocking(bool enable) noexcept override;

    void swap(shared_fd& fd) noexcept;

    std::size_t use_count() const noexcept;

private:
    // Give weak_fd access to the control block
    friend class weak_fd;

    void drop_reference();

    /** True if blocking behavior is enabled */
    bool m_blocking;

    /**
     * Data shared between owners of the file descriptor
     */
    fd::shared_internal* m_shared_info;
};

/**
 * @class weak_fd
 *
 * Provides access to a single file descriptor that is managed by a shared_fd
 * object, employing semantics similar to std::weak_ptr
 */
class weak_fd final {
public:
    weak_fd();

    explicit weak_fd(const shared_fd& fd);

    weak_fd(const weak_fd& fd);
    weak_fd(weak_fd&& fd);
    weak_fd& operator=(const weak_fd& fd);
    weak_fd& operator=(const shared_fd& fd);
    weak_fd& operator=(weak_fd&& fd);

    ~weak_fd();

    bool expired() const noexcept;

    shared_fd lock() const noexcept;

	void reset() noexcept;

    void swap(weak_fd& fd) noexcept;

    std::size_t use_count() const noexcept;

private:
    /**
     * Data shared between owners of the file descriptor
     */
    fd::shared_internal* m_shared_info;
};

}  // namespace jfern

#endif  // NETWORKING_SHARED_FD_H_
