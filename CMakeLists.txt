cmake_minimum_required(VERSION 3.18)
project(
    Matrix
    VERSION 0.0.1
    DESCRIPTION "Matrix class with overloaded operators"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_executable(matrix_class main.cpp)
