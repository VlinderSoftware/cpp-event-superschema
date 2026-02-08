#include "event_superschema/event_dispatcher.hpp"

#include <algorithm>

#include "event_superschema/super_schema.hpp"

namespace Vlinder {
namespace EventSuperSchema {

EventDispatcher getEventDispatcher(const ErrorHandler& err, const EventHandlers& handlers)
{
    return [err, handlers](const json& event)
    {
        // Validate against super-schema
        if (!validate(event))
        {
            err({"SchemaMismatchError", "Event does not match event schema"});
            return;
        }

        // Get event type
        std::string event_type = event["type"].get<std::string>();

        // Try to find exact match
        auto it = handlers.find(event_type);
        if (it != handlers.end())
        {
            it->second(err, event);
            return;
        }

        // Try to find base event name (strip version suffix after last ':')
        auto colon_pos = event_type.rfind(':');
        if (colon_pos != std::string::npos)
        {
            std::string base_event_name = event_type.substr(0, colon_pos);
            auto base_it = handlers.find(base_event_name);
            if (base_it != handlers.end())
            {
                base_it->second(err, event);
                return;
            }
        }

        // Try default handler
        auto default_it = handlers.find("__default__");
        if (default_it != handlers.end())
        {
            default_it->second(err, event);
        }
    };
}

}  // namespace EventSuperSchema
}  // namespace Vlinder
