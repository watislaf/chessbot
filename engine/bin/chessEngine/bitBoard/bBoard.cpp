#include <iostream>
#include "bBoard.h"
#include "magicsWrapper.cpp"

bool  BBoard::_data_initialised = false;
const int BBoard::index64[64] = {
    0, 47, 1, 56, 48, 27, 2, 60,
    57, 49, 41, 37, 28, 16, 3, 61,
    54, 58, 35, 52, 50, 42, 21, 44,
    38, 32, 29, 23, 17, 11, 4, 62,
    46, 55, 26, 59, 40, 36, 15, 53,
    34, 51, 20, 43, 31, 22, 10, 45,
    25, 39, 14, 33, 19, 30, 9, 24,
    13, 18, 8, 12, 7, 6, 5, 63
};
uint64_t BBoard::right_castle_spaces[2];
uint64_t BBoard::left_castle_spaces[2];

uint64_t BBoard::pawn_attacks_[2][64];
uint64_t BBoard::free_pawn_[2][64];
uint64_t BBoard::king_attacks_[64];
uint64_t BBoard::knight_attacks_[64];
uint64_t BBoard::after_castle_king_positions[2][2];
uint64_t BBoard::defending_pawns[2][2];
uint64_t BBoard::pawn_road_[64];
uint64_t BBoard::ray_attacks_[64][8];
uint64_t BBoard::file_attacks_[64];
uint64_t BBoard::rank_attacks_[65];
uint64_t BBoard::diagonal_attacks_[64];
uint64_t BBoard::anti_diagonal_attacks_[64];
uint64_t BBoard::rook_diagonals_[64];
uint64_t BBoard::bishop_diagonals_[64];
uint64_t BBoard::queen_diagonals_[64];
uint64_t BBoard::arrRectangular[64][64];
uint64_t BBoard::one_square_[64];
uint64_t BBoard::two_squares_[64][64];
uint64_t BBoard::good_castle_pawns;

BBoard::BBoard(FEN fen) {
  last_double_push_[0] = 64;
  for (int i = 0; i < 14; i++) {
    pieceBB[i] = 0;
  }
  left_castle_[BPieceType::WHITE_PIECES] = fen.getWLC();
  right_castle_[BPieceType::WHITE_PIECES] = fen.getWRC();

  left_castle_[BPieceType::BLACK_PIECES] = fen.getBLC();
  right_castle_[BPieceType::BLACK_PIECES] = fen.getBRC();

  pieces_count_ = 64;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      auto piece = fen.getPiece(i, j);
      if (piece.getType() == PieceType::NONE) {
        pieces_count_--;
        continue;
      }
      pieceBB[static_cast<int>(piece.getPieceColor())] |=
          (u_int64_t(1) << ((7 - i) * 8 + j));
      pieceBB[static_cast<int>(piece.getType()) * 2
          + static_cast<int>(piece.getPieceColor())]
          |= (u_int64_t(1) << ((7 - i) * 8 + j));
    }
  }
  is_white_turn_ = fen.getIsWhiteMowe();
  occupiedBB = pieceBB[0] | pieceBB[1];
  staticDataInit();
}

int BBoard::bitScanReverse(const uint64_t& bits) {
  unsigned long int nlb;
  asm (
  "BSR %[a], %[nlb]"
  : [nlb] "=r"(nlb)
  : [a] "mr"(bits)
  );
  return nlb;
}
int BBoard::bitScanForward(const uint64_t& bits) {
  unsigned long int nlb;
  asm (
  "BSF %[a], %[nlb]"
  : [nlb] "=r"(nlb)
  : [a] "mr"(bits)
  );
  return nlb;
}

int BBoard::bitScan(const uint64_t& bits, bool reverse) {
  if (reverse) {
    return bitScanReverse(bits);
  }
  return bitScanForward(bits);
}

int BBoard::countBits(uint64_t set) {
  const auto k1 = uint64_t(0x5555555555555555); /*  -1/3   */
  const auto k2 = uint64_t(0x3333333333333333); /*  -1/5   */
  const auto k4 = uint64_t(0x0f0f0f0f0f0f0f0f); /*  -1/17  */
  const auto kf = uint64_t(0x0101010101010101); /*  -1/255 */
  set = set - ((set >> 1) & k1);
  set = (set & k2) + ((set >> 2) & k2);
  set = (set + (set >> 4)) & k4;
  set = (set * kf) >> 56;
  return (uint8_t) set;
}

uint64_t BBoard::soutOne(const uint64_t& bit) {
  return bit >> 8;
}
uint64_t BBoard::nortOne(const uint64_t& bit) {
  return bit << 8;
}
uint64_t BBoard::eastOne(const uint64_t& bit) {
  return (bit & notHFile) << 1;
}
uint64_t BBoard::noEaOne(const uint64_t& bit) {
  return (bit & notHFile) << 9;
}
uint64_t BBoard::soEaOne(const uint64_t& bit) {
  return (bit & notHFile) >> 7;
}
uint64_t BBoard::westOne(const uint64_t& bit) {
  return (bit & notAFile) >> 1;
}
uint64_t BBoard::soWeOne(const uint64_t& bit) {
  return (bit & notAFile) >> 9;
}
uint64_t BBoard::noWeOne(const uint64_t& bit) {
  return (bit & notAFile) << 7;
}

uint64_t BBoard::mirrorHorizontal(uint64_t bit) {
  const auto k1 = uint64_t(0x5555555555555555);
  const auto k2 = uint64_t(0x3333333333333333);
  const auto k4 = uint64_t(0x0f0f0f0f0f0f0f0f);
  bit = ((bit >> 1) & k1) | ((bit & k1) << 1);
  bit = ((bit >> 2) & k2) | ((bit & k2) << 2);
  bit = ((bit >> 4) & k4) | ((bit & k4) << 4);
  return bit;
}
uint64_t BBoard::mirrorVertical(uint64_t bit) {
  const auto k1 = uint64_t(0x00FF00FF00FF00FF);
  const auto k2 = uint64_t(0x0000FFFF0000FFFF);
  bit = ((bit >> 8) & k1) | ((bit & k1) << 8);
  bit = ((bit >> 16) & k2) | ((bit & k2) << 16);
  bit = (bit >> 32) | (bit << 32);
  return bit;
}
uint64_t BBoard::northFill(uint64_t bit) {
  bit |= (bit << 8);
  bit |= (bit << 16);
  bit |= (bit << 32);
  return bit;
}

uint64_t BBoard::soutFill(uint64_t bit) {
  bit |= (bit >> 8);
  bit |= (bit >> 16);
  bit |= (bit >> 32);
  return bit;
}

uint64_t BBoard::fileFill(const uint64_t& bit) {
  return soutFill(bit) | northFill(bit);
}

