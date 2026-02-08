#pragma once

#include "types.hpp"

namespace event_superschema {

/**
 * Get an event dispatcher
 * 
 * @param err Error handler. Receives an error message that, if it comes from this module,
 *            will contain at least an 'error' and a 'message' field
 * @param handlers Handlers for the event. For each event type to handle, it should have a function
 *                 that takes the event as an argument. Only one handler per event is permitted.
 *                 Exceptions are not caught. If no specific handler is available and a '__default__'
 *                 handler is included in the handlers, that handler will be called by the dispatcher.
 * @return A dispatcher that will validate incoming events against the super-schema and call the
 *         appropriate event handler if one is available.
 */
EventDispatcher get_event_dispatcher(
    const ErrorHandler& err,
    const EventHandlers& handlers
);

} // namespace event_superschema
