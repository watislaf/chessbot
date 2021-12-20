cmake_minimum_required(VERSION 3.4...3.18)
project(chess)

set(AI_LIB_NAME ChessAiRaw)
set(TESTS_NAME AiTests)
set(EXTRA_DIR  ${CMAKE_SOURCE_DIR}/extra)

#CHESSLIBRARY
add_subdirectory(cppimport/bin)

#TESTING
add_subdirectory(cppimport/test)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/cppimport/output)

#PYTHON MODULES
set(WRAPPER_NAME CPPChessLib)
add_subdirectory(extra/pybind11 REQUIRED)
pybind11_add_module(${WRAPPER_NAME} cppimport/bin/wrapper.cpp)
target_compile_definitions(${WRAPPER_NAME} PRIVATE WRAPPER_NAME=${WRAPPER_NAME})
TARGET_LINK_LIBRARIES(${WRAPPER_NAME} PUBLIC ${AI_LIB_NAME} )
# TODO add black pycodestyle pylint
