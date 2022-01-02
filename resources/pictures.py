import os

import sys

if hasattr(sys, '_MEIPASS'):
    RESOURCES_PATH=sys._MEIPASS
else:
    RESOURCES_PATH = '/'.join(__file__.split("/")[:-1])

GRAY_COLOR = [122, 122, 122]
WHITE_BOARD_COLOR = [210, 238, 238]
BLACK_BOARD_COLOR = [86, 150, 118]
BLACK_PIECE = [0, 0, 0]
WHITE_PIECE = [255, 255, 255]
