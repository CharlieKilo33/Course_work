cmake_minimum_required(VERSION 3.26)
project(Course_work)

set(CMAKE_CXX_STANDARD 20)

add_executable(test tests.cpp parser.h parser.cpp lexer.h lexer.cpp)
