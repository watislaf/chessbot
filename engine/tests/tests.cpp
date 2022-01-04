#include <gtest/gtest.h>
#include "chessEngine.h"
#include "bitBoard/bBoard.h"
#include "bitBoard/bMove.h"
#include "bitBoard/bMovesGenerator.h"

TEST(BBoard, test1) {
  ChessEngine engine("bullet");
  engine.startNewGame("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  engine.applyMove( engine.getBestMove());
  engine.applyMove( engine.getBestMove());
  engine.applyMove( engine.getBestMove());
  /*
  ChessEngine chess_ai("random");
  chess_ai.startNewGame("k7/4P3/8/8/8/8/4p3/K7 b KQkq - 0 1");
  chess_ai.applyMoveParams(4, 1, 4, 0, 'q');
  chess_ai.applyMoveParams(4, 6, 4, 7, 'r');
///////////////////////////////////

  ChessEngine chess_ai2("random");
  chess_ai2.startNewGame("k7/4P3/N7/8/n7/8/4p3/K7 b KQkq - 0 1");
  chess_ai2.applyMoveParams(4, 1, 4, 0, 'q');
  chess_ai2.applyMoveParams(4, 6, 4, 7, 'r');

  //
  BBoard board(FEN("k7/4P3/8/8/8/3B4/4p3/K7 w Qkq - 0 1"));
  BBoard board2(FEN("k7/4P3/8/8/8/3B4/4p3/K7 w KQkq - 0 1"));
  board.apply(BMove(19, 12, BMove::CAPTURE_BY_BISHOP));
  std::cout << BBoard::toStr(board.get(BBoard::BLACK_PIECES));
  auto vector_bmove = BMovesGenerator::generate(&board, BBoard::BLACK_PIECES);
  for (const auto& move: vector_bmove) {
    board.apply(move);
    board.unApply(move);
    std::cout << move.toStr() << "\n";
  }

  board.unApply(BMove(19, 12, BMove::CAPTURE_BY_BISHOP));
//  ChessEngine chess_ai3("random");
  // chess_ai2.startNewGame("k7/8/B7/8/8/8/8/K7 w -- 0 1");


*/
//  ChessEngine ai("bullet");
//  ai.startNewGame("rn2kn1r/8/8/8/8/8/8/R1N1K1NR w KQkq - 0 1");
//  ai.startNewGame("rn2kn1r/8/8/8/8/8/8/R1N1K1NR w KQkq - 0 1");
//  ai.applyMove(BMove(5, 61, BMove::QUEEN_MOVE));
//  auto move = ai.getBestMove();
//  std::cout << move.toStr();
//  ai.applyMove(move);
  // ai.applyMove(BMove(61, 56, BMove::CAPTURE_BY_QUEEN));

  //move = ai.getBestMove();
  //std::cout << move.toStr();

  BBoard board(FEN("7k/8/8/8/8/ppp5/8/K5R1 w KQkq - 0 1"));
  BBoard board2(FEN("7k/8/8/8/8/ppp5/8/K5R1 w KQkq - 0 1"));
  board.apply(BMove(0, 1, BMove::KING_MOVE));
  board.apply(BMove(18, 10, BMove::PAWN_MOVE));

  std::cout << board.toStr();
  auto vector_bmove = BMovesGenerator::generate(&board);
  for (const auto& move: vector_bmove) {
    board.apply(move);
    board.unApply(move);
    if (!(board == board2)) {
      std::cout << "PO";
    }
    std::cout << move.toStr() << "\n";
  }
//  board.apply(Move(4, 6, BMove::KING_MOVE));
///  board2.apply(Move(4, 6, BMove::KING_MOVE));
  return;
//  board.apply(Move(60, 52, BMove::RUCK_MOVE));
//  board2.apply(Move(60, 52, BMove::RUCK_MOVE));

  std::cout << board.toStr();
  vector_bmove = BMovesGenerator::generate(&board);
  for (const auto& move: vector_bmove) {
    board.apply(move);
    board.unApply(move);
    if (!(board == board2)) {
      std::cout << "PO";
    }
    std::cout << move.toStr() << "\n";
  }
}

TEST(ChessAi, TEMP) {
  ChessEngine chess_ai("bullet");
  chess_ai.startNewGame(
      "8/8/8/8/6k1/4K3/7p/5R2 b - - 1 57");
  auto move = chess_ai.getBestMove();
  std::cout << move.toStr();
  chess_ai.applyMove(move);
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
      BMove move = chess_ai.getBestMove();;
      if (move_p == 0) {
        move = chess_ai.getBestMove();
        move_p = 1;
      } else {
        move_p = 0;
        move = chess_ai2.getBestMove();
      }
#if ARCH == 32
      if (move.getStart()->getPosition() == Position(9, 9)) {
        break;
      }
#else
      if (move.isInvalid()) {
        break;
      }
#endif
      std::cerr << move.toStr() << "\n";
      chess_ai.applyMove(move);
      chess_ai2.applyMove(move);
    }
    break;
  }
}

TEST(ChessAi, TreeGenerator) {
  ChessEngine chess_ai("bullet");
  chess_ai.startNewGame(
      "r1bqkbnr/pppppppp/n7/8/1P6/8/P1PPPPPP/RNBQKBNR w KQkq - 0 1");
  for (int i = 0; i < 30; i++) {
    auto move = chess_ai.getBestMove();
    if (chess_ai.isMoveExists()) {
      chess_ai.applyMove(move);
    } else {
      std::cout << "END";
    }
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