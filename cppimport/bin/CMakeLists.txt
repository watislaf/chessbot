cmake_minimum_required(VERSION 3.4...3.18)

if (NOT DEFINED ${MY_AI_LIB})
    project(OnlyCPP)
    SET(MY_AI_LIB CPP_ONLY)
endif (NOT DEFINED ${MY_AI_LIB})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib")
ADD_LIBRARY(${MY_AI_LIB} STATIC
        chessai/chessAi.cpp
        chessai/board/board.cpp
        chessai/board/piece.cpp
        chessai/tools/movesGenerator.cpp
        chessai/tools/pricer.cpp
        chessai/tools/fen.cpp
        chessai/abstracts/move.cpp
        chessai/abstracts/position.cpp main.cpp
        chessai/tools/movesTree.h)
target_include_directories(${MY_AI_LIB} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/chessai)
target_compile_options(${MY_AI_LIB} PUBLIC -fPIC)

if (${MY_AI_LIB} EQUAL OnlyCPP)
    ADD_EXECUTABLE(MAIN main.cpp)
    target_link_libraries(MAIN ${MY_AI_LIB})
endif (${MY_AI_LIB} EQUAL OnlyCPP)
