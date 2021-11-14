import time

from bin.controller import Controller
from bin.player.ai_player import AiPlayer

# controller = OpenCvController()
from bin.player.view_player import ViewPlayer
from cppimport.output.chessai import ChessAi

if __name__ == "__main__":
    controller = Controller()
    controller.start_window()
    view_player = ViewPlayer(controller.window.last_move,
                              controller.window_event_obj,
                              controller.window.get_window_board_str)
    controller.set_white_player(view_player)
    controller.set_black_player(AiPlayer(ChessAi("A1")))
    while True:
        winner = controller.start_game(
            "r1b1k2r/ppppqppp/7n/2b1p3/2BnP1PP/2N5/PPPP1PQ1/R1B1K1NR b KQkq - 2 8")
        print(winner)
        time.sleep(5)
