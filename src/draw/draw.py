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

# stats ------------------------------------------------------------------------
compare_wv_counts=1
compare_avg_qual_per_wv=1

STATS_BRIGHTS="% qual>5"
STATS_AVG_QUAL="avg qual"
STATS_AVG_COMPL="avg complex"
STATS_UNIQUE_WVS="unique wvs"

black = (0, 0, 0)
white = (255,255,255)

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

colors=map(Color, [blue, teal, olive, lime, yellow, orange, red, maroon, fuchsia, grey])

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
num_ideas=int(os.getenv("ideas",0))

if y==0 or x==0 or rounds==0 or procs==0 or num_ideas==0 :
    print("need x,y,ideas, rounds and procs env vars")
    sys.exit()

# if os.path.isfile(outfile):
#     if raw_input("\nthe file '{}' already exists. overwrite? [y/n] => ".format(
#         basename(outfile))) == "n": sys.exit()

AMOUNT = y if y >= x else x

y_resolution = 700  
x_size_graphs = 600
padding_graph = 30

# unique_ideas = set()
# all_ideas = []

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

        self.field_length = AMOUNT*SIZE
        self.screen = pygame.display.set_mode(
                (self.field_length+x_size_graphs,self.field_length), 0, 32)
        self.clock = pygame.time.Clock()
        self.completed_rounds = 0

        self.graph_box_length = x_size_graphs-padding_graph*2
        self.left_bottom_corner_of_graph = \
            (self.field_length + padding_graph, self.field_length - padding_graph)
        self.font = pygame.font.SysFont(None, 24)

# colors=map(Color, [blue, teal, yellow, orange, red, maroon, fuchsia, olive, lime, grey])
        self.legend_items = {
                    STATS_AVG_QUAL: (olive, self.avg_quality)
                  , STATS_AVG_COMPL: (orange, self.avg_complexity)
                  , STATS_BRIGHTS: (yellow , self.amount_brights)
                  , STATS_UNIQUE_WVS: (blue, self.unique_wvs)
            }

        self.draw_graph_box_and_legend()

    def draw_point(self,x,y, idea):
        quali, complx, wv, human_wv = idea
        color = dim(colors[wv], quali/9)
        draw.rect(self.screen, color, [x, y, SIZE, SIZE])

    def print_step(self):
        for y, row in enumerate(self.step_data):
            for x, idea in enumerate(row):
                if idea != [0,0,0,0]:
                    self.draw_point(SIZE*x,SIZE*y, idea)

    def draw_graph_point(self, legend_item_name, func=None, wv=None):
        if legend_item_name:
            color_hex, func = self.legend_items[legend_item_name]
            color = Color(color_hex)
            value, max_value = func()
        else:
            value, max_value = func(wv)
            color = colors[wv]

        round_ = self.completed_rounds

        offset_x = int((round_/rounds) * self.graph_box_length)
        offset_y = int((value/max_value) * self.graph_box_length)

        x = self.left_bottom_corner_of_graph[0] + offset_x
        y = self.left_bottom_corner_of_graph[1] - offset_y
        draw.rect(self.screen, color, [x , y, 2, 2])

    def draw_text(self, txt, x,y):
        x_size,_ = self.font.size(txt)
        color = (200,200,200)
        self.screen.blit(self.font.render(txt, True, color), (x, y))
        return x_size

    def draw_graph_box_and_legend(self):
        x_end = self.left_bottom_corner_of_graph[0] + self.graph_box_length
        # y_end = self.left_bottom_corner_of_graph[1] - self.graph_box_length
        color = (30,30,30)
        draw.line(self.screen, color, 
                    self.left_bottom_corner_of_graph,
                    (self.left_bottom_corner_of_graph[0] + self.graph_box_length,
                        self.left_bottom_corner_of_graph[1]))
        draw.line(self.screen, color, 
                    self.left_bottom_corner_of_graph,
                    (self.left_bottom_corner_of_graph[0],
                        self.left_bottom_corner_of_graph[1] - self.graph_box_length))
        # legend
        x = self.field_length+padding_graph
        y = 10
        if not compare_wv_counts:
            rectsize = 15
            for legend_item_name, (legend_item_color,_) in self.legend_items.items():
                draw.rect(self.screen, Color(legend_item_color), [x,y, rectsize,rectsize])
                x += rectsize + 10
                x_size_text = self.draw_text(legend_item_name, x, y)
                x += x_size_text + 20
        elif compare_avg_qual_per_wv==1:
            self.draw_text("Average quality per WV",x,y)
        else:
            self.draw_text("WV distribution",x,y)
                    

    def run(self):
        unique_wvs_last=10

        for round_ in range(rounds):
            self.completed_rounds = round_
            # get data from file ---------------------------------------------------
            fnames_for_round = ["{}-{}".format(round_,i) for i in range(procs)]
            pool = Pool()

            self.step_data =list(chain(*pool.map(read_step_file, fnames_for_round)))
            pool.close()
            pool.join()
 
            # draw on screen -------------------------------------------------------
            self.screen.fill(black, rect=(0,0,self.field_length,self.field_length))
            self.print_step()

            if not compare_wv_counts:
                self.draw_graph_point(STATS_AVG_QUAL)
                self.draw_graph_point(STATS_AVG_COMPL)
                self.draw_graph_point(STATS_BRIGHTS)
                self.draw_graph_point(STATS_UNIQUE_WVS)
            else:
                for wv in range(10): 
                    self.draw_graph_point(None, self.wv_count, wv)

            pygame.display.flip()

            self.clock.tick(framerate)

            self.output_image(round_)

            event = pygame.event.poll()
            if event.type == QUIT or event.type == KEYDOWN and event.key == K_ESCAPE:
                self.quit()
                break

        if export: self.generate_animation(self.completed_rounds)
        self.quit()

    def wv_count(self, wv):
        if not compare_avg_qual_per_wv:
            return (len([idea[2] for idea in chain(*self.step_data) 
                if idea[2]==wv and idea != [0,0,0,0]]),
                    num_ideas)
        else:
            wv_filtered = [idea for idea in chain(*self.step_data) 
                    if idea[2]==wv and idea != [0,0,0,0] ]
            len_wv_filtered = len(wv_filtered)
            if len_wv_filtered:
                return (sum(idea[0] for idea in wv_filtered) / len(wv_filtered), 9)
            else:
                return (0,9)

    def unique_wvs(self):
        return (len(set(idea[2] for idea in chain(*self.step_data))),
                10)

    def avg_quality(self):
        return (sum(idea[0] for idea in chain(*self.step_data) 
                if idea != [0,0,0,0]) / num_ideas, 
                9)

    def avg_complexity(self):
        return (sum(idea[1] for idea in chain(*self.step_data) 
                if idea != [0,0,0,0]) / num_ideas, 
                9)

    def amount_brights(self):
        return (sum(1 for idea in chain(*self.step_data) 
                if idea[0] > 5 ), 
                num_ideas)

    def output_image(self, round_):
        fname=os.path.join(tmp_dir, "%05d" % round_ + ".png")
        pygame.image.save(self.screen, fname)

    def generate_animation(self, round_=rounds):
        print("\nGenerating animation into {}...\n\n".format(outfile))
        filename_list = ["{}.png".format(i) for i in range(round_)]
        command_list = ['ffmpeg', '-r', str(framerate), '-i', '%05d.png', outfile]
        subprocess.call(command_list, cwd=tmp_dir)

    def quit(self):
        pygame.quit()






# main -------------------------------------------------------------------------
clean_tmp_images()
Draw(files_sorted)




