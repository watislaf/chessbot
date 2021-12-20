#include "bitBoard.h"
#include "magicsWrapper.cpp"

bool  BitBoard::_data_initialised = false;
const int BitBoard::index64[64] = {
    0, 47, 1, 56, 48, 27, 2, 60,
    57, 49, 41, 37, 28, 16, 3, 61,
    54, 58, 35, 52, 50, 42, 21, 44,
    38, 32, 29, 23, 17, 11, 4, 62,
    46, 55, 26, 59, 40, 36, 15, 53,
    34, 51, 20, 43, 31, 22, 10, 45,
    25, 39, 14, 33, 19, 30, 9, 24,
    13, 18, 8, 12, 7, 6, 5, 63
};

BitBoard::BitBoard(FEN fen) {
  for (int i = 0; i < 14; i++) {
    pieceBB[i] = 0;
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      auto piece = fen.getPiece(i, j);
      if (piece.getType() == PieceType::tNONE) {
        continue;
      }
      pieceBB[static_cast<int>(piece.getPieceColor())] |=
          (u_int64_t(1) << ((7 - i) * 8 + j));
      pieceBB[static_cast<int>(piece.getType()) * 2
          + static_cast<int>(piece.getPieceColor())]
          |= (u_int64_t(1) << ((7 - i) * 8 + j));
    }
  }
  occupiedBB = pieceBB[0] | pieceBB[1];
  emptyBB = ~occupiedBB;
  staticDataInit();
}

int BitBoard::bitScanReverse(uint64_t a) {
  unsigned long int nlb;
  asm (
  "BSR %[a], %[nlb]"
  : [nlb] "=r"(nlb)
  : [a] "mr"(a)
  );
  return nlb;
}
int BitBoard::bitScanForward(uint64_t a) {
  unsigned long int nlb;
  asm (
  "BSF %[a], %[nlb]"
  : [nlb] "=r"(nlb)
  : [a] "mr"(a)
  );
  return nlb;
}

