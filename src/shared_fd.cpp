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
            m_shared_info->add_reference();
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

}  // namespace jfern