uint64_t BBoard::wFrontSpans(const uint64_t& wpawns) {
  return nortOne(northFill(wpawns));
}

uint64_t BBoard::bRearSpans(const uint64_t& bpawns) {
  return nortOne(northFill(bpawns));
}
uint64_t BBoard::bFrontSpans(const uint64_t& bpawns) {
  return soutOne(soutFill(bpawns));
}
uint64_t BBoard::wRearSpans(const uint64_t& wpawns) {
  return soutOne(soutFill(wpawns));
}
std::string BBoard::toStr(uint64_t bits) {
  std::bitset<64> set = bits;
  std::string answer;
  for (int i = 0; i < 8; i++) {
    std::string temp_answer;
    for (int j = 0; j < 8; j++) {
      temp_answer += std::to_string(set[(7 - i) * 8 + j]);
    }
    temp_answer += "\n";
    answer += temp_answer;
  }
  return answer;
}
uint64_t BBoard::wFrontFill(const uint64_t& wpawns) { return northFill(wpawns); }
uint64_t BBoard::wRearFill(const uint64_t& wpawns) { return soutFill(wpawns); }

uint64_t BBoard::bFrontFill(const uint64_t& bpawns) { return soutFill(bpawns); }
uint64_t BBoard::bRearFill(const uint64_t& bpawns) { return northFill(bpawns); }

uint64_t BBoard::wEastAttackFrontSpans(const uint64_t& wpawns) {
  return eastOne(wFrontSpans(wpawns));
}
uint64_t BBoard::wWestAttackFrontSpans(const uint64_t& wpawns) {
  return westOne(wFrontSpans(wpawns));
}
uint64_t BBoard::bEastAttackFrontSpans(const uint64_t& bpawns) {
  return eastOne(bFrontSpans(bpawns));
}
uint64_t BBoard::bWestAttackFrontSpans(const uint64_t& bpawns) {
  return westOne(bFrontSpans(bpawns));
}

uint64_t BBoard::wEastAttackRearSpans(const uint64_t& wpawns) {
  return eastOne(BBoard::wRearFill(wpawns));
}
uint64_t BBoard::wWestAttackRearSpans(const uint64_t& wpawns) {
  return westOne(BBoard::wRearFill(wpawns));
}
uint64_t BBoard::bEastAttackRearSpans(const uint64_t& bpawns) {
  return eastOne(BBoard::bRearFill(bpawns));
}
uint64_t BBoard::bWestAttackRearSpans(const uint64_t& bpawns) {
  return westOne(BBoard::bRearFill(bpawns));
}

uint64_t BBoard::eastAttacks(uint64_t rooks, const uint64_t& empty) {
  uint64_t flood = rooks;
  flood |= rooks = (rooks << 1) & (empty & notAFile);
  flood |= rooks = (rooks << 1) & (empty & notAFile);
  flood |= rooks = (rooks << 1) & (empty & notAFile);
  flood |= rooks = (rooks << 1) & (empty & notAFile);
  flood |= rooks = (rooks << 1) & (empty & notAFile);
  flood |= (rooks << 1) & (empty & notAFile);
  return (flood << 1) & notAFile;
}

uint64_t BBoard::noEaAttacks(uint64_t bishops, const uint64_t& empty) {
  uint64_t flood = bishops;
  flood |= bishops = (bishops << 9) & (empty & notAFile);
  flood |= bishops = (bishops << 9) & (empty & notAFile);
  flood |= bishops = (bishops << 9) & (empty & notAFile);
  flood |= bishops = (bishops << 9) & (empty & notAFile);
  flood |= bishops = (bishops << 9) & (empty & notAFile);
  flood |= (bishops << 9) & (empty & notAFile);
  return (flood << 9) & notAFile;
}

uint64_t BBoard::soutAttacks(uint64_t bishops, const uint64_t& empty) {
  uint64_t flood = bishops;
  flood |= bishops = (bishops >> 7) & (empty & notAFile);
  flood |= bishops = (bishops >> 7) & (empty & notAFile);
  flood |= bishops = (bishops >> 7) & (empty & notAFile);
  flood |= bishops = (bishops >> 7) & (empty & notAFile);
  flood |= bishops = (bishops >> 7) & (empty & notAFile);
  flood |= (bishops >> 7) & (empty & notAFile);
  return (flood >> 7) & notAFile;
}

uint64_t BBoard::nortAttacks(uint64_t rooks, const uint64_t& empty) {
  uint64_t flood = rooks;
  flood |= rooks = (rooks >> 1) & (empty & notHFile);
  flood |= rooks = (rooks >> 1) & (empty & notHFile);
  flood |= rooks = (rooks >> 1) & (empty & notHFile);
  flood |= rooks = (rooks >> 1) & (empty & notHFile);
  flood |= rooks = (rooks >> 1) & (empty & notHFile);
  flood |= (rooks >> 1) & (empty & notHFile);
  return (flood >> 1) & notHFile;
}

uint64_t BBoard::noWeAttacks(uint64_t bishops, const uint64_t& empty) {
  uint64_t flood = bishops;
  flood |= bishops = (bishops >> 9) & (empty & notHFile);
  flood |= bishops = (bishops >> 9) & (empty & notHFile);
  flood |= bishops = (bishops >> 9) & (empty & notHFile);
  flood |= bishops = (bishops >> 9) & (empty & notHFile);
  flood |= bishops = (bishops >> 9) & (empty & notHFile);
  flood |= (bishops >> 9) & (empty & notHFile);
  return (flood >> 9) & notHFile;
}

uint64_t BBoard::soEaAttacks(uint64_t bishops, const uint64_t& empty) {
  uint64_t flood = bishops;
  flood |= bishops = (bishops >> 7) & (empty & notHFile);
  flood |= bishops = (bishops >> 7) & (empty & notHFile);
  flood |= bishops = (bishops >> 7) & (empty & notHFile);
  flood |= bishops = (bishops >> 7) & (empty & notHFile);
  flood |= bishops = (bishops >> 7) & (empty & notHFile);
  flood |= (bishops >> 7) & (empty & notHFile);
  return (flood >> 7) & notAFile;
}

uint64_t BBoard::westAttacks(uint64_t rooks, const uint64_t& empty) {
  uint64_t flood = rooks;
  flood |= rooks = (rooks >> 1) & (empty & notHFile);
  flood |= rooks = (rooks >> 1) & (empty & notHFile);
  flood |= rooks = (rooks >> 1) & (empty & notHFile);
  flood |= rooks = (rooks >> 1) & (empty & notHFile);
  flood |= rooks = (rooks >> 1) & (empty & notHFile);
  flood |= (rooks >> 1) & (empty & notHFile);
  return (flood >> 1) & notHFile;
}

