cmake_minimum_required(VERSION 3.12)
project(campus_help_button CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server
    main.cpp
    notifier.cpp
    storage.cpp
    utils.cpp
)

# Header-only: httplib.h and nlohmann/json.hpp expected in include path
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
