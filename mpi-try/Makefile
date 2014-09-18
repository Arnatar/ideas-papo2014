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
debug: x=4
debug: y=20
debug: rounds=150
debug: ideas=20
debug: run

# pygame: CFLAGS += -DDRAW
pygame: CFLAGS += -DFAIRMOVEMENT
pygame: CFLAGS += -DFAIRCHECKING
pygame: setup_draw_data_folder
pygame: procs=4
pygame: x=2000
pygame: y=4000
pygame: rounds=120
pygame: ideas=200
pygame: framerate=15
pygame: export_gif=0
pygame: outfile=out2.gif
pygame: build_executable
pygame: run
pygame: run_draw

# the build script for the cluster
# (is not meant to be called locally. gets called by ./cluster-draw.sh
cluster: CFLAGS += -DDRAW
cluster: CFLAGS += -DFAIRMOVEMENT
cluster: CFLAGS += -DFAIRCHECKING
cluster: setup_draw_data_folder
cluster pygame_cluster: x=2000
cluster pygame_cluster: y=4000
cluster pygame_cluster: rounds=120
cluster pygame_cluster: ideas=200
cluster pygame_cluster: procs=32
cluster: build_executable
cluster: run

# the command to run the build on the cluster and get pygame happening locally
pygame_cluster: setup_draw_data_folder
	procs=$(procs) ./cluster-draw.sh

build_executable: setup_folders
	mpicc $(CFLAGS) $(f) $(library_paths) $(include_paths) -include allheads.h 


setup_draw_data_folder:
	mkdir -p draw/data
	mkdir -p draw/tmp
	mkdir -p draw/out
	rm -rf draw/data/*

setup_folders:
	mkdir -p out 
	mkdir -p log 
	rm -rf log/* 
	rm -rf out/*

run:
	x=$(x) y=$(y) ideas=$(ideas) rounds=$(rounds) mpirun -np $(procs) ./a.out

run_draw:
	x=$(x) y=$(y) export_gif=$(export_gif) framerate=$(framerate) rounds=$(rounds) procs=$(procs) outfile=$(outfile) python2 draw/draw.py
