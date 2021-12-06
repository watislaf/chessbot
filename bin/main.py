import time

from bin.controller import Controller
from bin.player.ai_player import AiPlayer
from bin.player.chesscom_player import ChesscomPlayer

from bin.player.view_player import ViewPlayer
from cppimport.output.chessai import ChessAi

if __name__ == "__main__":
    is_player_mode = False
    controller = Controller()
    controller.start_window()
    view_player = ViewPlayer(controller.window.last_move,
                             controller.window_event_obj,
                             controller.window.get_window_board_str)
    # chess_com_player = ChesscomPlayer()
    controller.set_players(AiPlayer(ChessAi("random")), AiPlayer(ChessAi("random")))
    while True:
        winner = controller.start_game(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
        print(winner)
        time.sleep(3)
