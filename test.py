import os
import numpy as np
import pygame
os.add_dll_directory(r"C:\ProgramData\mingw64\mingw64\bin")
from build.cppTetris import Tetris, Tetromino, init_tetrominoes
import random

pieces = init_tetrominoes()

game = Tetris()

game.get_state()