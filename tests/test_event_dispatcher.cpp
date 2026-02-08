#include <catch2/catch_test_macros.hpp>

#include "event_superschema/event_dispatcher.hpp"

using namespace Vlinder::EventSuperschema;

TEST_CASE("Event dispatcher", "[dispatcher]")
{
    SECTION("Dispatches valid event to correct handler")
    {
        bool error_called = false;
        bool handler_called = false;
        json received_event;

        auto err = [&error_called](const ErrorMessage& msg)
        {
            error_called = true;
            (void)msg;
        };

        EventHandlers handlers;
        handlers["test.event"] =
            [&handler_called, &received_event](const ErrorHandler& err, const json& event)
        {
            handler_called = true;
            received_event = event;
            (void)err;
        };

        auto dispatcher = getEventDispatcher(err, handlers);

        json valid_event = {{"id", "550e8400-e29b-41d4-a716-446655440000"},
                            {"type", "test.event"},
                            {"metadata",
                             {{"cid", "550e8400-e29b-41d4-a716-446655440001"},
                              {"pid", "550e8400-e29b-41d4-a716-446655440002"}}}};

        dispatcher(valid_event);

        REQUIRE(handler_called);
        REQUIRE_FALSE(error_called);
        REQUIRE(received_event == valid_event);
    }

    SECTION("Calls error handler for invalid event")
    {
        bool error_called = false;
        bool handler_called = false;
        ErrorMessage error_msg;

        auto err = [&error_called, &error_msg](const ErrorMessage& msg)
        {
            error_called = true;
            error_msg = msg;
        };

        EventHandlers handlers;
        handlers["test.event"] = [&handler_called](const ErrorHandler& err, const json& event)
        {
            handler_called = true;
            (void)err;
            (void)event;
        };

        auto dispatcher = getEventDispatcher(err, handlers);

        json invalid_event = {{"type", "test.event"},
                              {"metadata",
                               {{"cid", "550e8400-e29b-41d4-a716-446655440001"},
                                {"pid", "550e8400-e29b-41d4-a716-446655440002"}}}};

        dispatcher(invalid_event);

        REQUIRE(error_called);
        REQUIRE_FALSE(handler_called);
        REQUIRE(error_msg.error == "SchemaMismatchError");
        REQUIRE(error_msg.message == "Event does not match event schema");
    }

    SECTION("Dispatches to base event handler when specific handler not found")
    {
        bool handler_called = false;
        json received_event;

        auto err = [](const ErrorMessage& msg)
        {
            (void)msg;
        };

        EventHandlers handlers;
        handlers["test.event"] =
            [&handler_called, &received_event](const ErrorHandler& err, const json& event)
        {
            handler_called = true;
            received_event = event;
            (void)err;
        };

        auto dispatcher = getEventDispatcher(err, handlers);

        json versioned_event = {{"id", "550e8400-e29b-41d4-a716-446655440000"},
                                {"type", "test.event:v1"},
                                {"metadata",
                                 {{"cid", "550e8400-e29b-41d4-a716-446655440001"},
                                  {"pid", "550e8400-e29b-41d4-a716-446655440002"}}}};

        dispatcher(versioned_event);

        REQUIRE(handler_called);
        REQUIRE(received_event["type"] == "test.event:v1");
    }

    SECTION("Dispatches to default handler when no specific handler found")
    {
        bool default_handler_called = false;
        json received_event;

        auto err = [](const ErrorMessage& msg)
        {
            (void)msg;
        };

        EventHandlers handlers;
        handlers["__default__"] =
            [&default_handler_called, &received_event](const ErrorHandler& err, const json& event)
        {
            default_handler_called = true;
            received_event = event;
            (void)err;
        };

        auto dispatcher = getEventDispatcher(err, handlers);

        json event = {{"id", "550e8400-e29b-41d4-a716-446655440000"},
                      {"type", "unknown.event"},
                      {"metadata",
                       {{"cid", "550e8400-e29b-41d4-a716-446655440001"},
                        {"pid", "550e8400-e29b-41d4-a716-446655440002"}}}};

        dispatcher(event);

        REQUIRE(default_handler_called);
        REQUIRE(received_event == event);
    }

    SECTION("Does nothing when no handler found and no default")
    {
        bool any_handler_called = false;

        auto err = [](const ErrorMessage& msg)
        {
            (void)msg;
        };

        EventHandlers handlers;
        handlers["other.event"] = [&any_handler_called](const ErrorHandler& err, const json& event)
        {
            any_handler_called = true;
            (void)err;
            (void)event;
        };

        auto dispatcher = getEventDispatcher(err, handlers);

        json event = {{"id", "550e8400-e29b-41d4-a716-446655440000"},
                      {"type", "unknown.event"},
                      {"metadata",
                       {{"cid", "550e8400-e29b-41d4-a716-446655440001"},
                        {"pid", "550e8400-e29b-41d4-a716-446655440002"}}}};

        dispatcher(event);

        REQUIRE_FALSE(any_handler_called);
    }
}
