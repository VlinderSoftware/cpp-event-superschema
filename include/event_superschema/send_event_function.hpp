#pragma once

#include "types.hpp"
#include <string>
#include <optional>

namespace event_superschema {

/**
 * Get a function to send properly formatted events
 * 
 * @param send A generic function to send events on the event bus, once they're properly
 *             formatted. Should expect a json object and not return anything.
 * @param pid Producer ID (UUID string)
 * @param data_preprocessors Optional map of event types to their data preprocessors.
 *                           The data preprocessor should convert the event data to a serializable
 *                           json object conforming to the appropriate schema
 * @return A function to send events with proper formatting
 */
SendEventFunction get_send_event_function(
    const SendFunction& send,
    const std::string& pid,
    const DataPreprocessors& data_preprocessors = {}
);

// Helper function to generate UUID v4
std::string generate_uuid();

} // namespace event_superschema
