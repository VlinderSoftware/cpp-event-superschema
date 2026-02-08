#include <catch2/catch_test_macros.hpp>

#include "event_superschema/send_event_function.hpp"
#include "event_superschema/super_schema.hpp"

using namespace Vlinder::EventSuperschema;

TEST_CASE("Send event function", "[send_event]")
{
    SECTION("Sends minimal event")
    {
        json sent_event;
        auto send = [&sent_event](const json& event)
        {
            sent_event = event;
        };

        std::string pid = "550e8400-e29b-41d4-a716-446655440010";
        auto send_event = get_send_event_function(send, pid);

        send_event("test.event", json{}, "", "", "");

        REQUIRE(sent_event.contains("id"));
        REQUIRE(sent_event.contains("type"));
        REQUIRE(sent_event.contains("metadata"));
        REQUIRE(sent_event["type"] == "test.event");
        REQUIRE(sent_event["metadata"]["pid"] == pid);
        REQUIRE(validate_super_schema(sent_event));
    }

    SECTION("Sends event with data")
    {
        json sent_event;
        auto send = [&sent_event](const json& event)
        {
            sent_event = event;
        };

        std::string pid = "550e8400-e29b-41d4-a716-446655440010";
        auto send_event = get_send_event_function(send, pid);

        json event_data = {{"key", "value"}, {"count", 42}};

        send_event("test.event", event_data, "", "", "");

        REQUIRE(sent_event.contains("data"));
        REQUIRE(sent_event["data"]["key"] == "value");
        REQUIRE(sent_event["data"]["count"] == 42);
        REQUIRE(validate_super_schema(sent_event));
    }

    SECTION("Sends event with all optional parameters")
    {
        json sent_event;
        auto send = [&sent_event](const json& event)
        {
            sent_event = event;
        };

        std::string pid = "550e8400-e29b-41d4-a716-446655440010";
        auto send_event = get_send_event_function(send, pid);

        std::string cid = "550e8400-e29b-41d4-a716-446655440020";
        std::string uid = "550e8400-e29b-41d4-a716-446655440030";
        std::string token = "test-token";

        send_event("test.event", json{}, cid, uid, token);

        REQUIRE(sent_event["metadata"]["cid"] == cid);
        REQUIRE(sent_event["metadata"]["uid"] == uid);
        REQUIRE(sent_event["metadata"]["token"] == token);
        REQUIRE(validate_super_schema(sent_event));
    }

    SECTION("Uses data preprocessor")
    {
        json sent_event;
        auto send = [&sent_event](const json& event)
        {
            sent_event = event;
        };

        std::string pid = "550e8400-e29b-41d4-a716-446655440010";

        DataPreprocessors preprocessors;
        preprocessors["test.event"] = [](const json& data)
        {
            json processed = data;
            processed["processed"] = true;
            return processed;
        };

        auto send_event = get_send_event_function(send, pid, preprocessors);

        json event_data = {{"original", "data"}};
        send_event("test.event", event_data, "", "", "");

        REQUIRE(sent_event["data"]["original"] == "data");
        REQUIRE(sent_event["data"]["processed"] == true);
        REQUIRE(validate_super_schema(sent_event));
    }

    SECTION("Uses default preprocessor when specific not found")
    {
        json sent_event;
        auto send = [&sent_event](const json& event)
        {
            sent_event = event;
        };

        std::string pid = "550e8400-e29b-41d4-a716-446655440010";

        DataPreprocessors preprocessors;
        preprocessors["__default__"] = [](const json& data)
        {
            json processed = data;
            processed["default_processed"] = true;
            return processed;
        };

        auto send_event = get_send_event_function(send, pid, preprocessors);

        json event_data = {{"original", "data"}};
        send_event("other.event", event_data, "", "", "");

        REQUIRE(sent_event["data"]["original"] == "data");
        REQUIRE(sent_event["data"]["default_processed"] == true);
        REQUIRE(validate_super_schema(sent_event));
    }

    SECTION("Generated UUIDs are valid")
    {
        for (int i = 0; i < 10; i++)
        {
            std::string uuid = generate_uuid();
            REQUIRE(is_valid_uuid(uuid));
        }
    }

    SECTION("Generated UUIDs are unique")
    {
        std::string uuid1 = generate_uuid();
        std::string uuid2 = generate_uuid();
        REQUIRE(uuid1 != uuid2);
    }

    SECTION("Sets tid to event_id when not provided")
    {
        json sent_event;
        auto send = [&sent_event](const json& event)
        {
            sent_event = event;
        };

        std::string pid = "550e8400-e29b-41d4-a716-446655440010";
        auto send_event = get_send_event_function(send, pid);

        send_event("test.event", json{}, "", "", "");

        REQUIRE(sent_event["id"] == sent_event["metadata"]["tid"]);
    }

    SECTION("Sets cid to event_id when not provided")
    {
        json sent_event;
        auto send = [&sent_event](const json& event)
        {
            sent_event = event;
        };

        std::string pid = "550e8400-e29b-41d4-a716-446655440010";
        auto send_event = get_send_event_function(send, pid);

        send_event("test.event", json{}, "", "", "");

        REQUIRE(sent_event["id"] == sent_event["metadata"]["cid"]);
    }
}
