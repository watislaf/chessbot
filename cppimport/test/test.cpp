#include <gtest/gtest.h>
#include "chessAi.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
// Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
// Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(ChessAi, Basic) {
  ChessAi chess_ai;
  chess_ai.startNewGame(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  EXPECT_STREQ(chess_ai.getPossibleMovesForPiece(4, 1).c_str(),
               std::string("((4,1),(4,2)) ((4,1),(4,3)) ").c_str());
}