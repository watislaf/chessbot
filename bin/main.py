from bin.controller import Controller
from bin.player.view_player import ViewPlayer

if __name__ == "__main__":
    controller = Controller()
    controller.start_window()

    view_player = ViewPlayer()
    controller.set_white_player(view_player)
    #controller.set_black_player(chesss_ai)

    controller.start_game(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    # MoveValidator()
    # bots play
