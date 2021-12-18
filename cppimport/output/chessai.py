import sys

from bin.tools.chess_exception import ChessException
from bin.tools.pieceMove import PieceMove

AI_PATH = '/'.join(__file__.split("/")[:-1])
sys.path.append(AI_PATH)

import CPPChessLib as lib


class ChessAi:
    __CPPChessLib = None

    def __init__(self, mode):
        self.__CPPChessLib = lib.ChessAi(mode)  # random A1 A2 A3

    def start_new_game(self, str_fen: str):
        try:
            self.__CPPChessLib.startNewGame(str_fen)
        except:
            raise ChessException("FEN is wrong")

    def get_possible_moves_for_position(self, x: int, y: int):
        mowes_strings = self.__CPPChessLib.getPossibleMovesForPosition(x, y) \
                            .split(" ")[:-1]
        return tuple(map(lambda move_str: PieceMove(move_str), mowes_strings))

    def apply_move(self, move):
        self.__CPPChessLib.applyMoveParams(*move.position_from,
                                           *move.position_to,
                                           move.new_piece)

    def get_best_move(self):
        return PieceMove(self.__CPPChessLib.getBestMoveStr())

    def whos_move(self):
        return self.__CPPChessLib.whosMove()

    def get_board_str(self):
        return self.__CPPChessLib.getBoardStr()

    def is_move_exists(self):
        return self.__CPPChessLib.isMoveExists()
