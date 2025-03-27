import os
import numpy as np
import pygame
os.add_dll_directory(r"C:\ProgramData\mingw64\mingw64\bin")
from build.cppTetris import Tetris, Tetromino, init_tetrominoes
import random

pieces = init_tetrominoes()

SDF = 20.0 # soft drop factor
DAS = 10 # Delayed auto shift
ARR = 80 # Auto repeat rate

colors = [
    (0, 0, 0),#BLACK
    (98, 255, 245),#I
    (32, 24, 255),#J
    (252, 144, 21),#L
    (42, 245, 83),#S
    (255, 0, 0),#Z
    (255, 24, 247),#T
    (255, 208, 2),#O
]
shadows_colors = [
    (0, 0, 0),#BLACK
    (98/3, 255/3, 245/3),#I
    (32/3, 24/3, 255/3),#J
    (252/3, 144/3, 21/3),#L
    (42/3, 245/3, 83/3),#S
    (255/3, 0, 0),#Z
    (255/3, 24/3, 247/3),#T
    (255/3, 208/3, 2/3),#O
]

keys = {
    'up': pygame.K_UP,
    'down': pygame.K_DOWN,
    'left': pygame.K_LEFT,
    'right': pygame.K_RIGHT,
    'rotatec': pygame.K_z,
    'rotatecc': pygame.K_x,
    'rotatec180': pygame.K_a,
    'hold': pygame.K_c,
    'pause': pygame.K_ESCAPE,
    'drop': pygame.K_SPACE,
    'reset': pygame.K_r
}

pygame.init()
# Define some colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GRAY = (128, 128, 128)

size = (500, 500)
board_scale = 20
screen = pygame.display.set_mode(size)

done = False
clock = pygame.time.Clock()
fps = 144
gravityDelay = 2
game = Tetris()
counter = 0

pressing_down = False
pressing_left = False
pressing_right = False
move_down_timer = 0
move_left_timer = 0
move_right_timer = 0

continued_move_left = 0
continued_move_right = 0    


while not done:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        elif event.type == pygame.KEYDOWN:
            if event.key == keys['rotatecc']:
                game.rotate(1)
            if event.key ==  keys['rotatec']:
                game.rotate(-1)
            if event.key == keys['rotatec180']:
                game.rotate(2)
                
            if event.key == keys['down']:
                pressing_down = True
                move_down_timer = 0
            if event.key == keys['left']:
                game.move(-1)
                continued_move_left = 0
                pressing_left = True
                move_left_timer = 0
            if event.key == keys['right']:
                game.move(1)
                continued_move_right =0
                pressing_right = True
                move_right_timer = 0 
                
            if event.key == keys['drop']:
                game.drop()
            if event.key == keys['reset']:
                game = Tetris()
            if event.key == keys['hold']:
                game.hold_piece()
                
        elif event.type == pygame.KEYUP:
            if event.key == keys['down']:
                pressing_down = False
            if event.key == keys['left']:
                continued_move_left = 0
                pressing_left = False
            if event.key == keys['right']:
                continued_move_right = 0
                pressing_right = False
    
    counter += 1
    if counter % (fps//gravityDelay) == 0:
        game.fall()

        
    if pressing_down:
        move_down_timer += 1
    if pressing_down and move_down_timer % ((fps//20)+1) == 1:
        game.soft_drop(int(SDF))
     
    if pressing_left:
        continued_move_left += 1
        move_left_timer += 1
    if pressing_left and move_left_timer % ((fps//ARR)+1) == 1 and continued_move_left > fps//DAS:
        game.move(-1)
        
    if pressing_right:
        continued_move_right += 1
        move_right_timer += 1
    if pressing_right and move_right_timer % ((fps//ARR)+1) == 1 and continued_move_right > fps//DAS:
        game.move(1)
        
    if random.randint(0, 2000) == 0:
        game.recive_lines(2)
        
    screen.fill(BLACK)
    board = game.getBoard()
    x= 100
    y = 60
    
    y_focus = 20
    
    #board
    for i in range(int(len(board)/2)):
        for j in range(len(board[i])):
            pygame.draw.rect(screen, GRAY, [x + board_scale * j, y + board_scale * i, board_scale, board_scale], 1)
            
            if board[y_focus+i][j] != 0:
                pygame.draw.rect(screen, colors[board[i+y_focus][j]],
                                 [x + board_scale * j + 1, y + board_scale * i + 1, board_scale - 2, board_scale - 1])
    
    #shadow
    shadow = game.shadow_image()
    for i in range(len(shadow.getImage())):
        for j in range(len(shadow.getImage())):
            if shadow.getImage()[i][j]:
                pygame.draw.rect(screen, shadows_colors[shadow.getType()+1],
                                    [x + board_scale * (j + shadow.getX()) + 1,
                                    y + board_scale * (i + shadow.getY()-y_focus) + 1,
                                    board_scale - 2, board_scale - 2])
    
    #actual piece
    for i in range(len(game.getCurrent().getImage())):
        for j in range(len(game.getCurrent().getImage())):
            if game.getCurrent().getImage()[i][j]:
                pygame.draw.rect(screen, colors[game.getCurrent().getType()+1],
                                    [x + board_scale * (j + game.getCurrent().getX()) + 1,
                                    y + board_scale * (i + game.getCurrent().getY()-y_focus) + 1,
                                    board_scale - 2, board_scale - 2])
    
    #hold piece
    hold_image = pieces[game.getHold()][0]
    if game.getHold() != -1:
        for i in range(len(hold_image)):
            for j in range(len(hold_image[i])):
                if hold_image[i][j] > 0:
                    pygame.draw.rect(screen, colors[game.getHold()+1],
                                    [x + board_scale * (j + -4) + 1,
                                    y + board_scale * (i) + 1,
                                    board_scale - 2, board_scale - 2])

    #next pieces
    nexts_images = [] # 5 pieces
    for i in range(5):
        nexts_images.insert(i,pieces[game.getNexts()[i]][0])
    
    for i, next_image in enumerate(nexts_images):
        for j in range(len(next_image)):
            for k in range(len(next_image[j])):
                if next_image[j][k]:
                    pygame.draw.rect(screen, colors[nexts_images[i][j][k]],
                                     [x + board_scale * (k + 10) + 1,
                                      y + board_scale * (j + 4*i) + 1,
                                      board_scale - 2, board_scale - 2])
                    
    #score
    font = pygame.font.Font(None, 36)
    text = font.render("Score: " + str(game.getScore()), True, WHITE)
    screen.blit(text, (x + board_scale * 10, y - board_scale))
        
        
    pygame.display.flip()
    clock.tick(fps)
pygame.quit()