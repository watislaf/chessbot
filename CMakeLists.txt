cmake_minimum_required(VERSION 3.4...3.18)
project(chess)

set(MY_AI_LIB ChessAiRaw)
set(MY_TESTS_TO_AI AiTests)

#CHESSLIBRARY
add_subdirectory(cppimport/bin)

#TESTING
add_subdirectory(cppimport/test)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/cppimport/output)

#PYTHON MODULES
set(MY_WRAPPER wrap)
add_subdirectory(extra/pybind11 REQUIRED)
pybind11_add_module(${MY_WRAPPER} cppimport/bin/wrapper.cpp)
TARGET_LINK_LIBRARIES(${MY_WRAPPER} PUBLIC ${MY_AI_LIB} )

