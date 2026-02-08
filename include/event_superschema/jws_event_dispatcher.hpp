#pragma once

#include "types.hpp"

namespace Vlinder {
namespace EventSuperSchema {

/// Get a JWS (JSON Web Signature) event dispatcher
///
/// This is a simplified placeholder implementation. In production, you would integrate
/// with a proper JWS library (e.g., jwt-cpp, jose-cpp) for full signature verification.
///
/// @param err Error handler
/// @param handlers Event handlers
/// @param verification_key Key used to verify JWS signatures (placeholder)
/// @return A dispatcher that verifies JWS signatures before dispatching
EventDispatcher getJWSEventDispatcher(const ErrorHandler& err, const EventHandlers& handlers,
                                      const std::string& verification_key);

}  // namespace EventSuperSchema
}  // namespace Vlinder
