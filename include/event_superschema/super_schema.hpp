#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace Vlinder {
namespace EventSuperSchema {

using json = nlohmann::json;

// The super-schema definition
extern const json super_schema;

// Validate an event against the super-schema
bool validate(const json& event);

// Helper function to validate UUID format
bool validateUUID(const std::string& uuid_str);

}  // namespace EventSuperSchema
}  // namespace Vlinder
