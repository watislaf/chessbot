from threading import Thread, Lock
from time import sleep

import numpy as np
import cv2

from mss import mss

from bin.play_session_capture.chess_board import ChessBoard
import tkinter as tk

from bin.play_session_capture.chess_button import ChessButton


class OpenCvController:
    sct = mss()
    __screene_img = None
    __board = None
    __monitor_size = [None, None]
    __atomic_data = None
    __is_start = False
    __is_positions_fixed = False
    __button = ChessButton()

    def __init__(self, atomic_data):
        root = tk.Tk()
        self.__atomic_data = atomic_data
        self.__monitor_size = [root.winfo_screenwidth() - 2,
                               root.winfo_screenheight() - 2]
        self.__board = ChessBoard()
        t = Thread(target=self.start_process, args=())
        t.start()

    def start_process(self):
        while True:
            sleep(0.101)
            if self.__is_start != self.__atomic_data.is_start:
                self.__is_start = self.__atomic_data.is_start
                if not self.__is_start:
                    self.__is_positions_fixed = False

            screen_img = np.array(
                self.sct.grab(self.get_box(self.__board.found)))
            #                    self.__is_positions_fixed)))  #
            screen_img = cv2.resize(screen_img,
                                    np.array(screen_img.shape[:-1:])[::-1] // 2)
            gray_img = cv2.cvtColor(screen_img, cv2.COLOR_BGR2GRAY)

            if self.__is_positions_fixed:  # not
                self.__button.update(gray_img)
                self.__button.write(screen_img)
                self.__board.updateBoard(gray_img)
                self.__board.writeBoard(screen_img)
                self.__atomic_data.cant_find_go_button = not self.__button.found
                self.__atomic_data.cant_find_board = not self.__board.found
                if self.__is_start:
                    if self.__atomic_data.counter <= 0:
                        self.__is_positions_fixed = True

            #            if self.__is_positions_fixed:
            self.__board.updateBoard(gray_img)
            self.__board.writeBoard(screen_img)
            self.__board.updatePieces(gray_img)
            self.__board.writePieces(screen_img)

            cv2.imshow('search_for_chess_board',
                       cv2.resize(screen_img, np.array(
                           screen_img.shape[:-1:])[::-1] // 2))
            cv2.waitKey(1)

    def get_box(self, small_box=False):
        if small_box:
            bounding_box = \
                {'top': self.__board.position_left_top[1] * 2,
                 'left': self.__board.position_left_top[0] * 2,
                 'width': self.__board.position_right_bottom[0] * 2 -
                          self.__board.position_left_top[0] * 2,
                 'height': self.__board.position_right_bottom[1] * 2 -
                           self.__board.position_left_top[1] * 2, }
            print(self.__board.position_left_top)
        else:
            bounding_box = {'top': 0, 'left': 0,
                            'width': self.__monitor_size[0],
                            'height': self.__monitor_size[1]}
        bounding_box["top"] = max(0, bounding_box["top"])
        bounding_box["left"] = max(0, bounding_box["left"])
        bounding_box["width"] = min(
            self.__monitor_size[0] - bounding_box["left"],
            bounding_box["width"])
        bounding_box["height"] = min(
            self.__monitor_size[1] - bounding_box["top"],
            bounding_box["height"])
        return bounding_box
