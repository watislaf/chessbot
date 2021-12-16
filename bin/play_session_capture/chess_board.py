import random
from asyncio import sleep

import cv2
import numpy as np

from bin.play_session_capture.chess_peace import ChessPiece
from collections import deque

from graphics.pictures import GRAY_COLOR, BLACK_PIECE, WHITE_PIECE


def all_to_black(element):
    if element < 120:
        return BLACK_PIECE[0]
    else:
        return WHITE_PIECE[0]


def all_to_white(element):
    if element > 235:
        return WHITE_PIECE[0]
    else:
        return BLACK_PIECE[0]


def brighter(screen, isBlack):
    new_screen = screen.copy()
    if isBlack:
        vfunc = np.vectorize(all_to_black)
    else:
        vfunc = np.vectorize(all_to_white)
    new_screen = vfunc(new_screen).astype(np.uint8)
    return new_screen


class ChessBoard:
    found = False
    pieces_found = False
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
        self.pieces_found = False
        if self.__min_pos[0] + 1 < self.piece_size or \
                self.__min_pos[1] + 1 < self.piece_size or \
                self.position_right_bottom[0] + 1 < self.__min_pos[0] + \
                self.piece_size * 8 or \
                self.position_right_bottom[1] + 1 < self.__min_pos[1] + \
                self.piece_size * 8:
            return

        if self.found:
            self.preprocess_board_screen(screen, False)
            self.preprocess_board_screen(screen, True)
            self.pieces_found = len(self.__pieces_white[0].positions) != 0 and \
                                len(self.__pieces_black[0].positions) != 0

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

    def writeBoard(self, screen):
        if self.found:
            self.empty_spaces.write(screen, (244, 99, 0))
        else:
            self.empty_spaces.write(screen, (0, 0, 255))

    def clean(self):
        for piece in self.__pieces_black:
            piece.positions.clear()
        for piece in self.__pieces_white:
            piece.positions.clear()
        self.empty_spaces.positions.clear()

    def writePieces(self, screen):
        for piece in self.__pieces_black:
            piece.write(screen)
        for piece in self.__pieces_white:
            piece.write(screen)

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
            giv_int = (i * 47) % (len(self.empty_spaces.positions) - 1)
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
        new_screen = brighter(screen, isBlack)
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
                    piece.find(tmp_cut, [
                        int(self.__min_pos[0] + self.piece_size * (i - 1) + 8),
                        int(self.__min_pos[1] + self.piece_size * (6 - j) + 4)])

    def get_side(self):
        return self.__pieces_white[0].positions[0][0] < \
               self.__pieces_black[0].positions[0][0]

    def get_hash(self, screen_to_read):
        if not self.found:
            return 0
        answ = 0
        for i in range(8):
            for j in range(8):
                answ += (i + j * 9) * screen_to_read[
                    int(self.__min_pos[
                            0] + self.piece_size * i + 14 - self.piece_size / 2),
                    int(self.__min_pos[
                            1] + self.piece_size * j + 5 - self.piece_size / 2)]
        return answ

    def get_color_board(self, isWhite, screen_to_read):
        if not self.found:
            return 0
        board_matrix = [['_' for x in range(8)] for y in range(8)]
        new_screen = brighter(screen_to_read, not isWhite)

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
                if isWhite:
                    board_matrix[i][j] = '*'
                else:
                    board_matrix[i][j] = '*'
        return board_matrix