int BitBoard::bitScan(uint64_t bb, bool reverse) {
  uint64_t rMask;
  rMask = -(uint64_t) reverse;
  bb &= -bb | rMask;
  return bitScanReverse(bb);
}
int BitBoard::countBits(uint64_t set) {
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

uint64_t BitBoard::soutOne(uint64_t bit) {
  return bit >> 8;
}
uint64_t BitBoard::nortOne(uint64_t bit) {
  return bit << 8;
}
uint64_t BitBoard::eastOne(uint64_t bit) {
  return (bit & notHFile) << 1;
}
uint64_t BitBoard::noEaOne(uint64_t bit) {
  return (bit & notHFile) << 9;
}
uint64_t BitBoard::soEaOne(uint64_t bit) {
  return (bit & notHFile) >> 7;
}
uint64_t BitBoard::westOne(uint64_t bit) {
  return (bit & notAFile) >> 1;
}
uint64_t BitBoard::soWeOne(uint64_t bit) {
  return (bit & notAFile) >> 9;
}
uint64_t BitBoard::noWeOne(uint64_t bit) {
  return (bit & notAFile) << 7;
}

uint64_t BitBoard::mirrorHorizontal(uint64_t bit) {
  const auto k1 = uint64_t(0x5555555555555555);
  const auto k2 = uint64_t(0x3333333333333333);
  const auto k4 = uint64_t(0x0f0f0f0f0f0f0f0f);
  bit = ((bit >> 1) & k1) | ((bit & k1) << 1);
  bit = ((bit >> 2) & k2) | ((bit & k2) << 2);
  bit = ((bit >> 4) & k4) | ((bit & k4) << 4);
  return bit;
}
uint64_t BitBoard::mirrorVertical(uint64_t bit) {
  const auto k1 = uint64_t(0x00FF00FF00FF00FF);
  const auto k2 = uint64_t(0x0000FFFF0000FFFF);
  bit = ((bit >> 8) & k1) | ((bit & k1) << 8);
  bit = ((bit >> 16) & k2) | ((bit & k2) << 16);
  bit = (bit >> 32) | (bit << 32);
  return bit;
}
uint64_t BitBoard::northFill(uint64_t gen) {
  gen |= (gen << 8);
  gen |= (gen << 16);
  gen |= (gen << 32);
  return gen;
}

uint64_t BitBoard::soutFill(uint64_t gen) {
  gen |= (gen >> 8);
  gen |= (gen >> 16);
  gen |= (gen >> 32);
  return gen;
}

uint64_t BitBoard::fileFill(uint64_t bit) {
  return soutFill(bit) | northFill(bit);
}

uint64_t BitBoard::wFrontSpans(uint64_t wpawns) {
  return nortOne(northFill(wpawns));
}

uint64_t BitBoard::bRearSpans(uint64_t bpawns) {
  return nortOne(northFill(bpawns));
}
uint64_t BitBoard::bFrontSpans(uint64_t bpawns) {
  return soutOne(soutFill(bpawns));
}
uint64_t BitBoard::wRearSpans(uint64_t wpawns) {
  return soutOne(soutFill(wpawns));
}
std::string BitBoard::toStr(uint64_t bits) {
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
uint64_t BitBoard::wFrontFill(uint64_t wpawns) { return northFill(wpawns); }
uint64_t BitBoard::wRearFill(uint64_t wpawns) { return soutFill(wpawns); }

uint64_t BitBoard::bFrontFill(uint64_t bpawns) { return soutFill(bpawns); }
uint64_t BitBoard::bRearFill(uint64_t bpawns) { return northFill(bpawns); }

uint64_t BitBoard::wEastAttackFrontSpans(uint64_t wpawns) {
  return eastOne(wFrontSpans(wpawns));
}
uint64_t BitBoard::wWestAttackFrontSpans(uint64_t wpawns) {
  return westOne(wFrontSpans(wpawns));
}
uint64_t BitBoard::bEastAttackFrontSpans(uint64_t bpawns) {
  return eastOne(bFrontSpans(bpawns));
}
uint64_t BitBoard::bWestAttackFrontSpans(uint64_t bpawns) {
  return westOne(bFrontSpans(bpawns));
}

uint64_t BitBoard::wEastAttackRearSpans(uint64_t wpawns) {
  return eastOne(BitBoard::wRearFill(wpawns));
}
uint64_t BitBoard::wWestAttackRearSpans(uint64_t wpawns) {
  return westOne(BitBoard::wRearFill(wpawns));
}
uint64_t BitBoard::bEastAttackRearSpans(uint64_t bpawns) {
  return eastOne(BitBoard::bRearFill(bpawns));
}
uint64_t BitBoard::bWestAttackRearSpans(uint64_t bpawns) {
  return westOne(BitBoard::bRearFill(bpawns));
}

uint64_t BitBoard::eastAttacks(uint64_t rooks, uint64_t empty) {
  uint64_t flood = rooks;
  empty &= notAFile;
  flood |= rooks = (rooks << 1) & empty;
  flood |= rooks = (rooks << 1) & empty;
  flood |= rooks = (rooks << 1) & empty;
  flood |= rooks = (rooks << 1) & empty;
  flood |= rooks = (rooks << 1) & empty;
  flood |= (rooks << 1) & empty;
  return (flood << 1) & notAFile;
}

uint64_t BitBoard::noEaAttacks(uint64_t bishops, uint64_t empty) {
  uint64_t flood = bishops;
  empty &= notAFile;
  flood |= bishops = (bishops << 9) & empty;
  flood |= bishops = (bishops << 9) & empty;
  flood |= bishops = (bishops << 9) & empty;
  flood |= bishops = (bishops << 9) & empty;
  flood |= bishops = (bishops << 9) & empty;
  flood |= (bishops << 9) & empty;
  return (flood << 9) & notAFile;
}

uint64_t BitBoard::soutAttacks(uint64_t bishops, uint64_t empty) {
  uint64_t flood = bishops;
  empty &= notAFile;
  flood |= bishops = (bishops >> 7) & empty;
  flood |= bishops = (bishops >> 7) & empty;
  flood |= bishops = (bishops >> 7) & empty;
  flood |= bishops = (bishops >> 7) & empty;
  flood |= bishops = (bishops >> 7) & empty;
  flood |= (bishops >> 7) & empty;
  return (flood >> 7) & notAFile;
}

uint64_t BitBoard::nortAttacks(uint64_t rooks, uint64_t empty) {
  uint64_t flood = rooks;
  empty &= notHFile;
  flood |= rooks = (rooks >> 1) & empty;
  flood |= rooks = (rooks >> 1) & empty;
  flood |= rooks = (rooks >> 1) & empty;
  flood |= rooks = (rooks >> 1) & empty;
  flood |= rooks = (rooks >> 1) & empty;
  flood |= (rooks >> 1) & empty;
  return (flood >> 1) & notHFile;
}

uint64_t BitBoard::noWeAttacks(uint64_t bishops, uint64_t empty) {
  uint64_t flood = bishops;
  empty &= notHFile;
  flood |= bishops = (bishops >> 9) & empty;
  flood |= bishops = (bishops >> 9) & empty;
  flood |= bishops = (bishops >> 9) & empty;
  flood |= bishops = (bishops >> 9) & empty;
  flood |= bishops = (bishops >> 9) & empty;
  flood |= (bishops >> 9) & empty;
  return (flood >> 9) & notHFile;
}

uint64_t BitBoard::soEaAttacks(uint64_t bishops, uint64_t empty) {
  uint64_t flood = bishops;
  empty &= notAFile;
  flood |= bishops = (bishops >> 7) & empty;
  flood |= bishops = (bishops >> 7) & empty;
  flood |= bishops = (bishops >> 7) & empty;
  flood |= bishops = (bishops >> 7) & empty;
  flood |= bishops = (bishops >> 7) & empty;
  flood |= (bishops >> 7) & empty;
  return (flood >> 7) & notAFile;
}

uint64_t BitBoard::westAttacks(uint64_t rooks, uint64_t empty) {
  uint64_t flood = rooks;
  empty &= notHFile;
  flood |= rooks = (rooks >> 1) & empty;
  flood |= rooks = (rooks >> 1) & empty;
  flood |= rooks = (rooks >> 1) & empty;
  flood |= rooks = (rooks >> 1) & empty;
  flood |= rooks = (rooks >> 1) & empty;
  flood |= (rooks >> 1) & empty;
  return (flood >> 1) & notHFile;
}

uint64_t BitBoard::soWeAttacks(uint64_t bishops, uint64_t empty) {
  uint64_t flood = bishops;
  empty &= notHFile;
  flood |= bishops = (bishops >> 9) & empty;
  flood |= bishops = (bishops >> 9) & empty;
  flood |= bishops = (bishops >> 9) & empty;
  flood |= bishops = (bishops >> 9) & empty;
  flood |= bishops = (bishops >> 9) & empty;
  flood |= (bishops >> 9) & empty;
  return (flood >> 9) & notHFile;
}
uint64_t BitBoard::wSinglePushTargets(uint64_t wpawns, uint64_t empty) {
  return nortOne(wpawns) & empty;
}

uint64_t BitBoard::wDblPushTargets(uint64_t wpawns, uint64_t empty) const {
  uint64_t singlePushs = wSinglePushTargets(wpawns, empty);
  return nortOne(singlePushs) & empty & rank4;
}

uint64_t BitBoard::bSinglePushTargets(uint64_t bpawns, uint64_t empty) {
  return soutOne(bpawns) & empty;
}

uint64_t BitBoard::bDoublePushTargets(uint64_t bpawns, uint64_t empty) const {
  uint64_t singlePushs = bSinglePushTargets(bpawns, empty);
  return soutOne(singlePushs) & empty & rank5;
}

uint64_t BitBoard::wPawnsAble2Push(uint64_t wpawns, uint64_t empty) {
  return soutOne(empty) & wpawns;
}

uint64_t BitBoard::wPawnsAble2DblPush(uint64_t wpawns, uint64_t empty) const {
  uint64_t emptyRank3 = soutOne(empty & rank4) & empty;
  return wPawnsAble2Push(wpawns, emptyRank3);
}

uint64_t BitBoard::wPawnEastAttacks(uint64_t wpawns) {
  return noEaOne(wpawns);
}
uint64_t BitBoard::wPawnWestAttacks(uint64_t wpawns) {
  return noWeOne(wpawns);
}

uint64_t BitBoard::bPawnEastAttacks(uint64_t bpawns) {
  return soEaOne(bpawns);
}
uint64_t BitBoard::bPawnWestAttacks(uint64_t bpawns) {
  return soWeOne(bpawns);
}

uint64_t BitBoard::wPawnsAble2CaptureEast(uint64_t wpawns, uint64_t bpieces) {
  return wpawns & bPawnWestAttacks(bpieces);
}

uint64_t BitBoard::wPawnsAble2CaptureWest(uint64_t wpawns, uint64_t bpieces) {
  return wpawns & bPawnEastAttacks(bpieces);
}

uint64_t BitBoard::bPawnsAble2CaptureEast(uint64_t bpawns, uint64_t wpieces) {
  return bpawns & bPawnWestAttacks(wpieces);
}

uint64_t BitBoard::bPawnsAble2CaptureWest(uint64_t bpawns, uint64_t wpieces) {
  return bpawns & bPawnEastAttacks(wpieces);
}

uint64_t BitBoard::noNoEa(uint64_t b) {
  return (b & notHFile) << 17;
}
uint64_t BitBoard::noEaEa(uint64_t b) {
  return (b & notGHFile) << 10;
}
uint64_t BitBoard::soEaEa(uint64_t b) {
  return (b & notGHFile) >> 6;
}
uint64_t BitBoard::soSoEa(uint64_t b) {
  return (b & notHFile) >> 15;
}
uint64_t BitBoard::noNoWe(uint64_t b) {
  return (b & notAFile) << 15;
}
uint64_t BitBoard::noWeWe(uint64_t b) {
  return (b & notABFile) << 6;
}
uint64_t BitBoard::soWeWe(uint64_t b) {
  return (b & notABFile) >> 10;
}
uint64_t BitBoard::soSoWe(uint64_t b) {
  return (b & notAFile) >> 17;
}

uint64_t BitBoard::knightAttacks(uint64_t knights) {
  uint64_t l1 = (knights >> 1) & uint64_t(0x7f7f7f7f7f7f7f7f);
  uint64_t l2 = (knights >> 2) & uint64_t(0x3f3f3f3f3f3f3f3f);
  uint64_t r1 = (knights << 1) & uint64_t(0xfefefefefefefefe);
  uint64_t r2 = (knights << 2) & uint64_t(0xfcfcfcfcfcfcfcfc);
  uint64_t h1 = l1 | r1;
  uint64_t h2 = l2 | r2;
  return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

uint64_t BitBoard::forkTargetSquare(uint64_t targets) {
  uint64_t west, east, attak, forks;
  east = BitBoard::eastOne(targets);
  west = BitBoard::westOne(targets);
  attak = east << 16;
  forks = (west << 16) & attak;
  attak |= west << 16;
  forks |= (east >> 16) & attak;
  attak |= east >> 16;
  forks |= (west >> 16) & attak;
  attak |= west >> 16;
  east = BitBoard::eastOne(east);
  west = BitBoard::westOne(west);
  forks |= (east << 8) & attak;
  attak |= east << 8;
  forks |= (west << 8) & attak;
  attak |= west << 8;
  forks |= (east >> 8) & attak;
  attak |= east >> 8;
  forks |= (west >> 8) & attak;
  return forks;
}

uint64_t BitBoard::kingAttacks(uint64_t kingSet) {
  uint64_t attacks = eastOne(kingSet) | westOne(kingSet);
  kingSet |= attacks;
  attacks |= nortOne(kingSet) | soutOne(kingSet);
  return attacks;
}

uint64_t BitBoard::getPositiveRayAttacks(uint64_t obstacle,
                                         enumDir dir8,
                                         int square) {
  uint64_t attacks = rayAttacks[square][dir8];
  uint64_t blocker = attacks & obstacle;
  if (blocker) {
    square = bitScanForward(blocker);
    attacks ^= rayAttacks[square][dir8];
  }
  return attacks;
}

uint64_t BitBoard::getNegativeRayAttacks(uint64_t obstacle,
                                         enumDir dir8,
                                         int square) {
  uint64_t attacks = rayAttacks[square][dir8];
  uint64_t blocker = attacks & obstacle;
  if (blocker) {
    square = bitScanReverse(blocker);
    attacks ^= rayAttacks[square][dir8];
  }
  return attacks;
}

uint64_t BitBoard::getRayAttacks(uint64_t obstacle,
                                 enumDir dir8,
                                 int square) {
  uint64_t attacks = rayAttacks[square][dir8];
  uint64_t blocker = attacks & obstacle;
  if (blocker) {
    square = bitScan(blocker, isNegative(dir8));
    attacks ^= rayAttacks[square][dir8];
  }
  return attacks;
}

bool BitBoard::isNegative(enumDir dir8) {
  return dir8 < 4;
}

uint64_t BitBoard::diagonalAttacks(uint64_t obstacle, int square) {
  return getPositiveRayAttacks(obstacle, NoEa, square)
      | getNegativeRayAttacks(obstacle, SoWe, square); // ^ +
}

uint64_t BitBoard::antiDiagAttacks(uint64_t obstacle, int square) {
  return getPositiveRayAttacks(obstacle, NoWe, square)
      | getNegativeRayAttacks(obstacle, SoEa, square); // ^ +
}

uint64_t BitBoard::fileAttacks(uint64_t obstacle, int square) {
  return getPositiveRayAttacks(obstacle, Nort, square)
      | getNegativeRayAttacks(obstacle, Sout, square); // ^ +
}

uint64_t BitBoard::rankAttacks(uint64_t obstacle, int square) {
  return getPositiveRayAttacks(obstacle, East, square)
      | getNegativeRayAttacks(obstacle, West, square); // ^ +
}
/*
uint64_t BitBoard::rookAttacks(uint64_t obstacle, int square) {
  return fileAttacks(obstacle, square)
      | rankAttacks(obstacle, square); // ^ +
}


uint64_t BitBoard::bishopAttacks(uint64_t obstacle, int square) {
  return diagonalAttacks(obstacle, square)
      | antiDiagAttacks(obstacle, square); // ^ +
}
*/
uint64_t BitBoard::queenAttacks(uint64_t obstacle, int square) {
  return rookAttacks(obstacle, square)
      | bishopAttacks(obstacle, square); // ^ +
}

uint64_t BitBoard::bishopAttacks(uint64_t occ, int sq) {
  return Magics::get_bishop_attacks(Magics::d4, occ);
}

uint64_t BitBoard::rookAttacks(uint64_t occ, int sq) {
  return Magics::get_rook_attacks(Magics::d4, occ);
}

void BitBoard::staticDataInit() {
  if (_data_initialised) {
    return;
  }
  init_sliders_attacks(Magics::bishop);
  init_sliders_attacks(Magics::rook);

  _data_initialised = true;
  uint64_t nort = uint64_t(0x0101010101010100);
  uint64_t sout = uint64_t(0x0080808080808080);
  for (int square = 0; square < 64; square++, nort <<= 1) {
    rayAttacks[square][Nort] = nort;
  }
  for (int square = 63; square >= 0; square--, sout >>= 1)
    rayAttacks[square][Sout] = sout;

  const uint64_t one = 1;
  for (int square = 0; square < 64; square++) {
    rayAttacks[square][East] = 2 * ((one << (square | 7)) - (one << square));
    rayAttacks[square][West] = (one << square) - (one << (square & 56));;
  }
  uint64_t noea = uint64_t(0x8040201008040200);
  uint64_t nowe = mirrorHorizontal(uint64_t(0x8040201008040200));
  uint64_t sowe = mirrorVertical(nowe);
  uint64_t soea = mirrorHorizontal(sowe);
  for (int f = 0; f < 8; f++, noea = eastOne(noea)) {
    uint64_t ne = noea;
    for (int r8 = 0; r8 < 8 * 8; r8 += 8, ne <<= 8) {
      rayAttacks[r8 + f][NoEa] = ne;
    }
  }

  for (int f = 7; f >= 0; f--, nowe = westOne(nowe)) {
    uint64_t nw = nowe;
    for (int r8 = 0; r8 < 8 * 8; r8 += 8, nw <<= 8) {
      rayAttacks[r8 + f][NoWe] = nw;
    }
  }

  for (int f = 7; f >= 0; f--, sowe = westOne(sowe)) {
    uint64_t sw = sowe;
    for (int r8 = 8 * 8 - 8; r8 >= 0; r8 -= 8, sw >>= 8) {
      rayAttacks[r8 + f][SoWe] = sw;
    }
  }
  for (int f = 0; f < 8; f++, soea = eastOne(soea)) {
    uint64_t sa = soea;
    for (int r8 = 8 * 8 - 8; r8 >= 0; r8 -= 8, sa >>= 8) {
      rayAttacks[r8 + f][SoEa] = sa;
    }
  }

  for (int square = 63; square >= 0; square--) {
    RankAttacks[square] = rayAttacks[square][East] | rayAttacks[square][West];
    FileAttacks[square] = rayAttacks[square][Nort] | rayAttacks[square][Sout];
    DiagonalAttacks[square] =
        rayAttacks[square][NoEa] | rayAttacks[square][SoWe];
    AntiDiagonalAttacks[square] =
        rayAttacks[square][NoWe] | rayAttacks[square][SoEa];
    RookAttacks[square] = RankAttacks[square] | FileAttacks[square];
    BishopAttacks[square] =
        DiagonalAttacks[square] | AntiDiagonalAttacks[square];
    QueenAttacks[square] = RookAttacks[square] | BishopAttacks[square];
  }

  for (int f = 0; f < 8; f++) {
    for (int r8 = 0; r8 < 8; r8++) {
      auto pos = uint64_t(1) << (7 - r8 + 8 * f);
      int index = f * 8 + r8;
      PawnAttacks[BLACK_PIECES][index] =
          bPawnEastAttacks(pos) & bPawnEastAttacks(pos);
      PawnAttacks[WHITE_PIECES][index] =
          wPawnEastAttacks(pos) & wPawnEastAttacks(pos);
      KnightAttacks[index] = knightAttacks(pos);
      KnightAttacks[index] = kingAttacks(pos);
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
}

uint64_t BitBoard::attacksTo(uint64_t occupied, int sq) {
  uint64_t knights, kings, bishopsQueens, rooksQueens;
  knights = pieceBB[WHITE_KNIGHT] | pieceBB[BLACK_NIGHT];
  kings = pieceBB[WHITE_KNIGHT] | pieceBB[BLACK_KING];
  rooksQueens =
  bishopsQueens = pieceBB[WHITE_QUEEN] | pieceBB[BLACK_QUEEN];
  rooksQueens |= pieceBB[WHITE_ROOK] | pieceBB[BLACK_RUCK];
  bishopsQueens |= pieceBB[WHITE_BISHOP] | pieceBB[BLACK_BISHOP];

  return (PawnAttacks[WHITE_PIECES][sq] & pieceBB[BLACK_PAWN])
      | (PawnAttacks[BLACK_PIECES][sq] & pieceBB[WHITE_PAWN])
      | (KnightAttacks[sq] & knights)
      | (KingAttacks[sq] & kings)
      | (bishopAttacks(occupied, sq) & bishopsQueens)
      | (rookAttacks(occupied, sq) & rooksQueens);
}

bool BitBoard::attacked(uint64_t occupied, int square, BPieceType bySide) {
  uint64_t pawns = pieceBB[WHITE_PAWN + bySide];
  if (PawnAttacks[bySide ^ 1][square] & pawns) return true;
  uint64_t knights = pieceBB[WHITE_KNIGHT + bySide];
  if (KnightAttacks[square] & knights) return true;
  uint64_t king = pieceBB[WHITE_KING + bySide];
  if (KingAttacks[square] & king) return true;
  uint64_t bishopsQueens = pieceBB[WHITE_QUEEN + bySide]
      | pieceBB[WHITE_BISHOP + bySide];
  if (bishopAttacks(occupied, square) & bishopsQueens) return true;
  uint64_t rooksQueens = pieceBB[WHITE_QUEEN + bySide]
      | pieceBB[WHITE_ROOK + bySide];
  if (rookAttacks(occupied, square) & rooksQueens) return true;
  return false;
}

uint64_t BitBoard::inBetween(int from, int to) {
  return arrRectangular[from][to];
}

bool BitBoard::mayMove(int from, int to, uint64_t occ) {
  return (inBetween(from, to) & occ) == 0;
}

uint64_t BitBoard::xrayFileAttacks(uint64_t occ,
                                   uint64_t blockers,
                                   int rookSq) {
  uint64_t attacks = fileAttacks(occ, rookSq);
  blockers &= attacks & uint64_t(0x00FFFFFFFFFFFF00);
  if (blockers == 0) return blockers;
  return attacks ^ fileAttacks(occ ^ blockers, rookSq);
}

uint64_t BitBoard::xrayRankAttacks(uint64_t occ,
                                   uint64_t blockers,
                                   int rookSq) {
  uint64_t attacks = rankAttacks(occ, rookSq);
  blockers &= attacks & uint64_t(0x7E7E7E7E7E7E7E7E);
  if (blockers == 0) return blockers;
  return attacks ^ rankAttacks(occ ^ blockers, rookSq);
}

uint64_t BitBoard::xrayDiagonalAttacks(uint64_t occ,
                                       uint64_t blockers,
                                       int bishopSq) {
  uint64_t attacks = diagonalAttacks(occ, bishopSq);
  blockers &= attacks & uint64_t(0x007E7E7E7E7E7E00);
  if (blockers == 0) return blockers;
  return attacks ^ diagonalAttacks(occ ^ blockers, bishopSq);
}
uint64_t BitBoard::attacksToKing(int squareOfKing, BPieceType colorOfKing) {
  uint64_t opPawns, opKnights, opRQ, opBQ;
  opPawns = pieceBB[BLACK_PAWN - colorOfKing];
  opKnights = pieceBB[BLACK_NIGHT - colorOfKing];
  opRQ = opBQ = pieceBB[BLACK_QUEEN - colorOfKing];
  opRQ |= pieceBB[BLACK_RUCK - colorOfKing];
  opBQ |= pieceBB[BLACK_BISHOP - colorOfKing];
  return (PawnAttacks[colorOfKing][squareOfKing] & opPawns)
      | (KnightAttacks[squareOfKing] & opKnights)
      | (bishopAttacks(occupiedBB, squareOfKing) & opBQ)
      | (rookAttacks(occupiedBB, squareOfKing) & opRQ);
}

uint64_t BitBoard::getLeastValuablePiece(uint64_t attadef,
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
int BitBoard::see( int toSq, BPieceType target, int frSq, BPieceType aPiece)
{/*
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