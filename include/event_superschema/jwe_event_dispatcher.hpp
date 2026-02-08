#pragma once

#include <vector>

#include "types.hpp"

namespace Vlinder {
namespace EventSuperSchema {

/// Get a JWE (JSON Web Encryption) event dispatcher
///
/// This is a simplified placeholder implementation. In production, you would integrate
/// with a proper JWE library (e.g., jwt-cpp, jose-cpp) for full encryption support.
///
/// @param err Error handler
/// @param handlers Event handlers
/// @param decryption_key Key used to decrypt JWE events (placeholder)
/// @return A dispatcher that decrypts JWE events before dispatching
EventDispatcher getJWEEventDispatcher(const ErrorHandler& err, const EventHandlers& handlers,
                                      const std::vector<uint8_t>& decryption_key);

}  // namespace EventSuperSchema
}  // namespace Vlinder
