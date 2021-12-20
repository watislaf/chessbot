import time

from bin.controller import Controller
from bin.player.ai_player import AiPlayer
from bin.player.chesscom_player import ChesscomPlayer

from bin.player.view_player import ViewPlayer
from bin.tools.chess_exception import ChessException
from cppimport.output.chessai import ChessAi

CHESSCOM = True


def process():
    if CHESSCOM:
        controller = Controller()
        # controller.start_window()
        # view_player = ViewPlayer(controller.window.last_move,
        #                             controller.window_event_obj,
        #                             controller.window.get_window_board_str)
        chess_com_player = ChesscomPlayer()
        controller.set_players(chess_com_player, AiPlayer(ChessAi("A1")))
    else:
        controller = Controller()
        controller.start_window()
        view_player = ViewPlayer(controller.window.last_move,
                                 controller.window_event_obj,
                                 controller.window.get_window_board_str)
        #  controller.set_players( AiPlayer(ChessAi("A1")),view_player)
        controller.set_players( view_player,AiPlayer(ChessAi("A1")))
    while True:
        try:
            winner = controller.start_game(
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
            print(winner)
        except ChessException as E:
            print(E)
            return


if __name__ == "__main__":
    process()
