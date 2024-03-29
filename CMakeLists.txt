
cmake_minimum_required(VERSION 3.4...3.18)
project(ChessBot)

set(DEBUG 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Turn on to ensure -std=c++17 and not -std=gnu++17


set(AI_LIB_NAME ChessEngine)
set(TESTS_NAME EngineTests)
set(EXTRA_DIR ${CMAKE_SOURCE_DIR}/extra)

#CHESSLIBRARY
add_subdirectory(engine/bin)
target_compile_definitions(${AI_LIB_NAME} PUBLIC DEBUG=${DEBUG})

#TESTING
add_subdirectory(engine/tests)

# links output directory, so python can use it
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/engine/output)

#PYTHON MODULES
set(WRAPPER_NAME CPPChessLib)
add_subdirectory(extra/pybind11)
pybind11_add_module(${WRAPPER_NAME} engine/bin/wrapper.cpp)
target_compile_definitions(${WRAPPER_NAME} PRIVATE WRAPPER_NAME=${WRAPPER_NAME})
TARGET_LINK_LIBRARIES(${WRAPPER_NAME} PUBLIC ${AI_LIB_NAME})
