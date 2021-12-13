import random
from asyncio import sleep

import cv2
import numpy as np

from bin.play_session_capture.chess_peace import ChessPiece
from collections import deque

from graphics.pictures import GRAY_COLOR, BLACK_PIECE, WHITE_PIECE


class ChessBoard:
    found = False
    position_left_top = [0, 0]
    position_right_bottom = [0, 0]
    piece_size = 1
    __pieces_white = []
    __pieces_black = []
    empty_spaces = None
    __min_pos = None

    def __init__(self):
        self.empty_spaces = ChessPiece("")
        for piece in {"p", "k", "b", "q", "r", "n"}:
            self.__pieces_black.append(ChessPiece("b" + piece)),
        for piece in {"p", "k", "b", "q", "r", "n"}:
            self.__pieces_white.append(ChessPiece("w" + piece)),

        # self.pieces = ChessPiece("")

    def updatePieces(self, screen):
        if self.__min_pos[0] + 1 < self.piece_size or \
                self.__min_pos[1] + 1 < self.piece_size or \
                self.position_right_bottom[0] + 1 < self.__min_pos[0] + \
                self.piece_size * 8 or \
                self.position_right_bottom[1] + 1 < self.__min_pos[1] + \
                self.piece_size * 8:
            print("CONITUE")
            return
        if self.found:
            self.preprocess_board_screen(screen, False)
            self.preprocess_board_screen(screen, True)

    def updateBoard(self, screen):
        self.empty_spaces.find(screen)

        if self.find_board_pattern():
            if self.found is False:
                self.found = True
                self.apply_board_from_positions()
            else:
                self.update_board_from_positions()
            for piece in self.__pieces_black:
                piece.size = self.piece_size
            for piece in self.__pieces_white:
                piece.size = self.piece_size
        else:
            self.__min_pos = [10000, 10000]
            self.found = False

    def get_screen_only_from(self, screen, color_to_save):
        for i, elements in enumerate(screen):
            for j, color in enumerate(elements):
                if screen[i][j] != color_to_save:
                    screen[i][j] = GRAY_COLOR[0]
        return screen

    def writeBoard(self, screen):
        if self.found:
            self.empty_spaces.write(screen, (244, 99, 0))
        else:
            self.empty_spaces.write(screen, (0, 0, 255))
        self.empty_spaces.positions.clear()

    def writePieces(self, screen):
        for piece in self.__pieces_black:
            piece.write(screen)
            piece.positions.clear()
        for piece in self.__pieces_white:
            piece.write(screen)
            piece.positions.clear()

    def apply_board_from_positions(self):
        min_pos = [10000, 10000]
        max_pos = [-10000, -10000]
        for empty_pos in self.empty_spaces.positions:
            if empty_pos[0] <= min_pos[0]:
                min_pos[0] = empty_pos[0]
            if empty_pos[1] <= min_pos[1]:
                min_pos[1] = empty_pos[1]
            if empty_pos[0] >= max_pos[0]:
                max_pos[0] = empty_pos[0]
            if empty_pos[1] >= max_pos[1]:
                max_pos[1] = empty_pos[1]
        self.__min_pos = min_pos
        self.piece_size = (max_pos[0] - min_pos[0]) / 6
        self.position_right_bottom = (
            max_pos[0] + 11 + self.piece_size,
            max_pos[1] + 11 + self.piece_size)
        self.position_left_top = (
            min_pos[0] - self.piece_size, min_pos[1] - self.piece_size)

    def find_board_pattern(self):
        if len(self.empty_spaces.positions) < 49:
            return False
        for i in range(4):
            giv_int = (i * 47) % ( len(self.empty_spaces.positions) - 1)
            start = self.empty_spaces.positions[giv_int]
            green_ones = deque()  # get in
            orange_ones = deque()  # need analize
            gray_ones = list()  # all others
            size = 100000
            for pos in self.empty_spaces.positions:
                if pos == start:
                    continue
                gray_ones.append(pos)
                if abs(pos[0] - start[0]) < 5:
                    size = min(abs(pos[1] - start[1]), size)
            orange_ones.append(start)

            while len(orange_ones):
                element = orange_ones.pop()
                green_ones.append(element)
                for k in range(len(gray_ones) - 1, -1, -1):
                    if (abs(gray_ones[k][0] - element[0]) < 5 and abs(
                            abs(gray_ones[k][1] - element[1]) - size) < 10) or (
                            abs(gray_ones[k][1] - element[1]) < 5 and abs(
                        abs(gray_ones[k][0] - element[0]) - size) < 10):
                        orange_ones.append(gray_ones[k])
                        del gray_ones[k]
            if len(green_ones) == 49:
                self.empty_spaces.positions = list(green_ones)
                return True
        return False

    def update_board_from_positions(self):
        min_pos = [10000, 10000]
        max_pos = [-10000, -10000]
        for empty_pos in self.empty_spaces.positions:
            if empty_pos[0] <= min_pos[0]:
                min_pos[0] = empty_pos[0]
            if empty_pos[1] <= min_pos[1]:
                min_pos[1] = empty_pos[1]
            if empty_pos[0] >= max_pos[0]:
                max_pos[0] = empty_pos[0]
            if empty_pos[1] >= max_pos[1]:
                max_pos[1] = empty_pos[1]

        self.piece_size = (max_pos[0] - min_pos[0]) / 6
        self.position_left_top = (
            self.position_left_top[0] + min_pos[0] - int(self.piece_size),
            self.position_left_top[1] + min_pos[1] - int(self.piece_size))
        self.__min_pos = min_pos
        self.position_right_bottom = (
            self.position_left_top[0] +
            max_pos[0] + 11 + int(self.piece_size),
            self.position_left_top[1] +
            max_pos[1] + 11 + int(self.piece_size))

    def preprocess_board_screen(self, screen, isBlack):
        new_screen = screen.copy()
        for i, elements in enumerate(screen):
            for j, pas in enumerate(elements):
                if isBlack:
                    if new_screen[i][j] < 120:
                        new_screen[i][j] = BLACK_PIECE[0]
                    else:
                        new_screen[i][j] = WHITE_PIECE[0]
                else:
                    if new_screen[i][j] > 235:
                        new_screen[i][j] = WHITE_PIECE[0]
                    else:
                        new_screen[i][j] = BLACK_PIECE[0]
        if isBlack:
            pieces = self.__pieces_black
        else:
            pieces = self.__pieces_white

        for i in range(8):
            for j in range(8):
                size = new_screen.shape[0]

                right_bot = [int(self.__min_pos[0] + self.piece_size * i + 8),
                             int(self.__min_pos[1] + self.piece_size * j + 4)]
                left_top = [right_bot[0] - int(self.piece_size) - 2,
                            right_bot[1] - int(self.piece_size) - 2]
                tmp_cut = new_screen[size - right_bot[1]:size - left_top[1],
                          left_top[0]:right_bot[0]]

                if tmp_cut.shape[0] < int(self.piece_size) or \
                        tmp_cut.shape[1] < int(self.piece_size):
                    return
                mean = np.mean(tmp_cut)
                if 20 > mean or mean > 210:
                    continue
                for piece in pieces:
                    piece.find(tmp_cut,[int(self.__min_pos[0] + self.piece_size * (i-1) + 8),
                                        int(self.__min_pos[1] + self.piece_size * (6-j) + 4)])
