import sys
from bin.tools.move import PieceMove

AI_PATH = '/'.join(__file__.split("/")[:-1])
sys.path.append(AI_PATH)

import CPPChessLib as lib


class ChessAi:
    __CPPChessLib = None

    def __init__(self):
        self.__CPPChessLib = lib.ChessAi()

    def start_new_game(self, str_fen: str):
        self.__CPPChessLib.startNewGame(str_fen)

    def get_possible_moves_for_position(self, x: int, y: int):
        mowes_strings = self.__CPPChessLib.getPossibleMovesForPosition(x, y) \
                            .split(" ")[:-1]
        return tuple(map(lambda move_str: PieceMove(move_str), mowes_strings))
