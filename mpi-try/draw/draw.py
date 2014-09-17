from math import floor
from itertools import chain
from pprint import pprint
import pygame, sys
from pygame.locals import *
from pygame import draw
import os
from os.path import basename

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)

framerate=int(os.getenv("framerate", 15))

wd=os.path.dirname(os.path.realpath(__file__))
data_dir=os.path.join(wd,"data")
files = [data_dir + "/" + fname for fname in os.listdir(data_dir)]
files_sorted=sorted(files,key=lambda f: int(basename(f)))


# amount of pieces x/y
x=int(os.getenv("x",0))
y=int(os.getenv("y",0))

if y==0 or x==0:
    print("give x/y field length as environment variables")
    sys.exit()

AMOUNT = y if y >= x else x

y_resolution = 800

# size of one piece
SIZE=int(floor(y_resolution/AMOUNT))
if SIZE <= 0: 
    print("SIZE per idea too small")
    sys.exit()

class Draw():
    def __init__(self, files_sorted):

        self.files_sorted = files_sorted
        self.init_pygame()
        self.run()

    def init_pygame(self):
        pygame.init()
        pygame.display.set_caption('Hello world!')
        self.screen = pygame.display.set_mode((AMOUNT*SIZE,AMOUNT*SIZE), 0, 32)
        self.clock = pygame.time.Clock()

    def draw_point(self,x,y, qual):
        color=255-25*qual
        draw.rect(self.screen, (color,color,color), [x, y, SIZE, SIZE])

    def print_step(self, step_data):
        for y, row in enumerate(step_data):
            for x, col in enumerate(row):
                if col != [0,0,0,0]:
                    self.draw_point(SIZE*x,SIZE*y, col[0])

    def run(self):

        for fname in self.files_sorted:
            # get data from file ---------------------------------------------------
            with open(fname) as f:
                file_raw_data = f.read().split("\n")[:-1]

            step_data = [] 
            for line in file_raw_data:
                step_data.append([map(int,nums.strip().split(" ")) 
                            for nums in line.split(",")[:-1]])

            # draw on screen -------------------------------------------------------
            self.screen.fill((191,184,114))
            self.print_step(step_data)
            pygame.display.flip()

            self.clock.tick(framerate)

            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()


        pygame.quit()

# main -------------------------------------------------------------------------
Draw(files_sorted)



