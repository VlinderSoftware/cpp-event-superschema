#pragma once

#include "types.hpp"

namespace Vlinder {
namespace EventSuperschema {

/// Get a JWE (JSON Web Encryption) event dispatcher
///
/// This is a simplified placeholder implementation. In production, you would integrate
/// with a proper JWE library (e.g., jwt-cpp, jose-cpp) for full encryption support.
///
/// @param err Error handler
/// @param handlers Event handlers
/// @param decryption_key Key used to decrypt JWE events (placeholder)
/// @return A dispatcher that decrypts JWE events before dispatching
EventDispatcher get_jwe_event_dispatcher(const ErrorHandler& err, const EventHandlers& handlers,
                                         const std::string& decryption_key);

}  // namespace EventSuperschema
}  // namespace Vlinder
