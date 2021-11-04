# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(BIN_DIR ${CMAKE_CURRENT_LIST_DIR}/../bin/chessai)

#CMAKE_CURRENT_LIST_DIR - get current cmake dir
add_executable(
        hellotest
        ${CMAKE_CURRENT_LIST_DIR}/hellotest.cpp
        ${BIN_DIR}/chessAi.cpp
        ${BIN_DIR}/board/board.cpp
        ${BIN_DIR}/board/piece.cpp
        ${BIN_DIR}/board/pieceType.cpp
        ${BIN_DIR}/board/pieceColor.cpp
        ${BIN_DIR}/tools/moveGenerator.cpp
        ${BIN_DIR}/abstracts/move.cpp
        ${BIN_DIR}/abstracts/position.cpp
        ../bin/chessai/abstracts/move_type.cpp ../bin/chessai/abstracts/move_type.h)

target_include_directories(hellotest PRIVATE ${BIN_DIR})
target_link_libraries(
        hellotest
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(hellotest)