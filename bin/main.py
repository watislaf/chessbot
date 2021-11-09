import threading

from bin.controller import Controller
from bin.play_session_capture.OpenCvController import OpenCvController
from bin.play_session_capture.MouseController import MouseController
from bin.player.chesscom_player import ChesscomPlayer
from bin.player.view_player import ViewPlayer


controller = OpenCvController()

if __name__ == "__main__2":
    controller = Controller()
    controller.start_window()
    view_player = ViewPlayer(controller.window.last_move,
                             controller.window_event_obj)
    controller.set_white_player(view_player)
    controller.set_black_player(view_player)

    controller.start_game(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    # MoveValidator()
    # bots play
