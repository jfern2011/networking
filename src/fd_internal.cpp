/**
 *  \file   fd_internal.cpp
 *  \author Jason Fernandez
 *  \date   11/21/2021
 *
 *  https://github.com/jfern2011/networking
 */

#include "networking/fd_internal.h"

#include "networking/file_descriptor.h"

namespace jfern {
namespace fd {
/**
 * @brief Constructor
 *
 * @param[in] fd The file descriptor to manage
 */
shared_internal::shared_internal(int fd)
    : m_mutex(), m_count(1), m_fd(fd), m_weak_count(1) {
}

/**
 * @brief Get the current reference count
 *
 * @return The reference count
 */
std::size_t shared_internal::count() const {
    return m_count.load(std::memory_order_relaxed);
}

/**
 * @brief Get the current weak reference count
 *
 * @return The weak reference count
 */
std::size_t shared_internal::weak_count() const {
    return m_weak_count.load(std::memory_order_relaxed);
}

/**
 * @brief Get the internally held file descriptor
 *
 * @return The file descriptor
 */
int shared_internal::get() const {
    return m_fd;
}

/**
 * @brief Increment the reference count
 */
void shared_internal::add_reference() {
    m_count.fetch_add(1, std::memory_order_relaxed);
}

/**
 * @brief Increment the reference count only if the use count
 *        is non-zero
 *
 * @return True if the increment succeeded
 */
bool shared_internal::add_reference_if_valid() {
    std::size_t count
        = m_count.load(std::memory_order_relaxed);

    for (;;) {
        if (count == 0) return false;
        if (m_count.compare_exchange_weak(
                count, count+1, std::memory_order_relaxed)) {
            return true;
        }
    }
}

/**
 * @brief Decrement the reference count
 *
 * @return True if all references have been dropped
 */
bool shared_internal::release() {
    if (m_count.fetch_sub(1, std::memory_order_relaxed) == 1) {
        file_descriptor::close(m_fd);

        // Decrement the weak reference count since we held
        // the last strong reference
        return release_weak_reference();
    }
    return false;
}

/**
 * @brief Increment the weak reference count
 */
void shared_internal::add_weak_reference() {
    m_weak_count.fetch_add(1, std::memory_order_relaxed);
}
/**
 * @brief Increment the weak reference count only if its count
 *        is non-zero
 *
 * @return True if the increment succeeded
 */
bool shared_internal::add_weak_reference_if_valid() {
    std::size_t count
        = m_weak_count.load(std::memory_order_relaxed);

    for (;;) {
        if (count == 0) return false;
        if (m_weak_count.compare_exchange_weak(
                count, count+1, std::memory_order_relaxed)) {
            return true;
        }
    }
}

/**
 * @brief Decrement the weak reference count
 *
 * @return True if all references have been dropped
 */
bool shared_internal::release_weak_reference() {
    return m_weak_count.fetch_sub(1, std::memory_order_relaxed)
            == 1;
}

}  // namespace fd
}  // namespace jfern
