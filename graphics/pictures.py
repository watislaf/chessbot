import os
from pathlib import Path

print(__file__)
PICTURES_PATH = '\\'.join(os.path.realpath(__file__).split("\\")[:-1])
