import threading

from bin.controller import Controller
from bin.player.view_player import ViewPlayer
from cppimport.output.chessai import ChessAi

controller = Controller()

if __name__ == "__main__":
    # View open
    controller.start_view()

    view_plaer = ViewPlayer()
    #    controller.set_white_player(chesss_ai)
    #    controller.set_black_player(chesss_ai)

    controller.start_game(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    # MoveValidator()
    # bots play
