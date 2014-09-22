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
from collections import Counter

black = (0, 0, 0)
hirnatian = (166,219,171)

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

y_resolution = 700  

unique_ideas = set()
all_ideas = []

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
	pygame.display.set_caption('Ideas')
        self.screen = pygame.display.set_mode((AMOUNT*SIZE+200,AMOUNT*SIZE), 0, 32)
        self.clock = pygame.time.Clock()
        self.completed_rounds = 0
	
	
    def color_scale_intensity(self, value):
        # higher value = darker
        return 255 - 25*value
    
    def draw_unique_ideas(self,step_data):
	basicfont = pygame.font.SysFont(None, 30)
	text = basicfont.render("Unique ideas: %s" % self.count_unique_ideas(step_data), 1,hirnatian)
	self.screen.blit(text,(AMOUNT*SIZE,AMOUNT*SIZE/2)) 
 
    def draw_most_commons(self, step_data) :
	list = self.count_occurrences(step_data)	
	idea1 = list[0]
	idea2 = list[1]
	idea3 = list[2]
	basicfont = pygame.font.SysFont(None, 30)
	text = basicfont.render("Most commons:", 1,hirnatian)
	text1 = basicfont.render(str(idea1), 1, hirnatian)
	text2 = basicfont.render(str(idea2), 1, hirnatian)
	text3 = basicfont.render(str(idea3), 1, hirnatian)
	self.screen.blit(text,(AMOUNT*SIZE,AMOUNT*SIZE/2 + 30))
	self.screen.blit(text1,(AMOUNT*SIZE,AMOUNT*SIZE/2 + 60))
	self.screen.blit(text2,(AMOUNT*SIZE,AMOUNT*SIZE/2 + 90))
	self.screen.blit(text3,(AMOUNT*SIZE,AMOUNT*SIZE/2 + 120))
	

    def draw_point(self,x,y, idea,step_data):
        quali, complx, wv, human_wv = idea
        color = dim(colors[wv], quali/9)
        draw.rect(self.screen, color, [x, y, SIZE, SIZE])

    def print_step(self, step_data):
        for y, row in enumerate(step_data):
            for x, col in enumerate(row):
                if col != [0,0,0,0]:
                    self.draw_point(SIZE*x,SIZE*y, col,step_data)
	self.draw_unique_ideas(step_data)
	self.draw_most_commons(step_data)

    def count_unique_ideas(self, step_data):
	unique_ideas.clear()
	for x, row in enumerate(step_data):
	    for y, idea in enumerate(row):
		unique_ideas.add(tuple(idea))
	return len(unique_ideas) - 1 #(0,0,0,0)

    def count_occurrences(self, step_data):
	all_ideas = []
	for x, row in enumerate(step_data):
	    for y, idea in enumerate(row):
		if idea != [0,0,0,0]:
		    all_ideas.append(tuple(idea))
	return  Counter(all_ideas).most_common(3)

    def run(self):
        unique_wvs_last=10

        for round_ in range(rounds):
            # t0 = time.clock()

            # get data from file ---------------------------------------------------
            fnames_for_round = ["{}-{}".format(round_,i) for i in range(procs)]
            pool = Pool()
            step_data =list(chain(*pool.map(read_step_file, fnames_for_round)))
            pool.close()
            pool.join()

	    self.count_unique_ideas(step_data)
	    self.count_occurrences(step_data)	   
 
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




