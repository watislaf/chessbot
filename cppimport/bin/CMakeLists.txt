cmake_minimum_required(VERSION 3.4...3.18)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib")
set(LIB_HEADERS
        chessai/chessAi.cpp
        chessai/objBoard/objBoard.cpp
        chessai/objBoard/piece.cpp
        chessai/objBoard/movesGenerator.cpp
        chessai/bitBoard/bit_board.cpp
        chessai/bitBoard/BMovesGenerator.cpp
        chessai/tools/pricer.cpp
        chessai/tools/fen.cpp
        chessai/abstracts/move.cpp
        chessai/abstracts/position.cpp
        chessai/abstracts/aiAdvanceLvl.h
        chessai/movesTree.cpp
        chessai/abstracts/absBoard.h
        )
set(LIB_HEADERS ${LIB_HEADERS} ${EXTRA_DIR}/magics/magics.c)
ADD_LIBRARY(${AI_LIB_NAME} STATIC ${LIB_HEADERS}
        )
target_include_directories(${AI_LIB_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/chessai)
target_include_directories(${AI_LIB_NAME} PUBLIC ${EXTRA_DIR}/magics)
target_compile_options(${AI_LIB_NAME} PUBLIC -fPIC)
