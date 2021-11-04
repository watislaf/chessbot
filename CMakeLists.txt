cmake_minimum_required(VERSION 3.4...3.18)
project(chess)

#TESTING
include(cppimport/test/CMakeLists.txt REQUIRED)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/cppimport/output)

#PYTHON MODULES
set(MODULE_NAME "CPPchessai")
add_subdirectory(extra/pybind11 REQUIRED)
pybind11_add_module(${MODULE_NAME} cppimport/bin/wrapper.cpp)

