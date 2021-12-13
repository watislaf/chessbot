from threading import Thread, Lock
from time import sleep

import numpy as np
import cv2

from mss import mss

from bin.play_session_capture.chess_board import ChessBoard
import tkinter as tk

from bin.play_session_capture.chess_button import ChessButton
from bin.play_session_capture.mouse_controller import MouseController


def slicee(funct):
    def wrapper(self,*args):
        bounding_box = funct(self,*args)
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

    def __init__(self, atomic_data):
        root = tk.Tk()
        self.__atomic_data = atomic_data
        self.monitor_size = [root.winfo_screenwidth() - 2,
                             root.winfo_screenheight() - 2]
        self.__board = ChessBoard()
        t = Thread(target=self.start_process, args=())
        t.start()

    def start_process(self):
        while True:
            sleep(0.401)
            if self.__is_start != self.__atomic_data.is_start:
                self.__is_start = self.__atomic_data.is_start
                if self.__is_start == False:
                    self.__game_is_found = False
                    self.__game_in_search = False

            if self.__atomic_data.counter < 2:
                screen_img, gray_img = self.get_board()
            else:
                screen_img, gray_img = self.get_all()
                self.__board.found = False
            if not self.__is_start:
                if self.__atomic_data.counter > 2:
                    self.__button_start.update(gray_img)
                    self.__button_start.write(screen_img)
                self.__board.updateBoard(gray_img)
                self.__board.writeBoard(screen_img)
                self.__atomic_data.cant_find_go_button = not self.__button_start.found
                self.__atomic_data.cant_find_board = not self.__board.found
            else:
                if self.__game_is_found:
                    self.__board.updatePieces(gray_img)
                    self.__board.writePieces(screen_img)
                    self.__button_draw.update(
                        self.get_button(self.__button_draw)[1],
                        self.__button_draw.positions[0]
                    )
                    if not self.__button_draw.found:
                        self.__game_is_found = False
                    pass  # Game process
                else:
                    if not self.__game_in_search:
                        self.__mouse_controller.click(
                            self.__button_start.positions[0][0] * 2,
                            self.__button_start.positions[0][1] * 2)
                        self.__game_in_search = True
                    else:
                        self.__button_draw.update(self.get_all()[1])
                        if self.__button_draw.found:
                            self.__game_in_search = False
                            self.__game_is_found = True
            cv2.imshow('search_for_chess_board',
                       cv2.resize(screen_img, np.array(
                           screen_img.shape[:-1:])[::-1] // 2))
            cv2.waitKey(1)

    @slicee
    def get_board(self,*args):
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
    def get_all(self,*args):
        bounding_box = {'top': 0, 'left': 0,
                        'width': self.monitor_size[0],
                        'height': self.monitor_size[1]}
        return bounding_box
