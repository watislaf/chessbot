"""
 Controls the game flow
"""
import threading
import re

from bin.view.chess_board_window import ChessBoardWindow


def clean_from_piece_name(param: str):
    """
    gets string and clean every pqkbnr letter, that represents piece type, so
    OpenCv type-blind  .getBoard() can be equaled to
    """
    param = re.sub(r"w([pqkbnr])", "w", param)
    param = re.sub(r"b([pqkbnr])", "b", param)
    param = re.sub(r" ", "", param)
    return param


class GameController:
    window = None
    window_event_obj = None
    __first_player = None
    __second_player = None
    __window_is_initialized = False

    def start_window(self):
        """
        Creates pygame view window with chess on it
        """
        self.window_event_obj = threading.Event()
        self.window = ChessBoardWindow(self.window_event_obj)
        threading.Thread(daemon=True, target=self.window.start_loop).start()
        self.__window_is_initialized = True

    def start_game(self, str_fen: str):
        if self.window is not None:
            self.window.start_new_game_from_fen_str(str_fen)

        self.__first_player.start_game(str_fen)
        self.__second_player.start_game(str_fen)
        print("Start")

        """
        After initialization, is_my_move variable can be set. Actually, is going
        to be set only by ChessComPlayer, so we it can determined if its white 
        or black turn.   
        """

        if (
                self.__first_player.is_my_move is None
                and self.__second_player.is_my_move is None
        ):
            self.__first_player.is_my_move = True
            self.__second_player.is_my_move = False
        else:
            if self.__first_player.is_my_move is None:
                self.__first_player.is_my_move = \
                    not self.__second_player.is_my_move
            else:
                self.__second_player.is_my_move = \
                    not self.__first_player.is_my_move

        while True:
            if self.__first_player.is_my_move:
                move = self.__first_player.get_move()
            else:
                move = self.__second_player.get_move()

            """
            Invalid move means that there is no move can be done that is equal 
            to lose. If move have new_piece==k flag, that means Draw 
            """

            if move.isInvalid():
                self.__first_player.is_my_move = None
                self.__second_player.is_my_move = None
                if move.new_piece != "k":
                    return "DRAW"
                if self.__first_player.is_my_move:
                    return "Game end"
                return "Game endt"
            try:
                self.__first_player.apply_move(move)
                self.__second_player.apply_move(move)
            except ...:
                print("Move error...")

            if self.__window_is_initialized:
                self.window.apply_move(move)
                if clean_from_piece_name(
                        self.__first_player.get_board_str()
                ) != clean_from_piece_name(
                    self.__second_player.get_board_str()):
                    print("DIFFERENT BOARDS BETWEEN PLAYERS")
                    print(self.__first_player.get_board_str())
                    print(self.__second_player.get_board_str())

    def set_players(self, first_player, second_player):
        self.__first_player = first_player
        self.__second_player = second_player
