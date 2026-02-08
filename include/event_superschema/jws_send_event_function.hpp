#pragma once

#include "types.hpp"

namespace event_superschema
{

/// Get a function to send JWS signed events
///
/// This is a simplified placeholder implementation. In production, you would integrate
/// with a proper JWS library for full signature support.
///
/// @param send Send function
/// @param pid Producer ID
/// @param signing_key Key used to sign events (placeholder)
/// @param data_preprocessors Optional data preprocessors
/// @return A function that signs events before sending
SendEventFunction get_jws_send_event_function(const SendFunction& send, const std::string& pid,
                                              const std::string& signing_key,
                                              const DataPreprocessors& data_preprocessors = {});

}  // namespace event_superschema
