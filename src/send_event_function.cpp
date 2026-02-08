#include "event_superschema/send_event_function.hpp"

#include <iomanip>
#include <random>
#include <sstream>

namespace Vlinder {
namespace EventSuperSchema {

//TODO move to its own utils file if it grows or if we need more UUID-related functions in the future
std::string generateUUID()
{
    // Simple UUID v4 generator
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < 8; i++)
    {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 4; i++)
    {
        ss << dis(gen);
    }
    ss << "-4";  // UUID version 4
    for (int i = 0; i < 3; i++)
    {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);  // Variant bits
    for (int i = 0; i < 3; i++)
    {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 12; i++)
    {
        ss << dis(gen);
    }

    return ss.str();
}

SendEventFunction getSendEventFunction(const SendFunction& send, const std::string& pid,
                                       const DataPreprocessors& data_preprocessors)
{
    // Create preprocessors with default if needed
    DataPreprocessors preprocessors = data_preprocessors;
    if (preprocessors.find("__default__") == preprocessors.end())
    {
        preprocessors["__default__"] = [](const json& data)
        {
            return data;
        };
    }

    return [send, pid, preprocessors](const std::string& event_type,
                                      const json& event_data = json{}, const std::string& cid = "",
                                      const std::string& uid = "", const std::string& token = "")
    {
        // Format event data
        json formatted_data = json{};
        if (!event_data.is_null())
        {
            auto preprocessor_it = preprocessors.find(event_type);
            if (preprocessor_it != preprocessors.end())
            {
                formatted_data = preprocessor_it->second(event_data);
            }
            else
            {
                formatted_data = preprocessors.at("__default__")(event_data);
            }
        }

        // Generate event ID
        std::string event_id = generateUUID();

        // Build metadata
        json metadata = {{"cid", cid.empty() ? event_id : cid}, {"tid", event_id}, {"pid", pid}};

        if (!uid.empty())
        {
            metadata["uid"] = uid;
        }
        if (!token.empty())
        {
            metadata["token"] = token;
        }

        // Build formatted event
        json formatted_event = {{"id", event_id}, {"type", event_type}, {"metadata", metadata}};

        if (!formatted_data.is_null())
        {
            formatted_event["data"] = formatted_data;
        }

        // Send the event
        send(formatted_event);
    };
}

}  // namespace EventSuperSchema
}  // namespace Vlinder
