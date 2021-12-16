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

    def __init__(self, atomic_data):
        root = tk.Tk()
        self.__atomic_data = atomic_data
        self.monitor_size = [root.winfo_screenwidth() - 2,
                             root.winfo_screenheight() - 2]
        self.__board = ChessBoard()
        t = Thread(target=self.start_process, args=())
        t.start()

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
            sleep(0.100)
            self.is_start_update()
            if self.__atomic_data.counter < 2:
                screen_to_write, screen_to_read = self.get_board()
            else:
                screen_to_write, screen_to_read = self.get_all()
                self.__board.found = False

            if not self.__is_start:
                self.stabilisation(screen_to_write, screen_to_read)
            else:
                if self.__game_is_found:
                    self.game_process(screen_to_read)
                else:
                    if not self.__game_in_search:
                        self.click_start_button()
                    else:
                        self.__button_draw.update(self.get_all()[1])
                        if self.__button_draw.found:  # no not
                            self.found_the_game(screen_to_read)

            cv2.imshow('search_for_chess_board',
                       cv2.resize(screen_to_write, np.array(
                           screen_to_write.shape[:-1:])[::-1] // 2))
            cv2.waitKey(1)

    def is_start_update(self):
        if self.__is_start != self.__atomic_data.is_start:
            self.__is_start = self.__atomic_data.is_start
            if not self.__is_start:
                self.__game_is_found = False
                self.__game_in_search = False

    def stabilisation(self, screen_img, gray_img):
        if self.__atomic_data.counter > 2:
            self.__button_start.update(gray_img)
            self.__button_start.write(screen_img)
            self.__board.clean()

        self.__board.updateBoard(gray_img)
        self.__board.writeBoard(screen_img)
 
        self.__board.clean()
        self.__atomic_data.cant_find_go_button = not self.__button_start.found
        self.__atomic_data.cant_find_board = not self.__board.found

    def game_process(self, screen_to_read):
        hassh = self.__board.get_hash(screen_to_read)
        self.__button_draw.update(
            self.get_button(self.__button_draw)[1],
            self.__button_draw.positions[0]
        )
        if not self.__button_draw.found:
            print("GAME is ended because i cant see draw button")
            self.__game_is_found = False
            self.__game_controller = None
            self.__button_start.update(self.get_all()[1])
            if not self.__button_start.found:
                self.__atomic_data.is_start = False
                print("i cant see start button")
                return
            self.__counter = 0
            return

        if abs(self.__hash - hassh) < 10:
            return
        self.__hash = hassh
        self.__counter += 1
        print(self.__counter)
        for as_white in {True, False}:
            if not self.__game_controller.push_board(
                    as_white, self.__board.get_color_board(
                        as_white, screen_to_read)):
                print("To many changes on the board")
                self.__atomic_data.is_start = False
                return
        self.__game_controller.is_whites_move = not self.__game_controller.is_whites_move
        print(self.__game_controller.last_move)


    def click_start_button(self):
        self.__mouse_controller.click(
            self.__button_start.positions[0][0] * 2,
            self.__button_start.positions[0][1] * 2)
        self.__game_in_search = True

    def found_the_game(self, screen_to_read):
        self.__board.updatePieces(screen_to_read)
        if not self.__board.pieces_found:
            self.__game_in_search = False
            self.__atomic_data.is_start = False
            print("CANT SEE PIECES AT THE BEGINNING OF THE GAME")
            return

        self.__game_in_search = False
        self.__game_is_found = True
        print("START")
        self.__game_controller = CVGameController(
            self.__board.get_side(),
            self.__board.get_color_board(1, screen_to_read),
            self.__board.get_color_board(0, screen_to_read))

        self.__hash = self.__board.get_hash(screen_to_read)
        self.__board.clean()
