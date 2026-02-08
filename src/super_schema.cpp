#include "event_superschema/super_schema.hpp"

#include <regex>

namespace Vlinder {
namespace EventSuperschema {

const json super_schema = {{"type", "object"},
                           {"properties",
                            {{"id", {{"type", "string"}, {"format", "uuid"}}},
                             {"type", {{"type", "string"}}},
                             {"metadata",
                              {{"type", "object"},
                               {"properties",
                                {{"cid", {{"type", "string"}, {"format", "uuid"}}},
                                 {"tid", {{"type", "string"}, {"format", "uuid"}}},
                                 {"pid", {{"type", "string"}, {"format", "uuid"}}},
                                 {"uid", {{"type", "string"}, {"format", "uuid"}}},
                                 {"token", {{"type", "string"}}}}},
                               {"required", {"cid", "pid"}}}},
                             {"data", {{"type", "object"}}}}},
                           {"required", {"id", "type", "metadata"}}};

bool isValidUuid(const std::string& uuid_str)
{
    // UUID v4 regex pattern
    static const std::regex uuid_pattern(
        "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$");
    return std::regex_match(uuid_str, uuid_pattern);
}

bool validateSuperSchema(const json& event)
{
    // Check if event is an object
    if (!event.is_object())
    {
        return false;
    }

    // Check required fields
    if (!event.contains("id") || !event.contains("type") || !event.contains("metadata"))
    {
        return false;
    }

    // Validate id is a string and UUID format
    if (!event["id"].is_string() || !isValidUuid(event["id"].get<std::string>()))
    {
        return false;
    }

    // Validate type is a string
    if (!event["type"].is_string())
    {
        return false;
    }

    // Validate metadata
    const auto& metadata = event["metadata"];
    if (!metadata.is_object())
    {
        return false;
    }

    // Check required metadata fields
    if (!metadata.contains("cid") || !metadata.contains("pid"))
    {
        return false;
    }

    // Validate cid and pid are UUIDs
    if (!metadata["cid"].is_string() || !isValidUuid(metadata["cid"].get<std::string>()))
    {
        return false;
    }
    if (!metadata["pid"].is_string() || !isValidUuid(metadata["pid"].get<std::string>()))
    {
        return false;
    }

    // Validate optional UUID fields if present
    if (metadata.contains("tid"))
    {
        if (!metadata["tid"].is_string() || !isValidUuid(metadata["tid"].get<std::string>()))
        {
            return false;
        }
    }
    if (metadata.contains("uid"))
    {
        if (!metadata["uid"].is_string() || !isValidUuid(metadata["uid"].get<std::string>()))
        {
            return false;
        }
    }

    // Validate token is a string if present
    if (metadata.contains("token") && !metadata["token"].is_string())
    {
        return false;
    }

    // Validate data is an object if present
    if (event.contains("data") && !event["data"].is_object())
    {
        return false;
    }

    return true;
}

}  // namespace EventSuperschema
}  // namespace Vlinder
