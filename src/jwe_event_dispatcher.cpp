#include "event_superschema/jwe_event_dispatcher.hpp"

#include "event_superschema/event_dispatcher.hpp"

namespace Vlinder {
namespace EventSuperSchema {

EventDispatcher getJWEEventDispatcher(const ErrorHandler& err, const EventHandlers& handlers,
                                      const std::vector<uint8_t>& decryption_key)
{
    // Get the base dispatcher
    auto base_dispatcher = getEventDispatcher(err, handlers);

    // Return a wrapper that decrypts before dispatching
    return [base_dispatcher, err, decryption_key](const json& encrypted_event)
    {
        //TODO
        // Placeholder implementation
        // In production, this would:
        // 1. Verify the event is a valid JWE
        // 2. Decrypt using the decryption_key
        // 3. Parse the decrypted payload as JSON
        // 4. Pass to base_dispatcher

        // For now, just pass through (assumes event is already decrypted)
        // In real implementation, you would use a library like jwt-cpp
        (void)decryption_key;  // Suppress unused warning
        base_dispatcher(encrypted_event);
    };
}

}  // namespace EventSuperSchema
}  // namespace Vlinder
