
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
    controller.set_white_player(AiPlayer(ChessAi("random")))
    controller.set_black_player(AiPlayer(ChessAi("random")))
    while True:
        winner = controller.start_game(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
        print(winner)
