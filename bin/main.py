#!/usr/bin/sudo python

from bin.controller import Controller
from bin.play_session_capture.CaptureController import CaptureController
from bin.play_session_capture.MouseController import MouseController
from bin.player.chesscom_player import ChesscomPlayer
from bin.player.view_player import ViewPlayer

mouse_move = MouseController()
#mouse_move.drag()

if __name__ == "__main__2":
    controller = Controller()
    controller.start_window()

    view_player = ViewPlayer()
    controller.set_white_player(view_player)
    # controller.set_black_player(chesss_ai)

    controller.start_game(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    # MoveValidator()
    # bots play
