#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace Vlinder {
namespace EventSuperschema {

using json = nlohmann::json;

// The super-schema definition
extern const json super_schema;

// Validate an event against the super-schema
bool validate_super_schema(const json& event);

// Helper function to validate UUID format
bool is_valid_uuid(const std::string& uuid_str);

}  // namespace EventSuperschema
}  // namespace Vlinder
