#include "event_superschema/jwe_send_event_function.hpp"

#include "event_superschema/send_event_function.hpp"

namespace Vlinder {
namespace EventSuperSchema {

SendEventFunction getJWESendEventFunction(const SendFunction& send, const std::string& pid,
                                          const std::string& encryption_key,
                                          const DataPreprocessors& data_preprocessors)
{
    // Get the base send function
    auto base_send = getSendEventFunction(send, pid, data_preprocessors);

    // Placeholder: In production, create a wrapper that encrypts
    // For now, just use the base send function
    (void)encryption_key;  // Suppress unused warning

    // In real implementation, you would:
    // 1. Use base_send to format the event
    // 2. Encrypt the formatted event using encryption_key
    // 3. Send the encrypted JWE token

    return base_send;
}

}  // namespace EventSuperSchema
}  // namespace Vlinder