uint64_t BBoard::soWeAttacks(uint64_t bishops, const uint64_t& empty) {
  uint64_t flood = bishops;
  flood |= bishops = (bishops >> 9) & (empty & notHFile);
  flood |= bishops = (bishops >> 9) & (empty & notHFile);
  flood |= bishops = (bishops >> 9) & (empty & notHFile);
  flood |= bishops = (bishops >> 9) & (empty & notHFile);
  flood |= bishops = (bishops >> 9) & (empty & notHFile);
  flood |= (bishops >> 9) & (empty & notHFile);
  return (flood >> 9) & notHFile;
}
uint64_t BBoard::wSinglePushTargets(const uint64_t& wpawns,
                                    const uint64_t& empty) {
  return nortOne(wpawns) & empty;
}

uint64_t BBoard::wDblPushTargets(const uint64_t& wpawns,
                                 const uint64_t& empty) {
  return nortOne(wSinglePushTargets(wpawns, empty)) & empty & rank4;
}

uint64_t BBoard::bSinglePushTargets(const uint64_t& bpawns,
                                    const uint64_t& empty) {
  return soutOne(bpawns) & empty;
}

uint64_t BBoard::bDoublePushTargets(const uint64_t& bpawns,
                                    const uint64_t& empty) {
  uint64_t singlePushs = bSinglePushTargets(bpawns, empty);
  return soutOne(singlePushs) & empty & rank5;
}

uint64_t BBoard::bPawnsAble2Push(const uint64_t& bpawns,
                                 const uint64_t& empty) {
  return nortOne(empty) & bpawns;
}

uint64_t BBoard::bPawnsAble2DblPush(const uint64_t& bpawns,
                                    const uint64_t& empty) {
  return bPawnsAble2Push(bpawns, nortOne(empty & rank5) & empty);
}

uint64_t BBoard::wPawnsAble2Push(const uint64_t& wpawns,
                                 const uint64_t& empty) {
  return soutOne(empty) & wpawns;
}

uint64_t BBoard::wPawnsAble2DblPush(const uint64_t& wpawns,
                                    const uint64_t& empty) {
  return wPawnsAble2Push(wpawns, soutOne(empty & rank4) & empty);
}

uint64_t BBoard::wPawnEastAttacks(const uint64_t& wpawns) {
  return noEaOne(wpawns);
}
uint64_t BBoard::wPawnWestAttacks(const uint64_t& wpawns) {
  return noWeOne(wpawns);
}

uint64_t BBoard::bPawnEastAttacks(const uint64_t& bpawns) {
  return soEaOne(bpawns);
}
uint64_t BBoard::bPawnWestAttacks(const uint64_t& bpawns) {
  return soWeOne(bpawns);
}

uint64_t BBoard::wPawnsAble2CaptureEast(const uint64_t& wpawns,
                                        const uint64_t& bpieces) {
  return wpawns & bPawnWestAttacks(bpieces);
}

uint64_t BBoard::wPawnsAble2CaptureWest(const uint64_t& wpawns,
                                        const uint64_t& bpieces) {
  return wpawns & bPawnEastAttacks(bpieces);
}

uint64_t BBoard::bPawnsAble2CaptureEast(const uint64_t& bpawns,
                                        const uint64_t& wpieces) {
  return bpawns & wPawnWestAttacks(wpieces);
}

uint64_t BBoard::bPawnsAble2CaptureWest(const uint64_t& bpawns,
                                        const uint64_t& wpieces) {
  return bpawns & wPawnEastAttacks(wpieces);
}

uint64_t BBoard::noNoEa(const uint64_t& bit) {
  return (bit & notHFile) << 17;
}
uint64_t BBoard::noEaEa(const uint64_t& bit) {
  return (bit & notGHFile) << 10;
}
uint64_t BBoard::soEaEa(const uint64_t& bit) {
  return (bit & notGHFile) >> 6;
}
uint64_t BBoard::soSoEa(const uint64_t& bit) {
  return (bit & notHFile) >> 15;
}
uint64_t BBoard::noNoWe(const uint64_t& bit) {
  return (bit & notAFile) << 15;
}
uint64_t BBoard::noWeWe(const uint64_t& bit) {
  return (bit & notABFile) << 6;
}
uint64_t BBoard::soWeWe(const uint64_t& bit) {
  return (bit & notABFile) >> 10;
}
uint64_t BBoard::soSoWe(const uint64_t& bit) {
  return (bit & notAFile) >> 17;
}

