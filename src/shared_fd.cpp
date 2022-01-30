/**
 *  \file   shared_fd.cpp
 *  \author Jason Fernandez
 *  \date   11/05/2021
 *
 *  https://github.com/jfern2011/networking
 */

#include "networking/shared_fd.h"

#include <utility>

#include "networking/file_descriptor.h"

namespace jfern {
/**
 * @brief Default constructor
 */
shared_fd::shared_fd()
    : m_blocking(false), m_shared_info(nullptr) {
}

/**
 * @brief Constructor
 *
 * @param fd The file descriptor to manage
 */
shared_fd::shared_fd(int fd)
    : m_blocking(false), m_shared_info(new fd::shared_internal(fd)) {
}

/**
 * @brief Copy constructor
 *
 * @param fd The shared_fd whose file descriptor will be co-owned with *this
 */
shared_fd::shared_fd(const shared_fd& fd) {
    *this = fd;
}

/**
 * @brief Move constructor
 *
 * @param fd The shared_fd whose file descriptor will be handed to *this
 */
shared_fd::shared_fd(shared_fd&& fd) {
    *this = std::move(fd);
}

/**
 * @brief Copy assignment operator
 *
 * @param fd The shared_fd whose file descriptor will be co-owned with *this
 *
 * @return *this
 */
shared_fd& shared_fd::operator=(const shared_fd& fd) {
    if (this != &fd) {
        m_blocking = fd.m_blocking;

        // Drop the currently referenced file descriptor
        drop_reference();
        m_shared_info = fd.m_shared_info;

        if (m_shared_info) {
            if (!m_shared_info->add_reference_if_valid()) {
                m_shared_info = nullptr;
            }
        }
    }

    return *this;
}

/**
 * @brief Move assignment operator
 *
 * @param fd The shared_fd whose file descriptor will be handed to *this
 *
 * @return *this
 */
shared_fd& shared_fd::operator=(shared_fd&& fd) {
    if (this != &fd) {
        m_blocking = fd.m_blocking;

        // Drop the currently referenced file descriptor
        drop_reference();
        m_shared_info = fd.m_shared_info;

        fd.m_shared_info = nullptr;
    }

    return *this;
}

/**
 * @brief Destructor. Closes the file descriptor if *this is its only owner
 */
shared_fd::~shared_fd() {
    drop_reference();
}

/**
 * @see See fd_interface::operator bool()
 */
shared_fd::operator bool() const noexcept {
    return m_shared_info;
}

/**
 * @see See fd_interface::get()
 */
int shared_fd::get() const noexcept {
    return m_shared_info ? m_shared_info->get() : -1;
}

/**
 * @see See fd_interface::is_blocking()
 */
bool shared_fd::is_blocking() const noexcept {
    return m_blocking;
}

/**
 * @see See fd_interface::poll()
 */
int shared_fd::poll(short events) noexcept {
    if (!m_shared_info) return -1;

    return file_descriptor::poll(
            m_shared_info->get(), events, 0, &m_shared_info->m_mutex);
}

/**
 * @see See fd_interface::poll()
 */
int shared_fd::poll(short events,
                    fd_interface::event_handler_t handler) {
    const int revents = poll(events);
    if (revents != -1 && handler) handler(revents, *this);

    return revents;
}

/**
 * @see See fd_interface::poll()
 */
int shared_fd::poll(short events, int timeout) noexcept {
    if (!m_shared_info) return -1;

    return file_descriptor::poll(
            m_shared_info->get(), events, timeout, &m_shared_info->m_mutex);
}

/**
 * @see See fd_interface::poll()
 */
int shared_fd::poll(short events, int timeout,
                    fd_interface::event_handler_t handler) {
    const int revents = poll(events, timeout);
    if (revents != -1 && handler) handler(revents, *this);

    return revents;
}

/**
 * @see See fd_interface::reset()
 */
bool shared_fd::reset(int fd) noexcept {
    if (!file_descriptor::set_blocking(fd, m_blocking)) {
        return false;
    }

    drop_reference();

    m_shared_info = new fd::shared_internal(fd);
    return m_shared_info;
}

/**
 * @see See fd_interface::set_blocking()
 */
bool shared_fd::set_blocking(bool enable) noexcept {
    if (m_shared_info) {
        return file_descriptor::set_blocking(
                m_shared_info->get(), enable, &m_shared_info->m_mutex);
    }

    return false;
}

/**
 * @brief Swap this object's data members with \a fd
 * 
 * @param fd The shared_fd to swap with
 */
void shared_fd::swap(shared_fd& fd) noexcept {
    if (this != &fd) {
        const bool my_blocking = m_blocking;
        fd::shared_internal* my_shared_info = m_shared_info;

        m_blocking = fd.m_blocking;
        m_shared_info = fd.m_shared_info;

        fd.m_shared_info = my_shared_info;
        fd.m_blocking = my_blocking;
    }
}

/**
 * @brief Get the number of objects sharing ownership of the file descriptor
 *
 * @return The number of owners
 */
std::size_t shared_fd::use_count() const noexcept {
    if (!m_shared_info) return 0;

    return m_shared_info->count();
}

/**
 * @brief Release ownership of the currently held file descriptor
 */
void shared_fd::drop_reference() {
    if (m_shared_info) {
        if (m_shared_info->release()) {
            delete m_shared_info;
        }

        m_shared_info = nullptr;
    }
}

/**
 * @brief Default constructor
 */
weak_fd::weak_fd() : m_shared_info(nullptr) {
}

/**
 * @brief Constructor
 *
 * @param fd The shared_fd which manages the file descriptor to obtain a weak
 *           reference to
 */
weak_fd::weak_fd(const shared_fd& fd)
    : m_shared_info(fd.m_shared_info) {
}

/**
 * @brief Copy constructor
 *
 * @param fd The weak_fd to create a copy of
 */
weak_fd::weak_fd(const weak_fd& fd) {
    *this = fd;
}

/**
 * @brief Move constructor
 *
 * @param fd The weak_fd which will be moved into *this
 */
weak_fd::weak_fd(weak_fd&& fd) {
    *this = std::move(fd);
}

/**
 * @brief Copy assignment operator
 *
 * @param fd The weak_fd to copy *this to
 *
 * @return *this
 */
weak_fd& weak_fd::operator=(const weak_fd& fd) {
    if (this != &fd) {
        // Drop the currently referenced file descriptor
        reset();
        m_shared_info = fd.m_shared_info;

        if (m_shared_info) {
            if (!m_shared_info->add_weak_reference_if_valid()) {
                m_shared_info = nullptr;
            }
        }
    }

    return *this;
}

/**
 * @brief Move assignment operator
 *
 * @param fd The weak_fd whose reference will be transferred to *this
 *
 * @return *this
 */
weak_fd& weak_fd::operator=(weak_fd&& fd) {
    if (this != &fd) {
        // Drop the currently referenced file descriptor
        reset();
        m_shared_info = fd.m_shared_info;

        fd.m_shared_info = nullptr;
    }

    return *this;
}

/**
 * @brief Assign *this to reference a new file descriptor
 *
 * @param fd The object currently managing the target file descriptor
 *
 * @return *this
 */
weak_fd& weak_fd::operator=(const shared_fd& fd) {
    m_shared_info = fd.m_shared_info;
    return *this;
}

/**
 * @brief Destructor. Drops the current reference and deletes the control block
 *        if this is the last weak reference and there are no shared_fds still
 *        managing the file descriptor
 */
weak_fd::~weak_fd() {
    reset();
}

/**
 * @brief Check if *this references a valid file descriptor
 *
 * @return True if a file descriptor is currently being referenced and is valid
 *         (has not been closed)
 */
bool weak_fd::expired() const noexcept {
    return m_shared_info && m_shared_info->count() == 0u;
}

/**
 * @brief Create a new shared_fd which shares ownership of the file descriptor
 *        referenced by *this
 * 
 * @return A new shared_fd which manages the referenced file descriptor, or an
 *         empty shared_fd if the reference is invalid
 */
shared_fd weak_fd::lock() const noexcept {
    if (m_shared_info) {
        if (m_shared_info->add_reference_if_valid()) {
            return shared_fd(m_shared_info->get());
        }
    }

    // Return an empty object
    return shared_fd();
}

/**
 * @brief Drops the reference to the currently held file descriptor if a
 *        reference is currently being held
 */
void weak_fd::reset() noexcept {
    if (m_shared_info) {
        if (m_shared_info->release_weak_reference()) {
            delete m_shared_info;
        }

        m_shared_info = nullptr;
    }
}

/**
 * @brief Swap this object's data members with \a fd
 * 
 * @param fd The weak_fd to swap with
 */
void weak_fd::swap(weak_fd& fd) noexcept {
    if (this != &fd) {
        fd::shared_internal* my_shared_info = m_shared_info;

        m_shared_info = fd.m_shared_info;
        fd.m_shared_info = my_shared_info;
    }
}

/**
 * @brief Returns the number of shared_fds managing the referenced file
 *        descriptor
 * 
 * @return The number of file descriptor owners, or zero if *this does not
 *         reference a valid file descriptor
 */
std::size_t weak_fd::use_count() const noexcept {
    if (m_shared_info) return m_shared_info->count();
    return 0;
}

}  // namespace jfern
