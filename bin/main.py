import time

from bin.controller import Controller
from bin.player.ai_player import AiPlayer

from bin.player.view_player import ViewPlayer
from cppimport.output.chessai import ChessAi
from play_session_capture.open_cv_controller import OpenCvController

controller2 = OpenCvController()

if __name__ == "__main__2":
    controller = Controller()
    controller.start_window()
    view_player = ViewPlayer(controller.window.last_move,
                              controller.window_event_obj,
                              controller.window.get_window_board_str)
    controller.set_white_player(view_player)
    controller.set_black_player(AiPlayer(ChessAi("A1")))
    while True:
        winner = controller.start_game(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
        print(winner)
        time.sleep(5)
