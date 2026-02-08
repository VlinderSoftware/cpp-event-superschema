#pragma once

#include "types.hpp"

namespace Vlinder {
namespace EventSuperSchema {

/// Get a function to send JWE encrypted events
///
/// This is a simplified placeholder implementation. In production, you would integrate
/// with a proper JWE library for full encryption support.
///
/// @param send Send function
/// @param pid Producer ID
/// @param encryption_key Key used to encrypt events (placeholder)
/// @param data_preprocessors Optional data preprocessors
/// @return A function that encrypts events before sending
SendEventFunction getJWESendEventFunction(const SendFunction& send, const std::string& pid,
                                          const std::string& encryption_key,
                                          const DataPreprocessors& data_preprocessors = {});

}  // namespace EventSuperSchema
}  // namespace Vlinder
