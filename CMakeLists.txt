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
        lang/lexer.cpp
        lang/lexer.hpp
        db/db.cpp
        db/db.hpp
        db/table.cpp
        db/table.hpp
        db/column.cpp
        db/column.hpp
        db/row.cpp
        db/row.hpp
        lang/ast.cpp
        lang/ast.hpp
        lang/parser.cpp
        lang/parser.hpp
        lang/interpreter.cpp
        lang/interpreter.hpp
        db/result.hpp
        db/result.cpp
        db/logic.cpp
        db/logic.hpp)
target_link_libraries(cpp_based fmt)

add_executable(cpp_based_db_test db_test.cpp
        db/db.cpp
        db/db.hpp
        db/table.cpp
        db/table.hpp
        db/column.cpp
        db/column.hpp
        db/row.cpp
        db/row.hpp
        db/result.hpp
        db/result.cpp
        db/logic.cpp
        db/logic.hpp)
target_link_libraries(cpp_based_db_test fmt)


