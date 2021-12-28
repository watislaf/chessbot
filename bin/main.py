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


def init_personal_board(args: dict):
    """
    Create board and players object
    """
    _controller = GameController()
    _controller.start_window()
    if args.get("white_player", "player") == "engine":
        white_player = AiPlayer(ChessEngine(args.get("lvl", "A1")))
    else:
        white_player = ViewPlayer(
            _controller.window.last_move,
            _controller.window_event_obj,
            _controller.window.get_window_board_str,
        )

    if args.get("black_player", "engine") == "human":
        black_player = ViewPlayer(
            _controller.window.last_move,
            _controller.window_event_obj,
            _controller.window.get_window_board_str,
        )
    else:
        black_player = AiPlayer(ChessEngine(args.get("lvl", "A1")))

    #  controller.set_players( AiPlayer(ChessAi(lvl)),view_player)
    _controller.set_players(white_player, black_player)
    return _controller


def init_board_finder(args: dict):
    """
    Create OpenCv class and window to track OpenCv progress
    """
    print("Please, check your board settings on chess.com with the "
          "settings.png settings.")
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

    _controller.set_players(chess_com_player,
                            AiPlayer(ChessEngine(args.get("lvl", "bullet"))))
    return _controller


if __name__ == "__main__":
    try:
        args = dict()
        for k, v in ((k.lstrip('-'), v) for k, v in
                     (a.split('=') for a in sys.argv[1:])):
            args[k.lower()] = v.lower()
    except ValueError:
        print("Bad arguments")
        exit(0)
    if args.get("lvl", "bullet") not in ["bullet", "blitz", "rapid"]:
        args["lvl"] = "bullet"
    print("Engine level is: ", args.get("lvl", "bullet"))
    if args.get("mode", "chesscom") == "board":
        CHESSCOM = False
    else:
        CHESSCOM = True

    if CHESSCOM:
        _controller = init_board_finder(args)
    else:
        _controller = init_personal_board(args)
    while True:
        try:
            winner = _controller.start_game(
                "2kr1b1r/2p1p1pp/4q1p1/p1p5/3P1BP1/N1P2Q2/PP3P2/R4RK1 b - - 1 17"
#                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
            )
            print(winner)
        except ChessException as _e:
            print(_e)
            exit(0)
