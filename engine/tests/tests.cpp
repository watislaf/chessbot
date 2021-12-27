#include <gtest/gtest.h>
#include "chessEngine.h"
#include "bitBoard/bit_board.h"
#include "bitBoard/b_move.h"
#include "bitBoard/BMovesGenerator.h"

TEST(BitBoard, test1) {
  BitBoard board(FEN("k7/pppppppp/8/8/8/8/PPPPPPPP/K7 w KQkq - 0 1"));
  std::cout << board.toStr();
  auto vector_bmove = BMovesGenerator::generate(&board, BitBoard::WHITE_PIECES);
  for (const auto& move: vector_bmove) {
    std::cout << move.toStr() << "\n";
  }
}

TEST(ChessAi, TEMP) {
  ChessEngine chess_ai("A1");
  chess_ai.startNewGame(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  chess_ai.applyMoveParams(4, 1, 4, 3);
  auto move = chess_ai.getBestMove();
  chess_ai.applyMove(move);
  std::cerr << move.toStr();

}

TEST(ChessAi, AlphaBeta) {
  ChessEngine chess_ai("random");
  ChessEngine chess_ai2("random");
  while (true) {
    chess_ai.startNewGame(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    chess_ai2.startNewGame(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    int move_p = 0;
    while (true) {
      Move move = chess_ai.getBestMove();;
      if (move_p == 0) {
        move = chess_ai.getBestMove();
        move_p = 1;
      } else {
        move_p = 0;
        move = chess_ai2.getBestMove();
      }
      if (move.getStart()->getPosition() == Position(9, 9)) {
        break;
      }
      std::cerr << move.toStr() << "\n";
      chess_ai.applyMove(move);
      chess_ai2.applyMove(move);
    }
    break;
  }
}

TEST(ChessAi, TreeGenerator) {
  ChessEngine chess_ai("A1");
  chess_ai.startNewGame(
      "r1bqkbnr/pppppppp/n7/8/1P6/8/P1PPPPPP/RNBQKBNR w KQkq - 0 1");
  for (int i = 0; i < 10; i++) {
    auto move = chess_ai.getBestMove();
    chess_ai.isMoveExists();
    chess_ai.applyMove(move);
  }

  // 3sec 666mc  4 default 6 attack
  //
}

TEST(ChessAi, MoveGenerator) {
  ChessEngine chess_ai("random");
  chess_ai.startNewGame(
      "r1bqkbnr/pppppppp/n7/8/1P6/8/P1PPPPPP/RNBQKBNR w KQkq - 0 1");
  auto move = chess_ai.getBestMove();

  chess_ai.startNewGame(
      "k7/7p/6QK/8/8/8/8/8 b - - 0 1");
  move = chess_ai.getBestMove();
  chess_ai.applyMove(move);

  chess_ai.startNewGame(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  chess_ai.applyMoveParams(4, 1, 4, 2);
  move = chess_ai.getBestMove();

  chess_ai.applyMoveParams(1, 7, 0, 5);
  chess_ai.applyMoveParams(5, 0, 2, 3);
  move = chess_ai.getBestMove();

  chess_ai.applyMoveParams(0, 5, 1, 7);

  chess_ai.applyMoveParams(3, 0, 5, 2);
  move = chess_ai.getBestMove();
  chess_ai.applyMoveParams(1, 7, 0, 5);
  chess_ai.applyMoveParams(5, 2, 5, 6);
  move = chess_ai.getBestMove();
  chess_ai.startNewGame(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  chess_ai.applyMoveParams(6, 0, 7, 2);
  chess_ai.applyMoveParams(1, 6, 1, 4);
  chess_ai.applyMoveParams(5, 1, 5, 3);
  chess_ai.applyMoveParams(1, 4, 1, 3);
  chess_ai.applyMoveParams(7, 2, 6, 4);
  chess_ai.applyMoveParams(2, 6, 2, 4);
  chess_ai.applyMoveParams(0, 1, 0, 3);
  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(1, 3).c_str(),
               std::string(
                   "((1,3),(0,2)) ((1,3),(1,2)) ")
                   .c_str());
  chess_ai.startNewGame(
      "k6K/4P3/8/8/8/8/4p3/8 w - - 0 1");

  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(4, 6).c_str(),
               std::string(
                   "((4,6),(4,7),n) ((4,6),(4,7),r) ((4,6),(4,7),b) ((4,6),(4,7),q) ")
                   .c_str());

  chess_ai.startNewGame(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(0, 6).c_str(),
               std::string("((0,6),(0,5)) ((0,6),(0,4)) ").c_str());

  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(4, 1).c_str(),
               std::string("((4,1),(4,2)) ((4,1),(4,3)) ").c_str());
  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(0, 6).c_str(),
               std::string("((0,6),(0,5)) ((0,6),(0,4)) ").c_str());
  chess_ai.startNewGame(
      "rnbqkbnr/pppp1ppp/8/4p3/3K4/8/PPPPPPPP/RNBQ_BNR w KQkq - 0 1");
  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(3, 3).c_str(),
               std::string(
                   "((3,3),(2,2)) ((3,3),(2,3)) ((3,3),(3,2)) ((3,3),(3,4)) ((3,3),(4,2)) ((3,3),(4,3)) ((3,3),(4,4)) ").c_str());

  chess_ai.startNewGame(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  chess_ai.applyMoveParams(4, 1, 4, 3);

  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(5, 6).c_str(),
               std::string("((5,6),(5,5)) ((5,6),(5,4)) ")
                   .c_str());
  chess_ai.applyMoveParams(5, 6, 5, 4);
  chess_ai.applyMoveParams(4, 3, 4, 4);
  chess_ai.applyMoveParams(3, 6, 3, 4);
  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(4, 4).c_str(),
               std::string("((4,4),(3,5)) ((4,4),(4,5)) ")
                   .c_str());
}