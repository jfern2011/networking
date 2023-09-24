/**
 *  \file   fd_interface.h
 *  \author Jason Fernandez
 *  \date   10/30/2021
 *
 *  https://github.com/jfern2011/networking
 */

#ifndef NETWORKING_FD_INTERFACE_H_
#define NETWORKING_FD_INTERFACE_H_

#include <functional>

namespace jfern {
/**
 * Common file descriptor abstract interface
 */
class fd_interface {
public:
    /**
     * Callback invoked when \ref poll() returns valid events
     */
    using event_handler_t = std::function<void(short revents, fd_interface&)>;

    virtual ~fd_interface() = default;

    /**
     * Boolean type conversion operator
     * 
     * @return True if this object holds a valid file descriptor
     */
    virtual explicit operator bool() const noexcept = 0;

    /**
     * Get the value of the underlying file descriptor
     * 
     * @return The file descriptor, or -1 if not assigned
     */
    virtual int get() const noexcept = 0;

    /**
     * Check if blocking behavior is enabled on this file descriptor
     * 
     * @return True if this file descriptor is blocking
     */
    virtual bool is_blocking() const noexcept = 0;

    /**
     * Poll for file descriptor events
     *
     * @details
     * Based on https://www.man7.org/linux/man-pages/man2/poll.2.html
     *
     * @param[in] events A mask of events to poll for
     *
     * @return The mask of returned events, or -1 on error
     */
    virtual int poll(short events) noexcept = 0;

    /**
     * Poll for file descriptor events
     *
     * @details
     * Based on https://www.man7.org/linux/man-pages/man2/poll.2.html
     *
     * @param[in] events  A mask of events to poll for
     * @param[in] handler Callable which receives this object along with a mask
     *                    of returned events
     *
     * @return The mask of returned events, or -1 on error
     */
    virtual int poll(short events, event_handler_t handler) = 0;

    /**
     * Poll for file descriptor events
     *
     * @details
     * Based on https://www.man7.org/linux/man-pages/man2/poll.2.html
     *
     * @param[in] events  A mask of events to poll for
     * @param[in] timeout Wait at most this many milliseconds for events
     *
     * @return The mask of returned events, or -1 on error
     */
    virtual int poll(short events, int timeout) noexcept = 0;

    /**
     * Poll for file descriptor events
     *
     * @details
     * Based on https://www.man7.org/linux/man-pages/man2/poll.2.html
     *
     * @param[in] events  A mask of events to poll for
     * @param[in] timeout Wait at most this many milliseconds for events
     * @param[in] handler Callable which receives this object along with a mask
     *                    of returned events
     *
     * @return The mask of returned events, or -1 on error
     */
    virtual int poll(short events, int timeout, event_handler_t handler) = 0;

    /**
     * Assign a new file descriptor to this object
     *
     * @note Blocking behavior is carried to the new file descriptor
     *
     * @param[in] fd The new file descriptor to be maintained by this object
     *
     * @return True on success
     */
    virtual bool reset(int fd) noexcept = 0;

    /**
     * Enable blocking behavior on this file descriptor
     *
     * @param[in] enable True to enable blocking, false to disable
     * 
     * @return True on success. Otherwise, errno will be set
     */
    virtual bool set_blocking(bool enable) noexcept = 0;
};

}  // namespace jfern

#endif  // NETWORKING_FD_INTERFACE_H_
