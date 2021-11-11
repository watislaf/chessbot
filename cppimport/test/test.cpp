#include <gtest/gtest.h>
#include "chessAi.h"

TEST(ChessAi, Basic) {
  ChessAi chess_ai;

  chess_ai.startNewGame(
      "k6K/4P3/8/8/8/8/4p3/8 w - - 0 1");

  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(4, 6).c_str(),
               std::string(
                   "((4,6),(4,7),h) ((4,6),(4,7),r) ((4,6),(4,7),b) ((4,6),(4,7),q) ")
                   .c_str());

  chess_ai.startNewGame(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  chess_ai.applyMove(4, 0, 6, 0);
  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(6, 0).c_str(),
               std::string("((6,0),(5,2)) ((6,0),(7,2)) ")
                   .c_str());
  return;
  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(4, 7).c_str(),
               std::string("((4,7),(3,7)) ((4,7),(5,7)) ")
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
                   "((3,3),(2,2)) ((3,3),(2,3)) ((3,3),(2,4)) ((3,3),(3,2)) ((3,3),(3,4)) ((3,3),(4,2)) ((3,3),(4,3)) ((3,3),(4,4)) ").c_str());

  chess_ai.startNewGame(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  chess_ai.applyMove(4, 1, 4, 3);

  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(5, 6).c_str(),
               std::string("((5,6),(5,5)) ((5,6),(5,4)) ")
                   .c_str());
  chess_ai.applyMove(5, 6, 5, 4);
  chess_ai.applyMove(4, 3, 4, 4);
  chess_ai.applyMove(3, 6, 3, 4);
  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(4, 4).c_str(),
               std::string("((4,4),(4,5)) ((4,4),(3,5)) ")
                   .c_str());
}