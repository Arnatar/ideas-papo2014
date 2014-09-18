from __future__ import division
from math import floor
from itertools import chain
from pprint import pprint
import pygame, sys
from pygame.locals import *
from pygame import draw
from pygame.color import Color
import os
from os.path import basename

black = (0, 0, 0)

blue="#0074D9"
teal="#39CCCC"

yellow="#FFDC00"
orange="#FF851B"
red="#FF4136"

maroon="#85144B"
fuchsia="#F012BE"

olive="#3D9970"
lime="#01FF70"

grey="#AAAAAA"

colors=map(Color, [blue, teal, yellow, orange, red, maroon, fuchsia, olive, lime, grey])

def dim(color, factor):
    return tuple([int(c*factor) for c in color])

# print(dim(colors[0],1))
# print(dim(colors[0],.5))

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

    def color_scale_intensity(self, value):
        # higher value = darker
        return 255 - 25*value


    def draw_point(self,x,y, idea):
        quali, complx, wv, human_wv = idea
        # R=self.color_scale_intensity(complx)
        # R=self.color_scale_intensity(complx)
        color = dim(colors[wv], quali/9)
        draw.rect(self.screen, color, [x, y, SIZE, SIZE])

    def print_step(self, step_data):
        for y, row in enumerate(step_data):
            for x, col in enumerate(row):
                if col != [0,0,0,0]:
                    self.draw_point(SIZE*x,SIZE*y, col)

    def run(self):
        unique_wvs_last=10

        for step_nr, fname in enumerate(self.files_sorted):
            # get data from file ---------------------------------------------------
            with open(fname) as f:
                file_raw_data = f.read().split("\n")[:-1]

            step_data = [] 
            for line in file_raw_data:
                step_data.append([map(int,nums.strip().split(" ")) 
                            for nums in line.split(",")[:-1]])

            # draw on screen -------------------------------------------------------
            self.screen.fill(black)
            self.print_step(step_data)
            pygame.display.flip()

            self.clock.tick(framerate)

            unique_wvs = self.unique_wvs(step_data)
            if unique_wvs_last and unique_wvs != unique_wvs_last:
                print("round {}: {} unique wv's".format(
                    step_nr,self.unique_wvs(step_data)))
            unique_wvs_last = unique_wvs

            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()


        pygame.quit()

    def unique_wvs(self, step_data):
        return len(set(idea[2] for idea in chain(*step_data)))


# main -------------------------------------------------------------------------
Draw(files_sorted)




