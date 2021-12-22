import threading
from threading import Thread, Lock
from time import sleep

import numpy as np
import cv2

from mss import mss

from bin.play_session_capture.chess_board import ChessBoard
import tkinter as tk

from bin.play_session_capture.chess_button import ChessButton
from bin.play_session_capture.cv_game_controller import CVGameController
from bin.play_session_capture.mouse_controller import MouseController
from bin.tools.pieceMove import PieceMove


def slicee(funct):
    def wrapper(self, *args):
        bounding_box = funct(self, *args)
        bounding_box["top"] = max(0, bounding_box["top"])
        bounding_box["left"] = max(0, bounding_box["left"])
        bounding_box["width"] = min(
            self.monitor_size[0] - bounding_box["left"],
            bounding_box["width"])
        bounding_box["height"] = min(
            self.monitor_size[1] - bounding_box["top"],
            bounding_box["height"])
        screen_img = np.array(self.sct.grab(bounding_box))
        screen_img = cv2.resize(screen_img,
                                np.array(screen_img.shape[:-1:])[::-1] // 2)
        gray_img = cv2.cvtColor(screen_img, cv2.COLOR_BGR2GRAY)
        return screen_img, gray_img

    return wrapper


class OpenCvController:
    sct = mss()
    __screene_img = None
    __board = None
    monitor_size = [None, None]
    __atomic_data = None
    __is_start = False
    __is_positions_fixed = False
    __button_start = ChessButton(["next_button", "play_button"])
    __button_draw = ChessButton(["draw_button"])
    __button_resign = ChessButton(["resign_button"])
    __game_is_found = False
    __game_in_search = False
    __mouse_controller = MouseController()
    __game_controller = None
    __hash = 0
    __counter = 0
    __lock = threading.Lock()
    __start_game_event = None
    __get_move_event = None
    __last_screen_to_read = None

    def __init__(self, atomic_data, start_game_event):
        root = tk.Tk()
        self.__start_game_event = start_game_event
        self.__get_move_event = threading.Event()
        self.__atomic_data = atomic_data
        self.monitor_size = [root.winfo_screenwidth() - 2,
                             root.winfo_screenheight() - 2]
        self.__board = ChessBoard()
        Thread(target=self.start_process, args=(), daemon=True).start()

    @slicee
    def get_board(self, *args):
        return \
            {'top': int(self.__board.position_left_top[1] * 2),
             'left': int(self.__board.position_left_top[0] * 2),
             'width': int(self.__board.position_right_bottom[0] * 2 -
                          self.__board.position_left_top[0] * 2),
             'height': int(self.__board.position_right_bottom[1] * 2 -
                           self.__board.position_left_top[1] * 2)}

    @slicee
    def get_button(self, *args):
        return \
            {'top': int(args[0].positions[0][1] * 2 - 10),
             'left': int(args[0].positions[0][0] * 2 - 10),
             'width': int(args[0].last_len[1] * 2 + 20),
             'height': int(args[0].last_len[0] * 2 + 20)}

    @slicee
    def get_all(self, *args):
        bounding_box = {'top': 0, 'left': 0,
                        'width': self.monitor_size[0],
                        'height': self.monitor_size[1]}
        return bounding_box

    def start_process(self):
        while True:
            sleep(0.300)
            self.is_start_update()
            if self.__atomic_data.counter < 2:
                screen_to_write, screen_to_read = self.get_board()
            else:
                screen_to_write, screen_to_read = self.get_all()
                self.__board.found = False
            self.__last_screen_to_read = screen_to_read
            if not self.__is_start:
                self.stabilisation(screen_to_write, screen_to_read)
                cv2.imshow('search_for_chess_board',
                           cv2.resize(screen_to_write, np.array(
                               screen_to_write.shape[:-1:])[::-1] // 2))
                cv2.waitKey(1)
            else:
                if self.__game_is_found:
                    self.game_process(screen_to_read)
                else:
                    if not self.__game_in_search:
                        self.click_start_button()
                    else:
                        self.__button_draw.update(self.get_all()[1])
                        if self.__button_draw.found:
                            self.__board.updatePieces(screen_to_read)
                            if self.__board.pieces_found:
                                self.found_the_game()

    def is_start_update(self):
        if self.__is_start != self.__atomic_data.is_start:
            self.__is_start = self.__atomic_data.is_start
            if not self.__is_start:
                self.__game_is_found = False
                self.__game_in_search = False
                self.__get_move_event.set()
            else:
                cv2.destroyAllWindows()

    def stabilisation(self, screen_to_read, screen_to_write):
        if self.__atomic_data.counter > 2:
            self.__button_start.update(screen_to_write)
            self.__button_start.write(screen_to_read)
            self.__board.clean()

        if self.__atomic_data.counter < 1:
            self.__board.updatePieces(screen_to_write)
            self.__board.writePieces(screen_to_read)
            self.__board.clean()

        self.__board.updateBoard(screen_to_write)
        self.__board.writeBoard(screen_to_read)

        self.__board.clean()
        self.__atomic_data.cant_find_go_button = not self.__button_start.found
        self.__atomic_data.cant_find_board = not self.__board.found

    def game_process(self, screen_to_read):
        self.__button_draw.update(
            self.get_button(self.__button_draw)[1],
            self.__button_draw.positions[0]
        )
        if not self.__button_draw.found:
            print("GAME is ended because i cant see draw button")
            with self.__lock:
                self.__game_is_found = False
                self.__game_controller.defeat()
                self.__get_move_event.set()
            self.__button_start.update(self.get_all()[1])
            if not self.__button_start.found:
                self.__atomic_data.is_start = False

                with self.__lock:
                    self.__game_controller = None
                print("i cant see start button")
                return
            self.__counter = 0
            return

        hassh = self.__board.get_hash(
            screen_to_read, self.__game_controller.get_enemy_color_board(),
            not self.__game_controller.is_play_as_white)

        if self.__hash is None:
            self.__hash = hassh
        if self.__hash == hassh:
            return
        self.__hash = hassh
        with self.__lock:
            if not self.__game_controller.push_board(
                    self.__board.get_color_board(
                        not self.__game_controller.is_play_as_white,
                        screen_to_read)):
                print("Board Changed unproperly")
                return

            self.__counter += 1
            self.__get_move_event.set()

            self.__game_controller.is_whites_move = \
                not self.__game_controller.is_whites_move
            self.__hash = self.__board.get_hash(
                screen_to_read, self.__game_controller.get_enemy_color_board(),
                not self.__game_controller.is_play_as_white)

    def click_start_button(self):
        self.__mouse_controller.click(
            self.__button_start.positions[0][0] * 2,
            self.__button_start.positions[0][1] * 2)
        self.__game_in_search = True

    def found_the_game(self):
        print("Game found")
        self.__game_in_search = False
        self.__game_is_found = True
        self.__game_controller = CVGameController(
            self.__board.bottom_king_is_white())
        self.__hash = None
        self.__board.clean()
        self.__start_game_event.set()
        self.__get_move_event.clear()

    def is_my_move(self):
        with self.__lock:
            return self.__game_controller.is_whites_move != \
                   self.__game_controller.is_play_as_white

    def get_board_str(self):
        ans = [['_' for x in range(8)] for y in range(8)]
        with self.__lock:
            wb = self.__game_controller.white_board.map(
                lambda x: x if x == '_' else 'w')
            bb = self.__game_controller.black_board.map(
                lambda x: x if x == '_' else 'b')
            for i in range(8):
                for j in range(8):
                    if wb[i][j] != '_':
                        ans[i][j] = wb[i][j]
                    if bb[i][j] != '_':
                        ans[i][j] = bb[i][j]
        return ans

    def apply_move(self, move):
        with self.__lock:
            self.__mouse_controller.click(
                *self.__board.board_to_real_coordinates(
                    move.position_from,
                    not self.__game_controller.is_play_as_white))
            self.__mouse_controller.click(
                *self.__board.board_to_real_coordinates(
                    move.position_to,
                    not self.__game_controller.is_play_as_white)
            )

            ki = move.position_to[0]
            kj = move.position_to[1]
            if not self.__game_controller.is_play_as_white:
                ki = 7 - move.position_to[0]
                kj = 7 - move.position_to[1]
            self.__game_controller.get_enemy_color_board()[ki][kj] = "_"
            self.__hash = self.__board.get_hash(
                self.__last_screen_to_read,
                self.__game_controller.get_enemy_color_board(),
                not self.__game_controller.is_play_as_white
            )

    def get_move(self):
        print("HIS MOVE", end=" ")
        self.__get_move_event.wait(timeout=None)
        self.__get_move_event.clear()
        with self.__lock:
            if self.__game_controller is None:
                return PieceMove().getInvalid()
            last_move = self.__game_controller.get_last_move()
            print(last_move)
            return last_move
