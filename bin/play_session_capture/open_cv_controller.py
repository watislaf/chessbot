from threading import Thread, Lock
from time import sleep

import numpy as np
import cv2

from mss import mss

from bin.play_session_capture.chess_board import ChessBoard
import tkinter as tk


class OpenCvController:
    sct = mss()
    __screene_img = None
    __board = None
    __monitor_size = [None, None]
    __atomic_data = None
    __prev_state = 0

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
            sleep(0.201)
            sct_img = np.array(self.sct.grab(self.get_box()))
            sct_img = cv2.resize(sct_img,
                                 np.array(sct_img.shape[:-1:])[::-1] // 2)

            sct_img2 = cv2.cvtColor(sct_img, cv2.COLOR_BGR2GRAY)

            self.__board.update(sct_img2)
            self.__atomic_data.cant_find_board_ = not self.__board.all_pieces_found
            self.__board.write(sct_img)
            sct_img = cv2.resize(sct_img,
                                 np.array(sct_img.shape[:-1:])[::-1] // 2)
            cv2.imshow('search_for_chess_board', sct_img)
            cv2.waitKey(1)

    def get_box(self):
        if self.__board.all_pieces_found:
            bounding_box = \
                {'top': self.__board.position_left_top[1] * 2,
                 'left': self.__board.position_left_top[0] * 2,
                 'width': self.__board.position_right_bottom[0] * 2 -
                          self.__board.position_left_top[0] * 2,
                 'height': self.__board.position_right_bottom[1] * 2 -
                           self.__board.position_left_top[1] * 2, }

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
