/**
 *  \file   fd_internal.h
 *  \author Jason Fernandez
 *  \date   11/21/2021
 *
 *  https://github.com/jfern2011/networking
 */

#ifndef NETWORKING_FD_INTERNAL_H_
#define NETWORKING_FD_INTERNAL_H_

#include <atomic>
#include <cstddef>
#include <mutex>

namespace jfern {
namespace fd {
/**
 * Maintains data shared between all owners of the file descriptor, including
 * owners in other threads
 *
 * @note We maintain strong and weak reference counters. The file descriptor is
 *       closed when the last strong reference is released. The weak reference
 *       count is the number of weak references to the file descriptor, plus
 *       one if the number of strong references is non-zero. When the number of
 *       weak references reaches zero, *this is destroyed. Because the weak
 *       reference count is always at least equal to the strong reference count,
 *       the destruction of *this is guaranteed not to happen before the file
 *       descriptor is closed
 */
class shared_internal final {
public:
    explicit shared_internal(int fd);

    std::size_t count() const;
    std::size_t weak_count() const;

    int get() const;

    void add_reference();
    bool add_reference_if_valid();
    bool release();

    void add_weak_reference();
    bool add_weak_reference_if_valid();
    bool release_weak_reference();

    /**
     * Used to prevent simultaneous POSIX calls on the same file descriptor
     */
    std::mutex m_mutex;

private:
    /** The reference count */
    std::atomic<std::size_t> m_count;

    /** The actual file descriptor */
    int m_fd;

    /** The weak reference count */
    std::atomic<std::size_t> m_weak_count;
};

}  // namespace fd
}  // namespace jfern

#endif  // NETWORKING_FD_INTERNAL_H_
