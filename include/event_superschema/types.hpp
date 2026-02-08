#pragma once

#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <string>

namespace Vlinder {
namespace EventSuperschema {

using json = nlohmann::json;

// Error message structure
struct ErrorMessage
{
    std::string error;
    std::string message;

    json toJSON() const
    {
        return {{"error", error}, {"message", message}};
    }
};

// Type aliases for handlers and functions
using ErrorHandler = std::function<void(const ErrorMessage&)>;
using EventHandler = std::function<void(const ErrorHandler&, const json&)>;
using EventHandlers = std::map<std::string, EventHandler>;
using SendFunction = std::function<void(const json&)>;
using DataPreprocessor = std::function<json(const json&)>;
using DataPreprocessors = std::map<std::string, DataPreprocessor>;
using EventDispatcher = std::function<void(const json&)>;
using SendEventFunction =
    std::function<void(const std::string& event_type, const json& event_data,
                       const std::string& cid, const std::string& uid, const std::string& token)>;

}  // namespace EventSuperschema
}  // namespace Vlinder
