/**
 *  \file   unique_fd.cpp
 *  \author Jason Fernandez
 *  \date   10/30/2021
 *
 *  https://github.com/jfern2011/networking
 */

#include "unique_fd.h"

#include <utility>

#include "file_descriptor.h"

namespace jfern {
/**
 * Default constructor
 */
unique_fd::unique_fd() : m_blocking(false), m_fd(-1) {
}

/**
 * Constructor
 *
 * @param [in] fd A raw file descriptor
 */
unique_fd::unique_fd(int fd) : m_blocking(false), m_fd(fd) {
}

/**
 * Move constructor
 *
 * @param[in] fd The file descriptor to take ownership of
 */
unique_fd::unique_fd(unique_fd&& fd) {
	*this = std::move(fd);
}

/**
 * Destructor
 */
unique_fd::~unique_fd() {
	if (*this) file_descriptor::close(m_fd);
    m_fd = -1;
}

/**
 * Move assignment operator
 *
 * @note This will also copy the blocking behavior of \a fd
 *
 * @param [in] fd  The file descriptor to take ownership of
 *
 * @return *this
 */
unique_fd& unique_fd::operator=(unique_fd&& fd) {
	if (this != &fd) {
		m_blocking = fd.m_blocking;

        m_fd = fd.m_fd; fd.m_fd = -1;
	}

	return *this;
}

/**
 * @see See fd_interface::operator bool()
 */
unique_fd::operator bool() const noexcept {
	return 0 <= m_fd;
}

/**
 * @see See fd_interface::get()
 */
int unique_fd::get() const noexcept {
    return m_fd;
}

/**
 * @see See fd_interface::is_blocking()
 */
bool unique_fd::is_blocking() const noexcept {
    return m_blocking;
}

/**
 * @see See fd_interface::poll()
 */
int unique_fd::poll(short events) noexcept {
    return file_descriptor::poll(m_fd, events, 0);
}

/**
 * @see See fd_interface::poll()
 */
int unique_fd::poll(short events,
                    fd_interface::event_handler_t handler) {
    const int revents = poll(events);
    if (revents != -1 && handler) handler(revents, *this);

    return revents;
}

/**
 * @see See fd_interface::poll()
 */
int unique_fd::poll(short events, int timeout) noexcept {
    return file_descriptor::poll(m_fd, events, timeout);
}

/**
 * @see See fd_interface::poll()
 */
int unique_fd::poll(short events, int timeout,
                    fd_interface::event_handler_t handler) {
    const int revents = poll(events, timeout);
    if (revents != -1 && handler) handler(revents, *this);

    return revents;
}

/**
 * Release ownership of the file descriptor currently being managed. This will
 * not modify its blocking behavior
 *
 * @return The file descriptor we were managing
 */
int unique_fd::release() {
	const int fd = m_fd; m_fd = -1;

	return fd;
}

/**
 * @see See fd_interface::reset()
 */
bool unique_fd::reset(int fd) noexcept {
	if (*this) file_descriptor::close(m_fd);
	m_fd = fd;

    return file_descriptor::set_blocking(m_fd, m_blocking);
}

/**
 * @see See fd_interface::set_blocking()
 */
bool unique_fd::set_blocking(bool enable) noexcept {
    return file_descriptor::set_blocking(m_fd, enable);
}

/**
 * Swap internal file descriptors with another \ref unique_fd
 *
 * @param[in,out] fd The \ref unique_fd to swap with
 */
void unique_fd::swap(unique_fd& fd) {
	const int temp_fd = m_fd;
    const bool temp_blocking = m_blocking;

    m_blocking = fd.m_blocking;
	m_fd = fd.m_fd;

    fd.m_blocking = temp_blocking;
	fd.m_fd = temp_fd;
}

}  // namespace jfern
