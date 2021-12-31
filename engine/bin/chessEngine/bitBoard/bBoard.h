#ifndef BBOARD_H
#define BBOARD_H
#include <cstdint>
#include "tools/fen.h"
#include "bMove.h"
#include <bitset>

class
BBoard {
 public:
  BBoard(const BBoard&) = default;
  explicit BBoard(FEN fen);
  static void staticDataInit();
  static bool _data_initialised;
  bool operator==(const BBoard& other);
  void apply(const BMove& move);
  void unApply(const BMove& move);

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
    BLACK_ROOK = 9,
    WHITE_QUEEN = 10,
    BLACK_QUEEN = 11,
    WHITE_KING = 12,
    BLACK_KING = 13,
  };
  bool isWhiteTurn() const;
  BPieceType whosTurn() const;
  bool isShah(const BPieceType& whos_move) const;
  uint8_t countPieces() const;
  uint8_t getMoveCount() const;
  std::string toStr() const;
  BPieceType getType(uint8_t i) const;

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

  static std::string toStr(uint64_t);
  const uint64_t& getOccupied() const;

  /// \return get set of 1 0 , witch represents position
  /// \return of piece_type on the board
  uint64_t get(const BPieceType& white_piece_type,
               const BPieceType& color = BPieceType::WHITE_PIECES) const;

  static const uint64_t notAFile = 0xfefefefefefefefe;
  static const uint64_t notHFile = 0x7f7f7f7f7f7f7f7f;
  static const uint64_t notGHFile = 0xfcfcfcfcfcfcfcfc;
  static const uint64_t notABFile = 0x3f3f3f3f3f3f3f3f;
  static const uint64_t rank1 = uint64_t(0x00000000000000FF);
  static const uint64_t rank2 = uint64_t(0x000000000000FF00);
  static const uint64_t rank4 = uint64_t(0x00000000FF000000);
  static const uint64_t rank5 = uint64_t(0x000000FF00000000);
  static const uint64_t rank7 = uint64_t(0x00FF000000000000);
  static const uint64_t rank8 = uint64_t(0xFF00000000000000);

  ///---              BIT OPERATIONS           ---///
  /// \return count one in set
  static int countBits(uint64_t set);

  /// need for bit scan algorithm
  static const int index64[64];
  /// \return lowest num of piece on board
  static int bitScanForward(const uint64_t& bb);

  /// \return 1 down
  static uint64_t soutOne(const uint64_t& bit);
  /// \return 1 up
  static uint64_t nortOne(const uint64_t& bit);
  /// \return 1 right
  static uint64_t eastOne(const uint64_t& bit);
  /// \return 1 right top
  static uint64_t noEaOne(const uint64_t& bit);
  /// \return 1  right down
  static uint64_t soEaOne(const uint64_t& bit);
  /// \return  1 left
  static uint64_t westOne(const uint64_t& bit);
  /// \return 1 left down
  static uint64_t soWeOne(const uint64_t& bit);
  /// \return 1 left top
  static uint64_t noWeOne(const uint64_t& bit);

  static uint64_t mirrorVertical(uint64_t bit);
  static uint64_t mirrorHorizontal(uint64_t bit);

  /// \return fill Horizontal from bottom to top
  static uint64_t northFill(uint64_t bit);
  /// \return fill Horizontal from top to bottop
  static uint64_t soutFill(uint64_t bit);
  /// \return fill Horizontal both bottom to top
  static uint64_t fileFill(const uint64_t& bit);

  static uint64_t wFrontFill(const uint64_t& wpawns);
  static uint64_t wRearFill(const uint64_t& wpawns);

  static uint64_t bFrontFill(const uint64_t& bpawns);
  static uint64_t bRearFill(const uint64_t& bpawns);

  /// \return front of white  pawns
  static uint64_t wFrontSpans(const uint64_t& wpawns);
  /// \return back of Black pawns
  static uint64_t bRearSpans(const uint64_t& bpawns);
  /// \return front of black pawns
  static uint64_t bFrontSpans(const uint64_t& bpawns);
  /// \return back of White pawns
  static uint64_t wRearSpans(const uint64_t& wpawns);

  static uint64_t wEastAttackFrontSpans(const uint64_t& wpawns);
  static uint64_t wWestAttackFrontSpans(const uint64_t& wpawns);
  static uint64_t bEastAttackFrontSpans(const uint64_t& bpawns);
  static uint64_t bWestAttackFrontSpans(const uint64_t& bpawns);

  static uint64_t wEastAttackRearSpans(const uint64_t& wpawns);
  static uint64_t wWestAttackRearSpans(const uint64_t& wpawns);
  static uint64_t bEastAttackRearSpans(const uint64_t& bpawns);
  static uint64_t bWestAttackRearSpans(const uint64_t& bpawns);

  // GENERATE ATTACKS FOR EACH Floating FIGURE
  static uint64_t soutAttacks(uint64_t rooks, const uint64_t& empty);
  static uint64_t nortAttacks(uint64_t rooks, const uint64_t& empty);
  static uint64_t noWeAttacks(uint64_t bishops, const uint64_t& empty);
  static uint64_t noEaAttacks(uint64_t bishops, const uint64_t& empty);
  static uint64_t eastAttacks(uint64_t rooks, const uint64_t& empty);
  static uint64_t soEaAttacks(uint64_t bishops, const uint64_t& empty);
  static uint64_t westAttacks(uint64_t rooks, const uint64_t& empty);
  static uint64_t soWeAttacks(uint64_t bishops, const uint64_t& empty);

  ///---          Figures pushing             ---///

  // PAWN
  static uint64_t wSinglePushTargets(const uint64_t& wpawns,
                                     const uint64_t& empty);
  static uint64_t bSinglePushTargets(const uint64_t& bpawns,
                                     const uint64_t& empty);
  static uint64_t wPawnsAble2Push(const uint64_t& wpawns,
                                  const uint64_t& empty);
  static uint64_t wPawnsAble2DblPush(const uint64_t& wpawns,
                                     const uint64_t& empty);
  static uint64_t bPawnsAble2Push(const uint64_t& bpawns,
                                  const uint64_t& empty);
  static uint64_t bPawnsAble2DblPush(const uint64_t& bpawns,
                                     const uint64_t& empty);
  static uint64_t wDblPushTargets(const uint64_t& wpawns,
                                  const uint64_t& empty);
  static uint64_t bDoublePushTargets(const uint64_t& bpawns,
                                     const uint64_t& empty);

  static uint64_t wPawnEastAttacks(const uint64_t& wpawns);
  static uint64_t wPawnWestAttacks(const uint64_t& wpawns);
  static uint64_t bPawnEastAttacks(const uint64_t& bpawns);
  static uint64_t bPawnWestAttacks(const uint64_t& bpawns);
  static uint64_t wPawnsAble2CaptureEast(const uint64_t& wpawns,
                                         const uint64_t& bpieces);
  static uint64_t wPawnsAble2CaptureWest(const uint64_t& wpawns,
                                         const uint64_t& bpieces);
  static uint64_t bPawnsAble2CaptureEast(const uint64_t& bpawns,
                                         const uint64_t& wpieces);
  static uint64_t bPawnsAble2CaptureWest(const uint64_t& bpawns,
                                         const uint64_t& wpieces);

  //NIGHT
  static uint64_t forkTargetSquare(const uint64_t& targets);
  static uint64_t knightAttacks(const uint64_t& knights);
  static uint64_t knightAttacks(int pos);

  static uint64_t noNoEa(const uint64_t& bit);
  static uint64_t noEaEa(const uint64_t& bit);
  static uint64_t soEaEa(const uint64_t& bit);
  static uint64_t soSoEa(const uint64_t& bit);
  static uint64_t noNoWe(const uint64_t& bit);
  static uint64_t noWeWe(const uint64_t& bit);
  static uint64_t soWeWe(const uint64_t& bit);
  static uint64_t soSoWe(const uint64_t& bit);

  // KING
  static uint64_t kingAttacks(uint64_t kingSet);

  // TODO https://www.chessprogramming.org/King_Pattern  Vulnerable on distant Checks
  static int bitScanReverse(const uint64_t& bits);
  static int bitScan(const uint64_t& bits, bool reverse);
  uint64_t getPositiveRayAttacks(const uint64_t& occupied,
                                 enumDir dir8,
                                 int square);
  uint64_t getNegativeRayAttacks(const uint64_t& occupied,
                                 enumDir dir8,
                                 int square);
  uint64_t getRayAttacks(const uint64_t& occupied, enumDir dir8, int square);
  bool isNegative(enumDir dir8);
  uint64_t diagonalAttacks(const uint64_t& occupied, int sq);
  uint64_t antiDiagAttacks(const uint64_t& occupied, int sq);
  uint64_t fileAttacks(const uint64_t& occupied, int sq);
  uint64_t rankAttacks(const uint64_t& occupied, int sq);
  static uint64_t rookAttacks(uint64_t occupied, int sq);
  static uint64_t bishopAttacks(uint64_t occupied, int sq);
  static uint64_t queenAttacks(uint64_t occupied, int sq);

  ///          ---             BOARD ANALYZE           ---           ////

  uint64_t attacksTo(const uint64_t& occupied, int sq);
  bool attacked(const uint64_t& occupied, int square, const BPieceType& bySide) const;

  // array of bitsets i,j where there is a line of 1's from i to j
  static uint64_t arrRectangular[64][64];

  /// \return set of 1 from position FROM to TO
  uint64_t inBetween(int from, int to);

  /// \return if empty between two squarse
  bool mayMove(int from, int to, const uint64_t& occupied);
  uint64_t xrayFileAttacks(const uint64_t& occupied,
                           uint64_t blockers,
                           int rookSq);
  uint64_t xrayRankAttacks(const uint64_t& occupied,
                           uint64_t blockers,
                           int rookSq);
  uint64_t xrayDiagonalAttacks(const uint64_t& occupied,
                               uint64_t blockers,
                               int bishopSq);
  uint64_t attacksToKing(int squareOfKing, const BPieceType& colorOfKing) const;
  uint64_t getLeastValuablePiece(uint64_t attadef, int bySide, int& piece);
  int see(int toSq, const BPieceType& target, int frSq, const BPieceType& aPiece);

  const uint64_t& getLastDoublePush() const;

  static uint64_t wPawnsAble2WestEP(uint64_t wpawns, const uint64_t& file);
  static uint64_t wPawnsAble2EastEP(uint64_t wpawns, const uint64_t& file);
  static uint64_t bPawnsAble2WestEP(uint64_t bpawns, const uint64_t& file);
  static uint64_t bPawnsAble2EastEP(uint64_t bpawns, const uint64_t& file);
  BPieceType getPiece(uint8_t square, const BPieceType& side) const;

  static uint64_t one_square_[64];
  static uint64_t two_squares_[64][64];

  bool isCastle(const BMove& move) const;
  bool isBrakeCastle(const BMove& move,const bool& side) const;
  bool canLeftCastle() const;
  bool canRightCastle() const;

  static uint64_t bishopDiagonals(int sq);
  static uint64_t rookDiagonals(int sq);
  static uint64_t kingAttacks(uint8_t pos);
  static uint64_t queenDiagonal(uint8_t pos);

  uint64_t pieceBB[14]{};
 private:
  static uint64_t pawn_attacks_[2][64];
  static uint64_t king_attacks_[64];
  static uint64_t knight_attacks_[64];
  static uint64_t ray_attacks_[64][8];

  static uint64_t file_attacks_[64];

  static uint64_t rank_attacks_[65];
  static uint64_t diagonal_attacks_[64];
  static uint64_t anti_diagonal_attacks_[64];
  static uint64_t rook_diagonals_[64];
  static uint64_t bishop_diagonals_[64];
  static uint64_t queen_diagonals_[64];
  static uint64_t right_castle_spaces[2];
  static uint64_t left_castle_spaces[2];

  static const uint64_t debruijn64 = 0x03f79d71b4cb0a89;

  BPieceType onPos(uint8_t pos, uint64_t i) const;
  bool isBrakeLeftCastle(const BMove& move,const bool& side) const;
  bool isBrakeRightCastle(const BMove& move,const bool& side) const;
  bool isCanLeftCastle(const bool& side) const;
  bool isCanRightCastle(const bool& side) const;

  void setPieceCapturedThisMove(const BPieceType& type);
  BPieceType getPieceCapturedThisMove();

  uint8_t last_double_push_[400];
  BPieceType piece_captured_this_move_[400];
  bool last_left_castle[400];
  bool last_right_castle[400];

  uint8_t pieces_count_ = 0;
  uint8_t move_count_ = 0;
  bool is_white_turn_ = 0;
  uint64_t occupiedBB;
  bool left_castle_[2];
  bool right_castle_[2];

  void setPrevLeftCastle(bool b);
  void setPrevRightCastle(bool b);
  bool getPrevLeftCastle();
  bool getPrevRightCastle();
};

#endif //BBOARD_H

