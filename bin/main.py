"""
The project is created to play chess on chess.com, this is the main function
witch divide two main modes - play on your own board with bot and make
engine to play versus chess.com player.
"""

import sys
from bin.gamecontroller import GameController
from bin.player.ai_player import AiPlayer
from bin.tools.chess_exception import ChessException
from engine.output.chessengine import ChessEngine
from bin.player.view_player import ViewPlayer


def init_personal_board():
    """
    Create board and players object
    """
    _controller = GameController()
    _controller.start_window()
    view_player = ViewPlayer(
        _controller.window.last_move,
        _controller.window_event_obj,
        _controller.window.get_window_board_str,
    )
    #  controller.set_players( AiPlayer(ChessAi("A1")),view_player)
    _controller.set_players(view_player, AiPlayer(ChessEngine("A1")))
    return _controller


def init_board_finder():
    """
    Create OpenCv class and window to track OpenCv progress
    """
    try:
        from bin.player.chesscom_player import ChesscomPlayer
    except ModuleNotFoundError:
        print("Please reinstall project with Yest option to have all modules.")
        exit(0)
        return

    _controller = GameController()
    # controller.start_window()
    # view_player = ViewPlayer(controller.window.last_move,
    #                             controller.window_event_obj,
    #                             controller.window.get_window_board_str)
    chess_com_player = ChesscomPlayer()
    _controller.set_players(chess_com_player, AiPlayer(ChessEngine("A1")))
    return _controller


if __name__ == "__main__":
    if "board" in sys.argv:
        CHESSCOM = False
    else:
        CHESSCOM = True

    if not CHESSCOM:
        _controller = init_board_finder()
    else:
        _controller = init_personal_board()
    while True:
        try:
            winner = _controller.start_game(
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
            )
            print(winner)
        except ChessException as _e:
            print(_e)
            exit(0)
