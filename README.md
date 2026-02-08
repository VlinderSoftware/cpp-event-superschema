# C++ Event Superschema

A C++20 library for event validation, dispatching, and formatting with support for the event superschema standard. This library provides a robust foundation for event-driven architectures with type-safe event handling and schema validation.

## Features

- **Schema Validation**: Validate events against the superschema with UUID format checking
- **Event Dispatching**: Type-based event routing with fallback handlers
- **Event Formatting**: Automatic event formatting with UUID generation and metadata management
- **Data Preprocessing**: Transform event data before sending
- **JWE/JWS Support**: Placeholder implementations for encrypted and signed events (extensible with crypto libraries)
- **Modern C++**: Uses C++20 features, smart pointers, and functional programming patterns

## Dependencies

- **CMake 3.14+**: Build system
- **C++20 compiler**: GCC 10+, Clang 10+, or MSVC 2019+
- **nlohmann/json**: JSON handling (automatically fetched via CMake)
- **Catch2**: Testing framework (automatically fetched via CMake)

## Building

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
ctest --output-on-failure
# or directly:
./event_superschema_tests
```

## Quick Start

### Event Dispatching

```cpp
#include <event_superschema/event_dispatcher.hpp>
#include <iostream>

using namespace event_superschema;

int main() {
    // Define error handler
    auto err = [](const ErrorMessage& error) {
        std::cerr << "Error: " << error.error << " - " 
                  << error.message << std::endl;
    };

    // Define event handlers
    EventHandlers handlers;
    handlers["user.created"] = [](const ErrorHandler& err, const json& event) {
        std::cout << "User created: " << event["data"]["username"] << std::endl;
    };
    handlers["user.deleted"] = [](const ErrorHandler& err, const json& event) {
        std::cout << "User deleted: " << event["data"]["user_id"] << std::endl;
    };
    handlers["__default__"] = [](const ErrorHandler& err, const json& event) {
        std::cout << "Unhandled event: " << event["type"] << std::endl;
    };

    // Create dispatcher
    auto dispatcher = get_event_dispatcher(err, handlers);

    // Dispatch an event
    json event = {
        {"id", "550e8400-e29b-41d4-a716-446655440000"},
        {"type", "user.created"},
        {"metadata", {
            {"cid", "550e8400-e29b-41d4-a716-446655440001"},
            {"pid", "550e8400-e29b-41d4-a716-446655440002"}
        }},
        {"data", {
            {"username", "john_doe"}
        }}
    };

    dispatcher(event);

    return 0;
}
```

### Event Formatting and Sending

```cpp
#include <event_superschema/send_event_function.hpp>
#include <iostream>

using namespace event_superschema;

int main() {
    // Define send function (e.g., publish to message queue)
    auto send = [](const json& event) {
        std::cout << "Sending event: " << event.dump(2) << std::endl;
    };

    // Define producer ID
    std::string pid = "550e8400-e29b-41d4-a716-446655440099";

    // Optional: Define data preprocessors
    DataPreprocessors preprocessors;
    preprocessors["order.placed"] = [](const json& data) {
        json processed = data;
        processed["timestamp"] = std::time(nullptr);
        return processed;
    };

    // Create send event function
    auto send_event = get_send_event_function(send, pid, preprocessors);

    // Send an event
    json order_data = {
        {"order_id", "ORD-12345"},
        {"amount", 99.99}
    };

    send_event(
        "order.placed",           // event type
        order_data,               // event data
        "",                       // cid (auto-generated if empty)
        "550e8400-e29b-41d4-a716-446655440088", // uid
        "auth-token-xyz"          // token
    );

    return 0;
}
```

### Schema Validation

```cpp
#include <event_superschema/super_schema.hpp>
#include <iostream>

using namespace event_superschema;

