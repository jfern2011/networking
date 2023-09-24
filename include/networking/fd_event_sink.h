/**
 *  \file   fd_event_sink.h
 *  \author Jason Fernandez
 *  \date   09/24/2023
 *
 *  https://github.com/jfern2011/networking
 */

#ifndef NETWORKING_FD_EVENT_SINK_H_
#define NETWORKING_FD_EVENT_SINK_H_

#include <memory>
#include <vector>

#include "fd_interface.h"

namespace jfern {
/**
 * @class fd_event_sink
 *
 * Dispatches handlers that respond to file descriptor events
 */
class fd_event_sink final {
public:
    explicit fd_event_sink(std::unique_ptr<fd_interface> fd);

    fd_event_sink(const fd_event_sink& sink)            = default;
    fd_event_sink(fd_event_sink&& sink)                 = default;
    fd_event_sink& operator=(const fd_event_sink& sink) = default;
    fd_event_sink& operator=(fd_event_sink&& sink)      = default;

    ~fd_event_sink() = default;

    bool add_events(short events, fd_interface::event_handler_t handler);

    void clear_events();

    void handle_events(short events);

private:
    /**
     * @brief Mapping from event(s) to event handler
     */
    struct callback_info {
        callback_info(short mask_,
                      fd_interface::event_handler_t handler_)
            : handler(handler_), mask(mask_) {}

        /**
         * Called back in reponse to one or more of the events specified
         * in the event mask
         */
        fd_interface::event_handler_t handler;

        /**
         * Bitmask of events that will trigger the handler
         */
        short mask;
    };

    /**
     * Events of interest on the file descriptor and their handlers
     */
    std::vector<callback_info> m_events;

    /**
     * The file descriptor itself (owned by *this)
     */
    std::shared_ptr<fd_interface>
        m_fd;
};

}  // namespace jfern

#endif  // NETWORKING_FD_EVENT_SINK_H_
