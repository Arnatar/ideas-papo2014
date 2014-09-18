from __future__ import division

import time
from multiprocessing import Pool
from itertools import chain
from pprint import pprint
from math import floor

import pygame, sys
from pygame.locals import *
from pygame import draw
from pygame.color import Color

import os
from os.path import basename
import subprocess
import glob

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

framerate=int(os.getenv("framerate", 15))

wd=os.path.dirname(os.path.realpath(__file__))
data_dir=os.path.join(wd,"data")
tmp_dir=os.path.join(wd,"tmp")
out_dir=os.path.join(wd,"out")

files = os.listdir(data_dir)
files_sorted=sorted(files,
        key=lambda f: (int(f.split("-")[0]),int(f.split("-")[1])))

# amount of pieces x/y
x=int(os.getenv("x",0))
y=int(os.getenv("y",0))
rounds=int(os.getenv("rounds",0))
procs=int(os.getenv("procs",0))
outfile=os.path.join(out_dir, os.getenv("outfile","out.gif"))
export=int(os.getenv("export_gif",0))

if y==0 or x==0 or rounds==0 or procs==0:
    print("need x,y,rounds and procs env vars")
    sys.exit()

# if os.path.isfile(outfile):
#     if raw_input("\nthe file '{}' already exists. overwrite? [y/n] => ".format(
#         basename(outfile))) == "n": sys.exit()

AMOUNT = y if y >= x else x

y_resolution = 800

# size of one piece
SIZE=int(floor(y_resolution/AMOUNT))
if SIZE <= 0: 
    print("SIZE per idea too small")
    sys.exit()

# -------------- FUNC -----------------------
def read_step_file(fname):
    step_data = [] 
    with open(os.path.join(data_dir, fname)) as f:
        for line in f:
            step_data.append([map(int,nums.strip().split(" ")) 
                        for nums in line.split(",")[:-1]])
    return step_data

def clean_tmp_images():
    for f in glob.glob(os.path.join(tmp_dir, "*")): os.remove(f)

# -------------- CLASS -----------------------
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
        self.completed_rounds = 0

    def color_scale_intensity(self, value):
        # higher value = darker
        return 255 - 25*value


    def draw_point(self,x,y, idea):
        quali, complx, wv, human_wv = idea
        color = dim(colors[wv], quali/9)
        draw.rect(self.screen, color, [x, y, SIZE, SIZE])

    def print_step(self, step_data):
        for y, row in enumerate(step_data):
            for x, col in enumerate(row):
                if col != [0,0,0,0]:
                    self.draw_point(SIZE*x,SIZE*y, col)

    def run(self):
        unique_wvs_last=10

        for round_ in range(rounds):
            # t0 = time.clock()

            # get data from file ---------------------------------------------------
            fnames_for_round = ["{}-{}".format(round_,i) for i in range(procs)]
            pool = Pool()
            step_data = chain(*pool.map(read_step_file, fnames_for_round))
            pool.close()
            pool.join()

            # print time.clock() - t0
            # draw on screen -------------------------------------------------------
            # t0 = time.clock()
            self.screen.fill(black)
            self.print_step(step_data)

            pygame.display.flip()
            # print time.clock() - t0
            # print ""

            self.clock.tick(framerate)

            # unique_wvs = self.unique_wvs(step_data)
            # if unique_wvs_last and unique_wvs != unique_wvs_last:
            #     print("round {}: {} unique wv's".format(
            #         round_,self.unique_wvs(step_data)))
            # unique_wvs_last = unique_wvs

            self.output_image(round_)

            self.completed_rounds = round_
            event = pygame.event.poll()
            if event.type == QUIT or event.type == KEYDOWN and event.key == K_ESCAPE:
                self.quit()
                break

        if export: self.generate_animation(self.completed_rounds)
        # print("\nOpening in Chrome...\n")
        # subprocess.call(["google-chrome-stable", outfile])
        self.quit()

    def unique_wvs(self, step_data):
        return len(set(idea[2] for idea in chain(*step_data)))

    def output_image(self, round_):
        fname=os.path.join(tmp_dir, str(round_) + ".png")
        pygame.image.save(self.screen, fname)

    def generate_animation(self, round_=rounds):
        print("\nGenerating animation into {}...\n\n".format(outfile))
        frame_delay = str(int(100/framerate))
        filename_list = ["{}.png".format(i) for i in range(round_)]
        command_list = ['convert', '-delay', frame_delay, '-loop', '0'] + \
                       filename_list + [outfile]
        subprocess.call(command_list, cwd=tmp_dir)

    def quit(self):
        pygame.quit()






# main -------------------------------------------------------------------------
clean_tmp_images()
Draw(files_sorted)




