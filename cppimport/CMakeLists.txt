cmake_minimum_required(VERSION 3.4...3.18)
project(chess)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(MODULE_NAME "chessai")

add_subdirectory(pybind11 REQUIRED)
pybind11_add_module(${MODULE_NAME} class.cpp)