uint64_t BBoard::knightAttacks(const uint64_t& knights) {
  uint64_t l1 = (knights >> 1) & uint64_t(0x7f7f7f7f7f7f7f7f);
  uint64_t l2 = (knights >> 2) & uint64_t(0x3f3f3f3f3f3f3f3f);
  uint64_t r1 = (knights << 1) & uint64_t(0xfefefefefefefefe);
  uint64_t r2 = (knights << 2) & uint64_t(0xfcfcfcfcfcfcfcfc);
  uint64_t h1 = l1 | r1;
  uint64_t h2 = l2 | r2;
  return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

uint64_t BBoard::forkTargetSquare(const uint64_t& targets) {
  uint64_t west, east, attak, forks;
  east = BBoard::eastOne(targets);
  west = BBoard::westOne(targets);
  attak = east << 16;
  forks = (west << 16) & attak;
  attak |= west << 16;
  forks |= (east >> 16) & attak;
  attak |= east >> 16;
  forks |= (west >> 16) & attak;
  attak |= west >> 16;
  east = BBoard::eastOne(east);
  west = BBoard::westOne(west);
  forks |= (east << 8) & attak;
  attak |= east << 8;
  forks |= (west << 8) & attak;
  attak |= west << 8;
  forks |= (east >> 8) & attak;
  attak |= east >> 8;
  forks |= (west >> 8) & attak;
  return forks;
}

uint64_t BBoard::kingAttacks(uint64_t kingSet) {
  uint64_t attacks = eastOne(kingSet) | westOne(kingSet);
  kingSet |= attacks;
  attacks |= nortOne(kingSet) | soutOne(kingSet);
  return attacks;
}
uint64_t BBoard::kingAttacks(uint8_t pos) {
  return king_attacks_[pos];

}

uint64_t BBoard::getPositiveRayAttacks(const uint64_t& occupied,
                                       enumDir dir8,
                                       int square) {
  uint64_t attacks = ray_attacks_[square][dir8];
  uint64_t blocker = attacks & occupied;
  if (blocker) {
    square = bitScanForward(blocker);
    attacks ^= ray_attacks_[square][dir8];
  }
  return attacks;
}

uint64_t BBoard::getNegativeRayAttacks(const uint64_t& occupied,
                                       enumDir dir8,
                                       int square) {
  uint64_t attacks = ray_attacks_[square][dir8];
  uint64_t blocker = attacks & occupied;
  if (blocker) {
    square = bitScanReverse(blocker);
    attacks ^= ray_attacks_[square][dir8];
  }
  return attacks;
}

uint64_t BBoard::getRayAttacks(const uint64_t& occupied,
                               enumDir dir8,
                               int square) {
  uint64_t attacks = ray_attacks_[square][dir8];
  uint64_t blocker = attacks & occupied;
  if (blocker) {
    square = bitScan(blocker, isNegative(dir8));
    attacks ^= ray_attacks_[square][dir8];
  }
  return attacks;
}

bool BBoard::isNegative(enumDir dir8) {
  return dir8 < 4;
}

uint64_t BBoard::diagonalAttacks(const uint64_t& occupied, int square) {
  return getPositiveRayAttacks(occupied, NoEa, square)
      | getNegativeRayAttacks(occupied, SoWe, square); // ^ +
}

uint64_t BBoard::antiDiagAttacks(const uint64_t& occupied, int square) {
  return getPositiveRayAttacks(occupied, NoWe, square)
      | getNegativeRayAttacks(occupied, SoEa, square); // ^ +
}

uint64_t BBoard::fileAttacks(const uint64_t& occupied, int square) {
  return getPositiveRayAttacks(occupied, Nort, square)
      | getNegativeRayAttacks(occupied, Sout, square); // ^ +
}

uint64_t BBoard::rankAttacks(const uint64_t& occupied, int square) {
  return getPositiveRayAttacks(occupied, East, square)
      | getNegativeRayAttacks(occupied, West, square); // ^ +
}
/*
uint64_t BitBoard::rookAttacks(const uint64_t& occupied, int square) {
  return fileAttacks( occupied, square)
      | rankAttacks( occupied, square); // ^ +
}


uint64_t BitBoard::bishopAttacks(const uint64_t& occupied, int square) {
  return diagonalAttacks( occupied, square)
      | antiDiagAttacks( occupied, square); // ^ +
}
*/
uint64_t BBoard::queenAttacks(uint64_t occupied, int square) {
  return rookAttacks(occupied, square)
      | bishopAttacks(occupied, square); // ^ +
}

uint64_t BBoard::bishopAttacks(uint64_t occupied, int sq) {
  return Magics::get_bishop_attacks(sq, occupied);
}

uint64_t BBoard::bishopDiagonals(int sq) {
  return bishop_diagonals_[sq];
}

uint64_t BBoard::rookDiagonals(int sq) {
  return rook_diagonals_[sq];
}

uint64_t BBoard::rookAttacks(uint64_t occupied, int sq) {
  return Magics::get_rook_attacks(sq, occupied);
}

void BBoard::staticDataInit() {
  if (_data_initialised) {
    return;
  }

  init_sliders_attacks(Magics::bishop);
  init_sliders_attacks(Magics::rook);

  _data_initialised = true;
  uint64_t nort = uint64_t(0x0101010101010100);
  uint64_t sout = uint64_t(0x0080808080808080);
  for (int square = 0; square < 64; square++, nort <<= 1) {
    ray_attacks_[square][Nort] = nort;
  }
  for (int square = 63; square >= 0; square--, sout >>= 1)
    ray_attacks_[square][Sout] = sout;

  const uint64_t one = 1;
  for (int square = 0; square < 64; square++) {
    ray_attacks_[square][East] = 2 * ((one << (square | 7)) - (one << square));
    ray_attacks_[square][West] = (one << square) - (one << (square & 56));;
  }
  uint64_t noea = uint64_t(0x8040201008040200);
  uint64_t nowe = mirrorHorizontal(uint64_t(0x8040201008040200));
  uint64_t sowe = mirrorVertical(nowe);
  uint64_t soea = mirrorHorizontal(sowe);
  for (int f = 0; f < 8; f++, noea = eastOne(noea)) {
    uint64_t ne = noea;
    for (int r8 = 0; r8 < 8 * 8; r8 += 8, ne <<= 8) {
      ray_attacks_[r8 + f][NoEa] = ne;
    }
  }

  for (int f = 7; f >= 0; f--, nowe = westOne(nowe)) {
    uint64_t nw = nowe;
    for (int r8 = 0; r8 < 8 * 8; r8 += 8, nw <<= 8) {
      ray_attacks_[r8 + f][NoWe] = nw;
    }
  }

  for (int f = 7; f >= 0; f--, sowe = westOne(sowe)) {
    uint64_t sw = sowe;
    for (int r8 = 8 * 8 - 8; r8 >= 0; r8 -= 8, sw >>= 8) {
      ray_attacks_[r8 + f][SoWe] = sw;
    }
  }
  for (int f = 0; f < 8; f++, soea = eastOne(soea)) {
    uint64_t sa = soea;
    for (int r8 = 8 * 8 - 8; r8 >= 0; r8 -= 8, sa >>= 8) {
      ray_attacks_[r8 + f][SoEa] = sa;
    }
  }

  for (int square = 63; square >= 0; square--) {
    one_square_[square] = uint64_t(1) << square;

    rank_attacks_[square] =
        ray_attacks_[square][East] | ray_attacks_[square][West];
    file_attacks_[square] =
        ray_attacks_[square][Nort] | ray_attacks_[square][Sout];
    diagonal_attacks_[square] =
        ray_attacks_[square][NoEa] | ray_attacks_[square][SoWe];
    anti_diagonal_attacks_[square] =
        ray_attacks_[square][NoWe] | ray_attacks_[square][SoEa];
    rook_diagonals_[square] = rank_attacks_[square] | file_attacks_[square];
    bishop_diagonals_[square] =
        diagonal_attacks_[square] | anti_diagonal_attacks_[square];
    queen_diagonals_[square] =
        rook_diagonals_[square] | bishop_diagonals_[square];
  }
  for (int square1 = 63; square1 >= 0; square1--) {
    for (int square2 = 63; square2 >= 0; square2--) {
      two_squares_[square1][square2] =
          one_square_[square1] | one_square_[square2];
    }
  }
  for (int f = 0; f < 8; f++) {
    for (int r8 = 0; r8 < 8; r8++) {
      auto pos = uint64_t(1) << (r8 + 8 * f);
      int index = f * 8 + r8;
      pawn_attacks_[BLACK_PIECES][index] =
          bPawnEastAttacks(pos) | bPawnWestAttacks(pos);
      pawn_attacks_[WHITE_PIECES][index] =
          wPawnEastAttacks(pos) | wPawnWestAttacks(pos);
      knight_attacks_[index] = knightAttacks(pos);
      king_attacks_[index] = kingAttacks(pos);
    }
  }
  for (int sq1 = 0; sq1 < 64; sq1++) {
    for (int sq2 = 0; sq2 < 64; sq2++) {
      const auto m1 = uint64_t(-1);
      const auto a2a7 = uint64_t(0x0001010101010100);
      const auto b2g7 = uint64_t(0x0040201008040200);
      const auto h1b7 =
          uint64_t(0x0002040810204080); /* Thanks Dustin, g2b7 did not work for c1-a3 */
      uint64_t btwn, line, rank, file;

      btwn = (m1 << sq1) ^ (m1 << sq2);
      file = (sq2 & 7) - (sq1 & 7);
      rank = ((sq2 | 7) - sq1) >> 3;
      line = ((file & 7) - 1) & a2a7; /* a2a7 if same file */
      line += 2 * (((rank & 7) - 1) >> 58); /* b1g1 if same rank */
      line += (((rank - file) & 15) - 1) & b2g7; /* b2g7 if same diagonal */
      line += (((rank + file) & 15) - 1) & h1b7; /* h1b7 if same antidiag */
      line *= btwn & -btwn; /* mul acts like shift by smaller square */
      arrRectangular[sq1][sq2] = line & btwn;
    }
  }
  right_castle_spaces[0] = one_square_[5] | one_square_[6];
  right_castle_spaces[1] = one_square_[61] | one_square_[62];
  left_castle_spaces[0] = one_square_[1] | one_square_[2] | one_square_[3];
  left_castle_spaces[1] = one_square_[57] | one_square_[58] | one_square_[59];
  for (int i = 0; i < 64; i++) {
    free_pawn_[0][i] = northFill(nortOne(one_square_[i]));
    free_pawn_[0][i] |= eastOne(northFill(nortOne(one_square_[i])));
    free_pawn_[0][i] |= westOne(northFill(nortOne(one_square_[i])));
  }
  for (int i = 0; i < 64; i++) {
    free_pawn_[1][i] = soutFill(soutOne(one_square_[i]));
    free_pawn_[1][i] |= eastOne(soutFill(soutOne(one_square_[i])));
    free_pawn_[1][i] |= westOne(soutFill(soutOne(one_square_[i])));
  }
  for (int i = 0; i < 64; i++) {
    pawn_road_[i] =
        soutFill(westOne(one_square_[i])) | northFill(westOne(one_square_[i]));
    pawn_road_[i] |=
        soutFill(eastOne(one_square_[i])) | northFill(eastOne(one_square_[i]));
  }
  after_castle_king_positions[0][0] =
      one_square_[0] | one_square_[1] | one_square_[2];
  after_castle_king_positions[0][0] |=
      nortOne(after_castle_king_positions[0][0]);
  defending_pawns[0][0] =
      nortOne(after_castle_king_positions[0][0]);

  after_castle_king_positions[0][1] =
      one_square_[7] | one_square_[6] | one_square_[5];
  after_castle_king_positions[0][1] |=
      nortOne(after_castle_king_positions[0][1]);
  defending_pawns[0][1] =
      nortOne(after_castle_king_positions[0][1]);

  after_castle_king_positions[1][0] =
      one_square_[56] | one_square_[57] | one_square_[58];
  after_castle_king_positions[1][0] |=
      soutOne(after_castle_king_positions[1][0]);
  defending_pawns[1][0] =
      soutOne(after_castle_king_positions[1][0]);

  after_castle_king_positions[1][1] =
      one_square_[61] | one_square_[62] | one_square_[63];
  after_castle_king_positions[1][1] |=
      soutOne(after_castle_king_positions[1][1]);
  defending_pawns[1][1] =
      soutOne(after_castle_king_positions[1][1]);

  good_castle_pawns = one_square_[8] | one_square_[9] | one_square_[10] |
      one_square_[13] | one_square_[48] | one_square_[54] |
      one_square_[14] | one_square_[49] | one_square_[53] |
      one_square_[15] | one_square_[50] | one_square_[55] ;
}

uint64_t BBoard::attacksTo(const uint64_t& occupied, int sq) {
  uint64_t knights, kings, bishopsQueens, rooksQueens;
  knights = pieceBB[WHITE_KNIGHT] | pieceBB[BLACK_NIGHT];
  kings = pieceBB[WHITE_KNIGHT] | pieceBB[BLACK_KING];
  rooksQueens =
  bishopsQueens = pieceBB[WHITE_QUEEN] | pieceBB[BLACK_QUEEN];
  rooksQueens |= pieceBB[WHITE_ROOK] | pieceBB[BLACK_ROOK];
  bishopsQueens |= pieceBB[WHITE_BISHOP] | pieceBB[BLACK_BISHOP];

  return (pawn_attacks_[WHITE_PIECES][sq] & pieceBB[BLACK_PAWN])
      | (pawn_attacks_[BLACK_PIECES][sq] & pieceBB[WHITE_PAWN])
      | (knight_attacks_[sq] & knights)
      | (king_attacks_[sq] & kings)
      | (bishopAttacks(occupied, sq) & bishopsQueens)
      | (rookAttacks(occupied, sq) & rooksQueens);
}

bool BBoard::attacked(const uint64_t& occupied,
                      int square,
                      const BPieceType& bySide) const {
  uint64_t figures = pieceBB[WHITE_PAWN + bySide];

  if (pawn_attacks_[!bySide][square] & figures) return true;
  figures = pieceBB[WHITE_KNIGHT + bySide];
  if (knight_attacks_[square] & figures) return true;
  figures = pieceBB[WHITE_QUEEN + bySide]
      | pieceBB[WHITE_BISHOP + bySide];
  if (bishopAttacks(occupied, square) & figures) return true;
  figures = pieceBB[WHITE_QUEEN + bySide]
      | pieceBB[WHITE_ROOK + bySide];
  if (rookAttacks(occupied, square) & figures) return true;
  figures = pieceBB[WHITE_KING + bySide];
  if (king_attacks_[square] & figures) return true;

  return false;
}

uint64_t BBoard::inBetween(int from, int to) {
  return arrRectangular[from][to];
}

bool BBoard::mayMove(int from, int to, const uint64_t& occupied) {
  return (inBetween(from, to) & occupied) == 0;
}

uint64_t BBoard::xrayFileAttacks(const uint64_t& occupied,
                                 uint64_t blockers,
                                 int rookSq) {
  uint64_t attacks = fileAttacks(occupied, rookSq);
  blockers &= attacks & uint64_t(0x00FFFFFFFFFFFF00);
  if (blockers == 0) return blockers;
  return attacks ^ fileAttacks(occupied ^ blockers, rookSq);
}

uint64_t BBoard::xrayRankAttacks(const uint64_t& occupied,
                                 uint64_t blockers,
                                 int rookSq) {
  uint64_t attacks = rankAttacks(occupied, rookSq);
  blockers &= attacks & uint64_t(0x7E7E7E7E7E7E7E7E);
  if (blockers == 0) return blockers;
  return attacks ^ rankAttacks(occupied ^ blockers, rookSq);
}

uint64_t BBoard::xrayDiagonalAttacks(const uint64_t& occupied,
                                     uint64_t blockers,
                                     int bishopSq) {
  uint64_t attacks = diagonalAttacks(occupied, bishopSq);
  blockers &= attacks & uint64_t(0x007E7E7E7E7E7E00);
  if (blockers == 0) return blockers;
  return attacks ^ diagonalAttacks(occupied ^ blockers, bishopSq);
}
uint64_t BBoard::attacksToKing(int squareOfKing,
                               const BPieceType& colorOfKing) const {
  uint64_t opPawns, opKnights, opRQ, opBQ;
  opPawns = pieceBB[BLACK_PAWN - colorOfKing];
  opKnights = pieceBB[BLACK_NIGHT - colorOfKing];
  opRQ = opBQ = pieceBB[BLACK_QUEEN - colorOfKing];
  opRQ |= pieceBB[BLACK_ROOK - colorOfKing];
  opBQ |= pieceBB[BLACK_BISHOP - colorOfKing];
  return (pawn_attacks_[colorOfKing][squareOfKing] & opPawns)
      | (knight_attacks_[squareOfKing] & opKnights)
      | (bishopAttacks(occupiedBB, squareOfKing) & opBQ)
      | (rookAttacks(occupiedBB, squareOfKing) & opRQ);
}

uint64_t BBoard::getLeastValuablePiece(uint64_t attadef,
                                       int bySide,
                                       int& piece) {
  for (piece = WHITE_PAWN + bySide; piece <= WHITE_KING + bySide; piece += 2) {
    uint64_t subset = attadef & pieceBB[piece];
    if (subset)
      return subset & -subset; // single bit
  }
  return 0; // empty set
}

//TODO  https://www.chessprogramming.org/SEE_-_The_Swap_Algorithm
int BBoard::see(int toSq,
                const BPieceType& target,
                int frSq,
                const BPieceType& aPiece) {/*
  int gain[32], d = 0;
  U64 mayXray = pawns | bishops | rooks | queen;
  U64 fromSet = 1ULL << frSq;
  U64 occ     = occupiedBB;
  U64 attadef = attacksTo( occ, toSq );
  gain[d]     = value[target];
  do {
    d++; // next depth and side
    gain[d]  = value[aPiece] - gain[d-1]; // speculative store, if defended
    if (max (-gain[d-1], gain[d]) < 0) break; // pruning does not influence the result
    attadef ^= fromSet; // reset bit in set to traverse
    occ     ^= fromSet; // reset bit in temporary occupancy (for x-Rays)
    if ( fromSet & mayXray )
      attadef |= considerXrays(occ, ..);
    fromSet  = getLeastValuablePiece (attadef, d & 1, aPiece);
  } while (fromSet);
  while (--d)
    gain[d-1]= -max (-gain[d-1], gain[d]);
  return gain[0];*/
  return 42;
}
const uint64_t& BBoard::getOccupied() const {
  return this->occupiedBB;
}
uint64_t BBoard::get(const BPieceType& white_piece_type,
                     const BPieceType& color) const {
  return pieceBB[white_piece_type + color];
}
uint64_t BBoard::knightAttacks(int pos) {
  return knight_attacks_[pos];
}
uint8_t BBoard::getMoveCount() const {
  return move_count_;
}
std::string BBoard::toStr() const {
  std::string answer;
  for (short y = 0; y <= 7; y++) {
    for (short x = 0; x <= 7; x++) {
      BPieceType type = getType((x) * 8 + (y));
      std::string on_pos;
      switch (type) {
        case WHITE_PIECES:on_pos = "_";
          break;
        case WHITE_PAWN:on_pos = "wp";
          break;
        case BLACK_PAWN:on_pos = "bp";
          break;
        case WHITE_KNIGHT:on_pos = "wn";
          break;
        case BLACK_NIGHT:on_pos = "bn";
          break;
        case WHITE_BISHOP:on_pos = "wb";
          break;
        case BLACK_BISHOP:on_pos = "bb";
          break;
        case WHITE_ROOK:on_pos = "wr";
          break;
        case BLACK_ROOK:on_pos = "br";
          break;
        case WHITE_QUEEN:on_pos = "wq";
          break;
        case BLACK_QUEEN:on_pos = "bq";
          break;
        case WHITE_KING:on_pos = "wk";
          break;
        case BLACK_KING:on_pos = "bk";
          break;
      }
      answer += on_pos + " ";

    }
    if (y != 7)
      answer += "\n";
  }
  return answer;
}

BBoard::BPieceType BBoard::getType(uint8_t pos) const {
  int answ = 0;
  for (int i = 2; i <= 13 && answ == 0; i++) {
    answ = BBoard::onPos(pos, get(static_cast<BPieceType>(i))) * i;
  }
  return static_cast<BPieceType>(answ);
}

BBoard::BPieceType BBoard::onPos(uint8_t pos, uint64_t bit) const {
  return static_cast<BPieceType>(((bit >> (((pos) % 8) + 8 * (pos / 8))) & 1));
}
const uint64_t& BBoard::getLastDoublePush() const {
  return file_attacks_[last_double_push_[move_count_]];
}

uint64_t BBoard::wPawnsAble2WestEP(uint64_t wpawns,
                                   const uint64_t& file) {
  wpawns &= rank5;
  wpawns &= (eastOne(file));
  return wpawns;
}

uint64_t BBoard::wPawnsAble2EastEP(uint64_t wpawns,
                                   const uint64_t& file) {
  wpawns &= rank5;
  wpawns &= (westOne(file));
  return wpawns;
}
uint64_t BBoard::bPawnsAble2EastEP(uint64_t bpawns,
                                   const uint64_t& file) {
  bpawns &= rank4;
  bpawns &= (westOne(file));
  return bpawns;
}

uint64_t BBoard::bPawnsAble2WestEP(uint64_t bpawns,
                                   const uint64_t& file) {
  bpawns &= rank4;
  bpawns &= (eastOne(file));
  return bpawns;
}
bool BBoard::isWhiteTurn() const {
  return is_white_turn_;
}
BBoard::BPieceType BBoard::whosTurn() const {
  return static_cast<BPieceType>(!isWhiteTurn());
}

bool BBoard::isShah(const BPieceType& whos_move) const {
  uint8_t king_pos = BBoard::bitScanForward(get(BBoard::WHITE_KING, whos_move));
  BPieceType enemy_side = static_cast<BPieceType>(!whos_move);
  return (attacked((getOccupied()),
                   king_pos,
                   enemy_side));

}

BBoard::BPieceType BBoard::getPiece(uint8_t square,
                                    const BPieceType& side) const {
  if (get(BPieceType::WHITE_PAWN, side) & one_square_[square]) {
    return static_cast<BPieceType>(BPieceType::WHITE_PAWN + side);
  }
  if (get(BPieceType::WHITE_KNIGHT, side) & one_square_[square]) {
    return static_cast<BPieceType>(BPieceType::WHITE_KNIGHT + side);
  }
  if (get(BPieceType::WHITE_BISHOP, side) & one_square_[square]) {
    return static_cast<BPieceType>(BPieceType::WHITE_BISHOP + side);
  }
  if (get(BPieceType::WHITE_ROOK, side) & one_square_[square]) {
    return static_cast<BPieceType>(BPieceType::WHITE_ROOK + side);
  }
  if (get(BPieceType::WHITE_QUEEN, side) & one_square_[square]) {
    return static_cast<BPieceType>(BPieceType::WHITE_QUEEN + side);
  }
  if (get(BPieceType::WHITE_KING, side) & one_square_[square]) {
    return static_cast<BPieceType>(BPieceType::WHITE_KING + side);
  }
  return BPieceType::WHITE_PIECES;
}

uint8_t BBoard::countPieces() const {
  return pieces_count_;
}

void BBoard::apply(const BMove& move) {
  uint8_t to = move.getTo();
  uint8_t from = move.getFrom();
  auto his_move = whosTurn();
  auto not_his_move = !whosTurn();
  move_count_++;
  is_white_turn_ = !is_white_turn_;

  pieceBB[his_move] ^= two_squares_[from][to];
  occupiedBB ^= two_squares_[from][to];

  auto my_piece = (int(move.getCurrentPieceType()) * 2 + his_move);
  if (move.isPromotion()) {
    pieceBB[my_piece] ^= one_square_[to];
    pieceBB[WHITE_PAWN + his_move] ^= one_square_[from];
  } else {
    pieceBB[my_piece] ^= two_squares_[from][to];
  }

  if (move.getFlags() == BMove::BFlagType::PAWN_MOVE
      && abs(to - from) == 16) {
    last_double_push_[move_count_] = from % 8;
  } else {
    last_double_push_[move_count_] = 64;
  }

  if (move.isCapture() || move.isPromotion()) {
    auto his_piece =
        getPiece(to, static_cast<BPieceType>(not_his_move));
    setPieceCapturedThisMove(his_piece);

    if (!move.isPromotion() || his_piece != WHITE_PIECES) {
      pieces_count_--;
      if (his_piece == WHITE_PIECES) { // EPASSANT !!!!
        if (isWhiteTurn()) {
          to += 8;
        } else {
          to -= 8;
        }
        his_piece = static_cast<BPieceType>(WHITE_PAWN + not_his_move);
      }
      pieceBB[not_his_move] ^= one_square_[to];
      pieceBB[his_piece] ^= one_square_[to];
      occupiedBB ^= one_square_[to];
    }
  }

  setPrevLeftCastle(left_castle_[his_move]);
  setPrevRightCastle(right_castle_[his_move]);
  // CASTLE
  if (isCastle(move)) {
    if (move.getTo() + 2 == move.getFrom()) {
      if (!isWhiteTurn()) {
        // left white
        pieceBB[WHITE_PIECES] ^= two_squares_[0][3];
        pieceBB[WHITE_ROOK] ^= two_squares_[0][3];
        occupiedBB ^= two_squares_[0][3];

      } else {
        // left black
        pieceBB[BLACK_PIECES] ^= two_squares_[56][59];
        pieceBB[BLACK_ROOK] ^= two_squares_[56][59];
        occupiedBB ^= two_squares_[56][59];
      }
    } else {
      if (!isWhiteTurn()) {
        // right white
        pieceBB[WHITE_PIECES] ^= two_squares_[7][5];
        pieceBB[WHITE_ROOK] ^= two_squares_[7][5];
        occupiedBB ^= two_squares_[7][5];
      } else {
        // right black
        pieceBB[BLACK_PIECES] ^= two_squares_[63][61];
        pieceBB[BLACK_ROOK] ^= two_squares_[63][61];
        occupiedBB ^= two_squares_[63][61];
      }
    }
    left_castle_[his_move] = false;
    right_castle_[his_move] = false;
  } else {
    if (isBrakeLeftCastle(move, his_move)) {
      left_castle_[his_move] = false;
    }
    if (isBrakeRightCastle(move, his_move)) {
      right_castle_[his_move] = false;
    }
  }
}

void BBoard::unApply(const BMove& move) {
  uint8_t to = move.getTo();
  uint8_t from = move.getFrom();
  auto his_move = !whosTurn();
  auto not_his_move = whosTurn();
  // i am the player whos move is retirning
  // place back piece in my board
  pieceBB[his_move] ^= two_squares_[from][to];
  occupiedBB ^= two_squares_[from][to];

  auto my_piece = (int(move.getCurrentPieceType()) * 2 + his_move);

  if (move.isPromotion()) {
    pieceBB[my_piece] ^= one_square_[to];
    pieceBB[WHITE_PAWN + his_move] ^= one_square_[from];
  } else {
    pieceBB[my_piece] ^= two_squares_[from][to];
  }

  if (move.isCapture() || move.isPromotion()) {
    BPieceType his_piece = getPieceCapturedThisMove();
    if (!move.isPromotion() || his_piece != WHITE_PIECES) {

      pieces_count_++;
      if (his_piece == WHITE_PIECES) { // EPASSANT !!!!
        if (isWhiteTurn()) {
          to += 8;
        } else {
          to -= 8;
        }
        his_piece = static_cast<BPieceType>(WHITE_PAWN + not_his_move);
      }
      pieceBB[not_his_move] ^= one_square_[to];
      pieceBB[his_piece] ^= one_square_[to];
      occupiedBB ^= one_square_[to];
    }
  }

  // CASTLE
  if (isCastle(move)) {
    if (move.getTo() + 2 == move.getFrom()) {
      if (!isWhiteTurn()) {
        // left white
        pieceBB[WHITE_PIECES] ^= two_squares_[0][3];
        pieceBB[WHITE_ROOK] ^= two_squares_[0][3];
        occupiedBB ^= two_squares_[0][3];
      } else {
        // left black
        pieceBB[BLACK_PIECES] ^= two_squares_[56][59];
        pieceBB[BLACK_ROOK] ^= two_squares_[56][59];
        occupiedBB ^= two_squares_[56][59];
      }
    } else {
      if (!isWhiteTurn()) {
        // right white
        pieceBB[WHITE_PIECES] ^= two_squares_[7][5];
        pieceBB[WHITE_ROOK] ^= two_squares_[7][5];
        occupiedBB ^= two_squares_[7][5];
      } else {
        // right black
        pieceBB[BLACK_PIECES] ^= two_squares_[63][61];
        pieceBB[BLACK_ROOK] ^= two_squares_[63][61];
        occupiedBB ^= two_squares_[63][61];
      }
    }
  }

  left_castle_[his_move] = getPrevLeftCastle();
  right_castle_[his_move] = getPrevRightCastle();

  move_count_--;
  is_white_turn_ = !is_white_turn_;
}

bool BBoard::isCastle(const BMove& move) const {
  return move.getIsFlagSet(BMove::KING_MOVE)
      && ((move.getFrom() - move.getTo() == 2)
          || (move.getFrom() - move.getTo() == -2));
}

bool BBoard::isBrakeCastle(const BMove& move, const bool& side) const {
  return !isCastle(move)
      && (isBrakeLeftCastle(move, side) || isBrakeRightCastle(move, side));
}

bool BBoard::isBrakeLeftCastle(const BMove& move, const bool& side) const {
  if (isCanLeftCastle(side)) {
    return move.getIsFlagSet(BMove::KING_MOVE)
        || move.getIsFlagSet(BMove::CAPTURE_BY_KING) ||
        (move.getFrom() % 8 == 0 && (move.getIsFlagSet(BMove::RUCK_MOVE)
            || move.getIsFlagSet(BMove::CAPTURE_BY_RUCK)));
  }
  return false;
}
bool BBoard::isBrakeRightCastle(const BMove& move, const bool& side) const {
  if (isCanRightCastle(side)) {
    return move.getIsFlagSet(BMove::KING_MOVE)
        || move.getIsFlagSet(BMove::CAPTURE_BY_KING) ||
        (move.getFrom() % 8 == 7 && (move.getIsFlagSet(BMove::RUCK_MOVE)
            || move.getIsFlagSet(BMove::CAPTURE_BY_RUCK)));
  }
  return false;
}
bool BBoard::isCanLeftCastle(const bool& side) const {
  return left_castle_[side];
}
bool BBoard::isCanRightCastle(const bool& side) const {
  return right_castle_[side];
}
void BBoard::setPieceCapturedThisMove(const BPieceType& type) {
  piece_captured_this_move_[move_count_] = type;
}
BBoard::BPieceType BBoard::getPieceCapturedThisMove() {
  return piece_captured_this_move_[move_count_];
}

bool BBoard::operator==(const BBoard& other) {
  for (int i = 0; i < 14; i++) {
    if (pieceBB[i] != other.pieceBB[i]) {
      std::cout << "pieces " << i;

      return false;
    }
  }
  if (occupiedBB != other.occupiedBB) {
    std::cout << "occcupied";
    return false;
  }

  if (pieces_count_ != other.pieces_count_) {
    std::cout << "piece_count";
    return false;
  }
  if (move_count_ != other.move_count_) {
    std::cout << "move_count";
    return false;
  }
  if (is_white_turn_ != other.is_white_turn_) {
    std::cout << "white_turn";
    return false;
  }
  if (occupiedBB != other.occupiedBB) {
    std::cout << "occupied";
    return false;
  }
  if (left_castle_[0] != other.left_castle_[0]) {
    std::cout << "white_lc";
    return false;
  }
  if (left_castle_[1] != other.left_castle_[1]) {
    std::cout << "black_lc";
    return false;
  }
  if (right_castle_[1] != other.right_castle_[1]) {
    std::cout << "black_rc";
    return false;
  }
  if (right_castle_[0] != other.right_castle_[0]) {
    std::cout << "black_lc";
    return false;
  }
  return true;
}
uint64_t BBoard::queenDiagonal(uint8_t pos) {
  return queen_diagonals_[pos];
}

bool BBoard::canLeftCastle() const {
  return left_castle_[whosTurn()]
      && ((left_castle_spaces[whosTurn()] & occupiedBB) == 0)
      && (pieceBB[WHITE_ROOK + whosTurn()] & one_square_[56 * whosTurn()]);
}
bool BBoard::canRightCastle() const {
  return right_castle_[whosTurn()]
      && ((right_castle_spaces[whosTurn()] & occupiedBB) == 0)
      && (pieceBB[WHITE_ROOK + whosTurn()] & one_square_[7 + 56 * whosTurn()]);
}
void BBoard::setPrevLeftCastle(bool b) {
  last_left_castle[move_count_] = b;
}
void BBoard::setPrevRightCastle(bool b) {
  last_right_castle[move_count_] = b;
}
bool BBoard::getPrevLeftCastle() {
  return last_left_castle[move_count_];
}
bool BBoard::getPrevRightCastle() {
  return last_right_castle[move_count_];
}
bool BBoard::isPassedPawn(uint8_t i, bool b) const {
  return (free_pawn_[b][i]
      & get(static_cast<BPieceType>(BLACK_PAWN - b))) == 0;
}

bool BBoard::isolatedPawn(uint8_t from, uint8_t to) const {
  return ((get(WHITE_PAWN, whosTurn()) ^ one_square_[from]) & pawn_road_[to])
      == 0;
}

bool BBoard::doublePawn(uint8_t from, uint8_t to) const {
  return ((get(WHITE_PAWN, whosTurn()) ^ one_square_[from]) & file_attacks_[to])
      != 0;
}
bool BBoard::isDefendKingPawn(uint8_t pos) const {
  if ((get(WHITE_KING, whosTurn())
      & after_castle_king_positions[whosTurn()][0])) {
    return (one_square_[pos] & defending_pawns[whosTurn()][0]) != 0;
  }
  if ((get(WHITE_KING, whosTurn())
      & after_castle_king_positions[whosTurn()][1])) {
    return (one_square_[pos] & defending_pawns[whosTurn()][1]) != 0;
  }
  return false; // no one to defend
}
bool BBoard::isAttackKingPawn(uint8_t pos) const {
  return (free_pawn_[whosTurn()][pos]
      & get(BLACK_KING, static_cast<BPieceType>(!whosTurn()))) != 0;
}

uint8_t BBoard::kingPression(BPieceType enemy_color) {
  uint8_t king_pos =
      bitScanForward(get(WHITE_KING,enemy_color ));
  uint64_t king_nearby = kingAttacks(king_pos) | one_square_[king_pos];
  uint8_t ans = 0;
  while (king_nearby) {
    king_pos =
        BBoard::bitScanForward(king_nearby);
    king_nearby &= king_nearby - 1;
    ans += attacked(pieceBB[!enemy_color], king_pos, static_cast<BPieceType>(!enemy_color));
  }
  return ans;
}
bool BBoard::goodCastle(uint8_t pos_from) {
  return ((knight_attacks_[bitScanForward( eastOne(one_square_[pos_from])
      | westOne(one_square_[pos_from]))]
      & (pieceBB[WHITE_PAWN] | pieceBB[BLACK_PAWN])) | good_castle_pawns) != 0;
}