int main() {
    json event = {
        {"id", "550e8400-e29b-41d4-a716-446655440000"},
        {"type", "test.event"},
        {"metadata", {
            {"cid", "550e8400-e29b-41d4-a716-446655440001"},
            {"pid", "550e8400-e29b-41d4-a716-446655440002"}
        }}
    };

    if (validate_super_schema(event)) {
        std::cout << "Event is valid!" << std::endl;
    } else {
        std::cout << "Event is invalid!" << std::endl;
    }

    return 0;
}
```

## Event Superschema Structure

All events must conform to this schema:

```json
{
  "id": "uuid",           // Required: Unique event ID
  "type": "string",       // Required: Event type (e.g., "user.created")
  "metadata": {           // Required: Event metadata
    "cid": "uuid",        // Required: Correlation ID
    "pid": "uuid",        // Required: Producer ID
    "tid": "uuid",        // Optional: Transaction ID
    "uid": "uuid",        // Optional: User ID
    "token": "string"     // Optional: Auth token
  },
  "data": {}              // Optional: Event payload
}
```

## API Reference

### Core Functions

#### `get_event_dispatcher`
Creates an event dispatcher that validates and routes events.

```cpp
EventDispatcher get_event_dispatcher(
    const ErrorHandler& err,
    const EventHandlers& handlers
);
```

**Parameters:**
- `err`: Error handler function
- `handlers`: Map of event types to handler functions

**Returns:** Event dispatcher function

#### `get_send_event_function`
Creates a function to format and send events.

```cpp
SendEventFunction get_send_event_function(
    const SendFunction& send,
    const std::string& pid,
    const DataPreprocessors& data_preprocessors = {}
);
```

**Parameters:**
- `send`: Function to send formatted events
- `pid`: Producer ID (UUID)
- `data_preprocessors`: Optional data transformation functions

**Returns:** Send event function

#### `validate_super_schema`
Validates an event against the superschema.

```cpp
bool validate_super_schema(const json& event);
```

#### `generate_uuid`
Generates a UUID v4 string.

```cpp
std::string generate_uuid();
```

### JWE/JWS Functions (Placeholder)

The library includes placeholder implementations for JWE (encryption) and JWS (signing):

- `get_jwe_event_dispatcher()` - Decrypt events before dispatching
- `get_jws_event_dispatcher()` - Verify signatures before dispatching
- `get_jwe_send_event_function()` - Encrypt events before sending
- `get_jws_send_event_function()` - Sign events before sending

These can be extended with actual crypto implementations using libraries like [jwt-cpp](https://github.com/Thalhammer/jwt-cpp).

## Testing

The library includes comprehensive tests using Catch2:

```bash
# Run all tests
./build/event_superschema_tests

# Run specific test
./build/event_superschema_tests "[dispatcher]"

# List all tests
./build/event_superschema_tests --list-tests
```

## Integration

### CMake Integration

```cmake
add_subdirectory(path/to/cpp-event-superschema)

target_link_libraries(your_target
    PRIVATE
        event_superschema
)
```

### Header-Only Usage

Include the headers and link against the library:

```cpp
#include <event_superschema/event_dispatcher.hpp>
#include <event_superschema/send_event_function.hpp>
#include <event_superschema/super_schema.hpp>
```

## Design Patterns

### Event Versioning

Handle multiple versions of the same event:

```cpp
handlers["order.placed"] = [](const ErrorHandler& err, const json& event) {
    // Handles both "order.placed" and "order.placed:v1", "order.placed:v2", etc.
};
```

### Default Handler

Catch all unhandled events:

```cpp
handlers["__default__"] = [](const ErrorHandler& err, const json& event) {
    // Handle any event without a specific handler
};
```

### Data Transformation

Transform event data before sending:

```cpp
preprocessors["__default__"] = [](const json& data) {
    // Apply to all events without specific preprocessor
    return data;
};

preprocessors["specific.event"] = [](const json& data) {
    // Apply only to "specific.event"
    json transformed = data;
    // ... transform data ...
    return transformed;
};
```

## License

See LICENSE file for details.

## Related Implementations

- Python: `/workspace/python-event-superschema/`
- Node.js/TypeScript: `/workspace/node-event-superschema/`
- C#: `/workspace/cs-event-superschema/`
