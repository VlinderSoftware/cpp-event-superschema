#include "event_superschema/jws_event_dispatcher.hpp"

#include "event_superschema/event_dispatcher.hpp"

namespace Vlinder {
namespace EventSuperSchema {

EventDispatcher getJWSEventDispatcher(const ErrorHandler& err, const EventHandlers& handlers,
                                      const std::string& verification_key)
{
    // Get the base dispatcher
    auto base_dispatcher = getEventDispatcher(err, handlers);

    // Return a wrapper that verifies signature before dispatching
    return [base_dispatcher, err, verification_key](const json& signed_event)
    {
        // Placeholder implementation
        // In production, this would:
        // 1. Verify the event is a valid JWS
        // 2. Verify the signature using the verification_key
        // 3. Extract the payload
        // 4. Parse the payload as JSON
        // 5. Pass to base_dispatcher

        // For now, just pass through (assumes event signature is valid)
        // In real implementation, you would use a library like jwt-cpp
        (void)verification_key;  // Suppress unused warning
        base_dispatcher(signed_event);
    };
}

}  // namespace EventSuperSchema
}  // namespace Vlinder
