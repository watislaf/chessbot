import threading

from bin.controller import Controller
from bin.play_session_capture.OpenCvController import OpenCvController
from bin.player.view_player import ViewPlayer


#controller = OpenCvController()

if __name__ == "__main__":
    controller = Controller()
    controller.start_window()
    view_player = ViewPlayer(controller.window.last_move,
                             controller.window_event_obj,
                             controller.window.get_window_board_str)
    controller.set_white_player(view_player)
    controller.set_black_player(view_player)

    controller.start_game("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1")
    # MoveValidator()
    # bots play
