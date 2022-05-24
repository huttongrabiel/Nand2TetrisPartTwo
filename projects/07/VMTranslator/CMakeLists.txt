cmake_minimum_required(VERSION 3.16)

project(VMEmulator)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++20 -Werror -D CMAKE_CXX_COMPILER=g++-11")

set(SRC Main.cpp
        Lexer.h 
        Lexer.cpp 
        Parser.h 
        Parser.cpp 
        Codegen.h 
        Codegen.cpp
   )

add_executable(VMEmulator ${SRC})

include_directories("${CMAKE_SOURCE_DIR}/")

target_link_libraries(VMEmulator)