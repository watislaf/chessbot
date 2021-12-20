#ifndef BITBOARD_H
#define BITBOARD_H
#include <cstdint>
#include "tools/fen.h"
#include <bitset>

class BitBoard {
 public:
  explicit BitBoard(FEN fen);
  enum BPieceType {
    WHITE_PIECES = 0,     // any white piece
    BLACK_PIECES = 1,     // any black piece
    WHITE_PAWN = 2,
    BLACK_PAWN = 3,
    WHITE_KNIGHT = 4,
    BLACK_NIGHT = 5,
    WHITE_BISHOP = 6,
    BLACK_BISHOP = 7,
    WHITE_ROOK = 8,
    BLACK_RUCK = 9,
    WHITE_QUEEN = 10,
    BLACK_QUEEN = 11,
    WHITE_KING = 12,
    BLACK_KING = 13,

  };

  void staticDataInit();
  static bool _data_initialised;

  /// \return get set of 1 0 , witch represents position
  /// \return of piece_type on the board
  uint64_t get(BPieceType piece_type) const {
    return pieceBB[piece_type];
  }

  static const uint64_t notAFile = 0xfefefefefefefefe;
  static const uint64_t notHFile = 0x7f7f7f7f7f7f7f7f;
  static const uint64_t notGHFile = 0xfcfcfcfcfcfcfcfc;
  static const uint64_t notABFile = 0x3f3f3f3f3f3f3f3f;
  static const uint64_t debruijn64 = 0x03f79d71b4cb0a89;

  uint64_t pieceBB[14]{};
  uint64_t occupiedBB;
  uint64_t emptyBB;

  static std::string toStr(uint64_t);

  ///---              BIT OPERATIONS           ---///

  /// \return count one in set
  static int countBits(uint64_t set);

  /// need for bit scan algorithm
  static const int index64[64];

  /// \return lowest num of piece on board
  static int bitScanForward(uint64_t bb);

  /// \return 1 down
  static uint64_t soutOne(uint64_t bit);
  /// \return 1 up
  static uint64_t nortOne(uint64_t bit);
  /// \return 1 right
  static uint64_t eastOne(uint64_t bit);
  /// \return 1 right top
  static uint64_t noEaOne(uint64_t bit);
  /// \return 1  right down
  static uint64_t soEaOne(uint64_t bit);
  /// \return  1 left
  static uint64_t westOne(uint64_t bit);
  /// \return 1 left down
  static uint64_t soWeOne(uint64_t bit);
  /// \return 1 left top
  static uint64_t noWeOne(uint64_t bit);

  static uint64_t mirrorVertical(uint64_t bit);
  static uint64_t mirrorHorizontal(uint64_t bit);

  /// \return fill Horizontal from bottom to top
  static uint64_t northFill(uint64_t bit);
  /// \return fill Horizontal from top to bottop
  static uint64_t soutFill(uint64_t bit);
  /// \return fill Horizontal both bottom to top
  static uint64_t fileFill(uint64_t bit);

  static uint64_t wFrontFill(uint64_t wpawns);
  static uint64_t wRearFill(uint64_t wpawns);

  static uint64_t bFrontFill(uint64_t bpawns);
  static uint64_t bRearFill(uint64_t bpawns);

  /// \return front of white  pawns
  static uint64_t wFrontSpans(uint64_t wpawns);
  /// \return back of Black pawns
  static uint64_t bRearSpans(uint64_t bpawns);
  /// \return front of black pawns
  static uint64_t bFrontSpans(uint64_t bpawns);
  /// \return back of White pawns
  static uint64_t wRearSpans(uint64_t wpawns);

  static uint64_t wEastAttackFrontSpans(uint64_t wpawns);
  static uint64_t wWestAttackFrontSpans(uint64_t wpawns);
  static uint64_t bEastAttackFrontSpans(uint64_t bpawns);
  static uint64_t bWestAttackFrontSpans(uint64_t bpawns);

  static uint64_t wEastAttackRearSpans(uint64_t wpawns);
  static uint64_t wWestAttackRearSpans(uint64_t wpawns);
  static uint64_t bEastAttackRearSpans(uint64_t bpawns);
  static uint64_t bWestAttackRearSpans(uint64_t bpawns);

  // GENERATE ATTACKS FOR EACH Floating FIGURE
  static uint64_t soutAttacks(uint64_t rooks, uint64_t empty);
  static uint64_t nortAttacks(uint64_t rooks, uint64_t empty);
  static uint64_t noWeAttacks(uint64_t bishops, uint64_t empty);
  static uint64_t noEaAttacks(uint64_t bishops, uint64_t empty);
  static uint64_t eastAttacks(uint64_t rooks, uint64_t empty);
  static uint64_t soEaAttacks(uint64_t bishops, uint64_t empty);
  static uint64_t westAttacks(uint64_t rooks, uint64_t empty);
  static uint64_t soWeAttacks(uint64_t bishops, uint64_t empty);

  ///---          Figures pushing             ---///

