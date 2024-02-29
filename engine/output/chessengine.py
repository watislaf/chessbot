"""
Layer between CPP engine and Python view code
"""

import sys

from bin.tools.pieceMove import PieceMove

#    Pythons linker cant see CPPChessLib.so, if it is not in path variables
#    https://youtrack.jetbrains.com/issue/PY-12238

ENGINE_PATH = "/".join(__file__.split("/")[:-1])
sys.path.append(ENGINE_PATH)

import CPPChessLib as lib


class ChessEngine:
    __cpp_chess_lib: lib.ChessEngine = None

    def __init__(self, mode: str):
        self.__cpp_chess_lib = lib.ChessEngine(mode)

    def start_new_game(self, _fen: str):
        #        try:
        self.__cpp_chess_lib.startNewGame(_fen)

    #        except:
    #        raise ChessException("FEN is wrong")

    def get_possible_moves_for_position(self, _x: int, _y: int):
        mowes_strings = self.__cpp_chess_lib.getPossibleMovesForPosition(
            _x, _y).split(" ")[:-1]
        return tuple(map(PieceMove, mowes_strings))

    def apply_move(self, move: PieceMove):
        self.__cpp_chess_lib.applyMoveParams(
            *move.position_from, *move.position_to, move.new_piece
        )

    def get_best_move(self):
        return PieceMove(self.__cpp_chess_lib.getBestMoveStr())

    def who_is_turn(self):
        return self.__cpp_chess_lib.whosTurn()

    def get_board_str(self):
        return self.__cpp_chess_lib.getBoardStr()

    def is_move_exists(self):
        return self.__cpp_chess_lib.isMoveExists()
