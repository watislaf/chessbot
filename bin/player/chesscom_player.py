import threading

from bin.play_session_capture.open_cv_controller import OpenCvController
from bin.player.player import Player
from bin.tools.atomic_data import AtomicData
from bin.view.chesscom_player_window import WindowCV


class ChesscomPlayer(Player):
    __controller = None
    __window = None
    __start_game_event = threading.Event()

    def get_board_str(self):
        " \n".join(list(map(lambda list: " ".join(list),
                            self.__controller.get_board_str())))

    def start_game(self, str_fen: str):
        self.__start_game_event.wait(timeout=None)
        self.__start_game_event.clear()
        self.is_my_move = self.__controller.is_my_move()
        # self.__event_obj.wait(timeout=None)
        # self.__window_event_obj.set()

    def apply_move(self, move):
        self.is_my_move = not self.is_my_move
        if self.is_my_move:
            print("MY MOVE",move)
            self.__controller.apply_move(move)

    def get_move(self):
        return self.__controller.get_move()

    def __init__(self):
        atomic_data = AtomicData()
        self.__controller = OpenCvController(atomic_data,
                                             self.__start_game_event)
        self.__window = WindowCV(atomic_data)
        threading.Thread(target=self.__window.start_loop).start()
