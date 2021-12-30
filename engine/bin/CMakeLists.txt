cmake_minimum_required(VERSION 3.4...3.18)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib")
set(LIB_HEADERS
        chessEngine/chessEngine.cpp
        chessEngine/objBoard/objBoard.cpp
        chessEngine/objBoard/piece.cpp
        chessEngine/objBoard/movesGenerator.cpp
        chessEngine/bitBoard/bBoard.cpp
        chessEngine/bitBoard/bMove.cpp
        chessEngine/bitBoard/bMovesGenerator.cpp
        chessEngine/tools/pricer.cpp
        chessEngine/tools/fen.cpp
        chessEngine/abstracts/move.cpp
        chessEngine/abstracts/position.cpp
        chessEngine/abstracts/aiAdvanceLvl.h
        chessEngine/movesTree.cpp
        chessEngine/abstracts/absBoard.h
        )

set(LIB_HEADERS ${LIB_HEADERS} ${EXTRA_DIR}/magics/magics.c)
ADD_LIBRARY(${AI_LIB_NAME} STATIC ${LIB_HEADERS})
target_compile_definitions(${AI_LIB_NAME} PRIVATE ARCH=${ARCH})

target_compile_definitions(${AI_LIB_NAME} PUBLIC ARCH=32)


target_include_directories(${AI_LIB_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/chessEngine)
target_include_directories(${AI_LIB_NAME} PUBLIC ${EXTRA_DIR}/magics)
target_compile_options(${AI_LIB_NAME} PUBLIC -fPIC)
