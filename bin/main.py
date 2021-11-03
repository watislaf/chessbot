import os

from bin.FEN import FEN
from bin.controller import Controller
from tools.move_validator import MoveValidator

controller = Controller()

if __name__ == "__main__":
    # garik part
    controller.startView()
    MoveValidator()
    # bots play
