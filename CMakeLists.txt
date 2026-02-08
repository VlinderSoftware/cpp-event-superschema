cmake_minimum_required(VERSION 3.14)
project(event_superschema VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include FetchContent module
include(FetchContent)

# Fetch nlohmann/json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Fetch Catch2 for testing
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# Library target
add_library(event_superschema
  src/super_schema.cpp
  src/event_dispatcher.cpp
  src/send_event_function.cpp
  src/jwe_event_dispatcher.cpp
  src/jws_event_dispatcher.cpp
  src/jwe_send_event_function.cpp
  src/jws_send_event_function.cpp
)

target_include_directories(event_superschema
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(event_superschema
  PUBLIC
    nlohmann_json::nlohmann_json
)

# Test target
add_executable(event_superschema_tests
  tests/test_super_schema.cpp
  tests/test_event_dispatcher.cpp
  tests/test_send_event_function.cpp
)

target_link_libraries(event_superschema_tests
  PRIVATE
    event_superschema
    Catch2::Catch2WithMain
)

# Enable testing
enable_testing()
add_test(NAME event_superschema_tests COMMAND event_superschema_tests)

# Compiler warnings
if(MSVC)
  target_compile_options(event_superschema PRIVATE /W4)
  target_compile_options(event_superschema_tests PRIVATE /W4)
else()
  target_compile_options(event_superschema PRIVATE -Wall -Wextra -Wpedantic)
  target_compile_options(event_superschema_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install rules
install(TARGETS event_superschema
  EXPORT event_superschema-targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/event_superschema
  DESTINATION include
)
