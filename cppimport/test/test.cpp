#include <gtest/gtest.h>
#include "chessAi.h"

TEST(ChessAi, Basic) {
  ChessAi chess_ai;
  chess_ai.startNewGame(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  std::cout<<chess_ai.getBoardStr();
  EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(4, 1).c_str(),
               std::string("((4,1),(4,2)) ((4,1),(4,3)) ").c_str());
  //EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(0, 6).c_str(),
   //            std::string("((0,6),(0,5)) ((0,6),(0,4)) ").c_str());
  //EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(0, 6).c_str(),
  //             std::string("((0,6),(0,5)) ((0,6),(0,4)) ").c_str());
  // chess_ai.startNewGame("3k4/2ppp3/8/8/8/8/3PP3/3K4 w - - 0 1");
  //EXPECT_STREQ(chess_ai.getPossibleMovesForPosition(3, 7).c_str(),
  //             std::string("((0,6),(0,5)) ((0,6),(0,4))  ((0,6),(0,4))").c_str());

}