from threading import Thread, Lock
from time import sleep

import numpy as np
import cv2

from mss import mss

from bin.play_session_capture.chess_board import ChessBoard
from graphics.pictures import PICTURES_PATH
from bin.play_session_capture.chess_peace import ChessPiece
import tkinter as tk


class OpenCvController:
    sct = mss()
    screene_img = None
    board_mutex = Lock()
    board = None
    monitor_size = [None, None]

    def __init__(self):
        root = tk.Tk()
        self.monitor_size = [root.winfo_screenwidth() - 2,
                             root.winfo_screenheight() - 2]
        self.board = ChessBoard()
        t = Thread(target=self.start_process, args=())
        t.start()

    def start_process(self):
        while True:
            sleep(0.201)
            sct_img = np.array(self.sct.grab(self.get_box()))
            lol = np.array(sct_img.shape[:-1:])[::-1] // 4
            sct_img = cv2.resize(sct_img, np.array(sct_img.shape[:-1:])[::-1] // 2)

            sct_img2 = cv2.cvtColor(sct_img, cv2.COLOR_BGR2GRAY)

            self.board.update(sct_img2)

            self.board.write(sct_img)
            sct_img = cv2.resize(sct_img, np.array(sct_img.shape[:-1:])[::-1] // 2)

            cv2.imshow('search_for_chess_board', sct_img)
            if (cv2.waitKey(1) & 0xFF) == ord('q'):
                cv2.destroyAllWindows()
                break

    def get_box(self):
        if self.board.all_pieces_found:
            bounding_box = \
                {'top': self.board.position_left_top[1]*2,
                 'left': self.board.position_left_top[0]*2,
                 'width': self.board.position_right_bottom[0]*2 -
                          self.board.position_left_top[0]*2,
                 'height': self.board.position_right_bottom[1]*2 -
                           self.board.position_left_top[1]*2, }

        else:
            bounding_box = {'top': 0, 'left': 0, 'width': self.monitor_size[0],
                            'height': self.monitor_size[1]}
        bounding_box["top"] = max(0, bounding_box["top"])
        bounding_box["left"] = max(0, bounding_box["left"])
        bounding_box["width"] = min(self.monitor_size[0] - bounding_box["left"],
                                    bounding_box["width"])
        bounding_box["height"] = min(self.monitor_size[1] - bounding_box["top"],
                                     bounding_box["height"])
        return bounding_box
