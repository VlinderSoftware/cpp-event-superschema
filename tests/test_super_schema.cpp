#include <catch2/catch_test_macros.hpp>

#include "event_superschema/super_schema.hpp"

using namespace Vlinder::EventSuperschema;

TEST_CASE("Super schema validation", "[super_schema]")
{
    SECTION("Valid minimal event")
    {
        json valid_event = {{"id", "550e8400-e29b-41d4-a716-446655440000"},
                            {"type", "test.event"},
                            {"metadata",
                             {{"cid", "550e8400-e29b-41d4-a716-446655440001"},
                              {"pid", "550e8400-e29b-41d4-a716-446655440002"}}}};

        REQUIRE(validate_super_schema(valid_event));
    }

    SECTION("Valid event with all optional fields")
    {
        json valid_event = {{"id", "550e8400-e29b-41d4-a716-446655440000"},
                            {"type", "test.event"},
                            {"metadata",
                             {{"cid", "550e8400-e29b-41d4-a716-446655440001"},
                              {"tid", "550e8400-e29b-41d4-a716-446655440003"},
                              {"pid", "550e8400-e29b-41d4-a716-446655440002"},
                              {"uid", "550e8400-e29b-41d4-a716-446655440004"},
                              {"token", "some-token"}}},
                            {"data", {{"key", "value"}}}};

        REQUIRE(validate_super_schema(valid_event));
    }

    SECTION("Invalid - missing id")
    {
        json invalid_event = {{"type", "test.event"},
                              {"metadata",
                               {{"cid", "550e8400-e29b-41d4-a716-446655440001"},
                                {"pid", "550e8400-e29b-41d4-a716-446655440002"}}}};

        REQUIRE_FALSE(validate_super_schema(invalid_event));
    }

    SECTION("Invalid - missing type")
    {
        json invalid_event = {{"id", "550e8400-e29b-41d4-a716-446655440000"},
                              {"metadata",
                               {{"cid", "550e8400-e29b-41d4-a716-446655440001"},
                                {"pid", "550e8400-e29b-41d4-a716-446655440002"}}}};

        REQUIRE_FALSE(validate_super_schema(invalid_event));
    }

    SECTION("Invalid - missing metadata")
    {
        json invalid_event = {{"id", "550e8400-e29b-41d4-a716-446655440000"},
                              {"type", "test.event"}};

        REQUIRE_FALSE(validate_super_schema(invalid_event));
    }

    SECTION("Invalid - missing cid in metadata")
    {
        json invalid_event = {{"id", "550e8400-e29b-41d4-a716-446655440000"},
                              {"type", "test.event"},
                              {"metadata", {{"pid", "550e8400-e29b-41d4-a716-446655440002"}}}};

        REQUIRE_FALSE(validate_super_schema(invalid_event));
    }

    SECTION("Invalid - missing pid in metadata")
    {
        json invalid_event = {{"id", "550e8400-e29b-41d4-a716-446655440000"},
                              {"type", "test.event"},
                              {"metadata", {{"cid", "550e8400-e29b-41d4-a716-446655440001"}}}};

        REQUIRE_FALSE(validate_super_schema(invalid_event));
    }

    SECTION("Invalid - bad UUID format in id")
    {
        json invalid_event = {{"id", "not-a-uuid"},
                              {"type", "test.event"},
                              {"metadata",
                               {{"cid", "550e8400-e29b-41d4-a716-446655440001"},
                                {"pid", "550e8400-e29b-41d4-a716-446655440002"}}}};

        REQUIRE_FALSE(validate_super_schema(invalid_event));
    }

    SECTION("Invalid - bad UUID format in cid")
    {
        json invalid_event = {
            {"id", "550e8400-e29b-41d4-a716-446655440000"},
            {"type", "test.event"},
            {"metadata", {{"cid", "bad-uuid"}, {"pid", "550e8400-e29b-41d4-a716-446655440002"}}}};

        REQUIRE_FALSE(validate_super_schema(invalid_event));
    }

    SECTION("Invalid - data is not an object")
    {
        json invalid_event = {{"id", "550e8400-e29b-41d4-a716-446655440000"},
                              {"type", "test.event"},
                              {"metadata",
                               {{"cid", "550e8400-e29b-41d4-a716-446655440001"},
                                {"pid", "550e8400-e29b-41d4-a716-446655440002"}}},
                              {"data", "not-an-object"}};

        REQUIRE_FALSE(validate_super_schema(invalid_event));
    }
}

TEST_CASE("UUID validation", "[uuid]")
{
    SECTION("Valid UUIDs")
    {
        REQUIRE(is_valid_uuid("550e8400-e29b-41d4-a716-446655440000"));
        REQUIRE(is_valid_uuid("6ba7b810-9dad-11d1-80b4-00c04fd430c8"));
        REQUIRE(is_valid_uuid("AAAAAAAA-BBBB-CCCC-DDDD-EEEEEEEEEEEE"));
    }

    SECTION("Invalid UUIDs")
    {
        REQUIRE_FALSE(is_valid_uuid("not-a-uuid"));
        REQUIRE_FALSE(is_valid_uuid("550e8400-e29b-41d4-a716"));
        REQUIRE_FALSE(is_valid_uuid("550e8400-e29b-41d4-a716-446655440000-extra"));
        REQUIRE_FALSE(is_valid_uuid(""));
        REQUIRE_FALSE(is_valid_uuid("550e8400e29b41d4a716446655440000"));  // Missing hyphens
    }
}
