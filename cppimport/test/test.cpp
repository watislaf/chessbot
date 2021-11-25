#include <gtest/gtest.h>
#include "chessAi.h"

TEST(ChessAi, AlphaBeta) {
  ChessAi chess_ai("A1");
  chess_ai.startNewGame(
      "1n6/8/8/8/8/3kP3/3P4/4K3 w - - 0 1");
  auto move = chess_ai.getBestMove();
  chess_ai.applyMove(move);


}
TEST(ChessAi, Algo) {
  ChessAi chess_ai("A1");
  chess_ai.startNewGame(
      "1n1k4/pp6/8/8/8/8/PPP5/3K4 w - - 0 1");
  chess_ai.applyMoveParams(1,1,1,3);
  auto move = chess_ai.getBestMove();
  chess_ai.applyMove(move);
  chess_ai.applyMoveParams(2,1,2,3);
  move = chess_ai.getBestMove();
  chess_ai.applyMove(move);
  chess_ai.applyMoveParams(0,1,0,2);
  move = chess_ai.getBestMove();
  chess_ai.applyMove(move);

}

TEST(ChessAi, A2) {
  ChessAi chess_ai("A1");
  ChessAi chess_ai2("random");
  for (int j = 0; j < 5; j++) {
    chess_ai.startNewGame(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    chess_ai2.startNewGame(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    for (int i = 0; i < 20; i++) {
      auto move = chess_ai.getBestMove();
      if (move.getStart()->getPosition() == Position(9, 9)) {std::cerr<<"GG"; break; };
      chess_ai2.applyMove(move);
      chess_ai.applyMove(move);
      std::cerr<<i<<"";
    }
    std::cerr<<j<<" \n";
  }
  // default time 20.10 <=
  // default time 18.36 <= , generate_no_sort
  // default time 26.36 <= , generate_no_sort, apply release no sort
  // default time 1.0.13 <= , generate_no_sort_!white
}

TEST(ChessAi, TreeGenerator) {
  ChessAi chess_ai("A1");
  chess_ai.startNewGame(
      "r1bqkbnr/pppppppp/n7/8/1P6/8/P1PPPPPP/RNBQKBNR w KQkq - 0 1");
  for (int i = 0; i < 10; i++) {
    auto move = chess_ai.getBestMove();
    chess_ai.isMoveExists();
    chess_ai.applyMove(move);
  }
}

TEST(ChessAi, MoveGenerator) {
  ChessAi chess_ai("random");
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