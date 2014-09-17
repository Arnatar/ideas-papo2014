f=main.c lib/helpers.c logic.c idea.c

# for headers
include_paths=-Ilib/ -Iheaders/lib/ -Iheaders/ -I.

library_paths=-lrt -Llib
procs=4

CFLAGS=-std=c99 -O3

# this is the default config, runs when you call "make"
all: debug

debug: CFLAGS += -DDEBUG -g
debug: CFLAGS += -DFAIRMOVEMENT
debug: CFLAGS += -DFAIRCHECKING
debug: build_executable
debug: procs=2
debug: x=2
debug: y=8
debug: rounds=150
debug: ideas=2
debug: run

pygame: CFLAGS += -DDRAW
pygame: CFLAGS += -DFAIRMOVEMENT
pygame: CFLAGS += -DFAIRCHECKING
# pygame: CFLAGS += -DDRAW
pygame: setup_draw_data_folder
pygame: build_executable
pygame: procs=2
pygame: x=100
pygame: y=100
pygame: rounds=2500
pygame: ideas=400
pygame: framerate=40
pygame: run
pygame: run_draw

build_executable: setup_folders
	mpicc $(CFLAGS) $(f) $(library_paths) $(include_paths) -include allheads.h 


setup_draw_data_folder:
	mkdir -p draw/data
	rm -rf draw/data/*

setup_folders:
	mkdir -p out 
	mkdir -p log 
	rm -rf log/* 
	rm -rf out/*

run:
	x=$(x) y=$(y) ideas=$(ideas) rounds=$(rounds) mpirun -np $(procs) ./a.out

run_draw:
	x=$(x) y=$(y) framerate=$(framerate) python2 draw/draw.py
	
