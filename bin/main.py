from bin.controller import Controller
from cppimport.output.chessai import ChessAi

controller = Controller()

if __name__ == "__main__":
    # garik part
    controller.start_view()

#   chesss_ai = ChessAi()
#   chess.startNewGame()
#   lol = chess.getPossibleMoves(4, 1)
#    controller.set_white_player(chesss_ai)
#    controller.set_black_player(chesss_ai)

    controller.start_game()
    # MoveValidator()
    # bots play
