cmake_minimum_required(VERSION 3.26)
project(cpp_based)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.0.0
)

FetchContent_MakeAvailable(fmt)

add_executable(cpp_based main.cpp
        interpreter.cpp
        interpreter.hpp)
target_link_libraries(cpp_based fmt)
