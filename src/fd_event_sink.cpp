/**
 *  \file   fd_event_sink.cpp
 *  \author Jason Fernandez
 *  \date   09/24/2023
 *
 *  https://github.com/jfern2011/networking
 */

#include "networking/fd_event_sink.h"

#include <algorithm>
#include <utility>

namespace jfern {
/**
 * @brief Constructor
 *
 * @param fd The file descriptor to acquire ownership of
 */
fd_event_sink::fd_event_sink(std::unique_ptr<fd_interface> fd)
    : m_events(), m_fd(std::move(fd)) {
}

/**
 * @brief Add events of interest for this file descriptor
 *
 * @param events  Bitmask of events to handle
 * @param handler Called back in reponse to any events specified in \a events
 *
 * @note If any event in \a events already has an associated handler, the
 *       handler for that event will be replaced by \a handler
 *
 * @return True on success
 */
bool fd_event_sink::add_events(short events,
                               fd_interface::event_handler_t handler) {
    if (events == 0 || !handler)
        return false;

    for (auto& event : m_events) {
        const short common_events = event.mask & events;

        event.mask ^= common_events;
    }

    erase_null_events();

    m_events.emplace_back(events, handler);

    return true;
}

/**
 * @brief Clear all events/event handlers for this file descriptor
 */
void fd_event_sink::clear_events() {
    m_events.clear();
}

/**
 * @brief Handle file desciptor events
 *
 * @param events Bitmask specifying which events occurred
 */
void fd_event_sink::handle_events(short events) {
    std::for_each(m_events.begin(), m_events.end(),
        [&](const callback_info& event) {
            if (event.mask & events) event.handler(events, *m_fd);
        });
}

/**
 * @brief Remove previously added events
 *
 * @param events Bitmask specifying which events to no longer check for
 *
 * @note No-op if no events in \a events were previously added 
 */
void fd_event_sink::remove_events(short events) {
    for (auto& event : m_events) {
        const short to_remove = event.mask & events;

        event.mask ^= to_remove;
    }

    erase_null_events();
}

/**
 * @brief Clean up events whose bitmask is zero
 */
void fd_event_sink::erase_null_events() {
    for (auto iter = m_events.begin(), end = m_events.end(); iter != end; ) {
        iter = iter->mask == 0 ? m_events.erase(iter) : iter + 1;
    }
}

}  // namespace jfern