  // PAWN
  const uint64_t rank4 = uint64_t(0x00000000FF000000);
  const uint64_t rank5 = uint64_t(0x000000FF00000000);
  static uint64_t wSinglePushTargets(uint64_t wpawns, uint64_t empty);
  static uint64_t bSinglePushTargets(uint64_t bpawns, uint64_t empty);
  static uint64_t wPawnsAble2Push(uint64_t wpawns, uint64_t empty);
  uint64_t wDblPushTargets(uint64_t wpawns, uint64_t empty) const;
  uint64_t bDoublePushTargets(uint64_t bpawns, uint64_t empty) const;
  uint64_t wPawnsAble2DblPush(uint64_t wpawns, uint64_t empty) const;

  static uint64_t wPawnEastAttacks(uint64_t wpawns);
  static uint64_t wPawnWestAttacks(uint64_t wpawns);
  static uint64_t bPawnEastAttacks(uint64_t bpawns);
  static uint64_t bPawnWestAttacks(uint64_t bpawns);
  static uint64_t wPawnsAble2CaptureEast(uint64_t wpawns, uint64_t bpieces);
  static uint64_t wPawnsAble2CaptureWest(uint64_t wpawns, uint64_t bpieces);
  static uint64_t bPawnsAble2CaptureEast(uint64_t bpawns, uint64_t wpieces);
  static uint64_t bPawnsAble2CaptureWest(uint64_t bpawns, uint64_t wpieces);

  //NIGHT
  static uint64_t forkTargetSquare(uint64_t targets);
  static uint64_t knightAttacks(uint64_t knights);
  static uint64_t noNoEa(uint64_t b);
  static uint64_t noEaEa(uint64_t b);
  static uint64_t soEaEa(uint64_t b);
  static uint64_t soSoEa(uint64_t b);
  static uint64_t noNoWe(uint64_t b);
  static uint64_t noWeWe(uint64_t b);
  static uint64_t soWeWe(uint64_t b);
  static uint64_t soSoWe(uint64_t b);

  // KING
  static uint64_t kingAttacks(uint64_t kingSet);

  // SLIDING PIECES
  enum enumDir {
    West = 0,
    Sout = 1,
    SoWe = 2,
    SoEa = 3,
    NoWe = 4,
    Nort = 5,
    NoEa = 6,
    East = 7,
  };

  static uint64_t PawnAttacks[2][64];
  static uint64_t KingAttacks[64];
  static uint64_t KnightAttacks[64];

  static uint64_t rayAttacks[64][8];
  static uint64_t FileAttacks[64];
  static uint64_t RankAttacks[64];
  static uint64_t DiagonalAttacks[64];
  static uint64_t AntiDiagonalAttacks[64];
  static uint64_t RookAttacks[64];
  static uint64_t BishopAttacks[64];
  static uint64_t QueenAttacks[64];

  // TODO https://www.chessprogramming.org/King_Pattern  Vulnerable on distant Checks
  static int bitScanReverse(uint64_t bb);
  static int bitScan(uint64_t bb, bool reverse);
  uint64_t getPositiveRayAttacks(uint64_t occupied, enumDir dir8, int square);
  uint64_t getNegativeRayAttacks(uint64_t occupied, enumDir dir8, int square);
  uint64_t getRayAttacks(uint64_t occupied, enumDir dir8, int square);
  bool isNegative(enumDir dir8);
  uint64_t diagonalAttacks(uint64_t occ, int sq);
  uint64_t antiDiagAttacks(uint64_t occ, int sq);
  uint64_t fileAttacks(uint64_t occ, int sq);
  uint64_t rankAttacks(uint64_t occ, int sq);
  uint64_t rookAttacks(uint64_t occ, int sq);
  uint64_t bishopAttacks(uint64_t occ, int sq);
  uint64_t queenAttacks(uint64_t occ, int sq);

  ///          ---             BOARD ANALYZE           ---           ////

  uint64_t attacksTo(uint64_t occupied, int sq);
  bool attacked(uint64_t occupied, int square, BPieceType bySide);

  // array of bitsets i,j where there is a line of 1's from i to j
  static uint64_t arrRectangular[64][64];

  /// \return set of 1 from position FROM to TO
  uint64_t inBetween(int from, int to);

  /// \return if empty between two squarse
  bool mayMove(int from, int to, uint64_t occupied);
  uint64_t xrayFileAttacks(uint64_t occ, uint64_t blockers, int rookSq);
  uint64_t xrayRankAttacks(uint64_t occ, uint64_t blockers, int rookSq);
  uint64_t xrayDiagonalAttacks(uint64_t occ, uint64_t blockers, int bishopSq);
  uint64_t attacksToKing(int squareOfKing, BPieceType colorOfKing);
  uint64_t getLeastValuablePiece(uint64_t attadef, int bySide, int& piece);
  int see(int toSq, BPieceType target, int frSq, BPieceType aPiece);
};

#endif //BITBOARD_H

