f=main.c lib/helpers.c logic.c idea.c

# for headers
include_paths=-Ilib/ -Iheaders/lib/ -Iheaders/ -I.

library_paths=-lrt -Llib
procs=4

CFLAGS=-std=c99

# this is the default config, runs when you call "make"
all: debug

debug: CFLAGS += -DDEBUG -g
debug: build_executable
debug: procs=2
debug: x=4
debug: y=24
debug: rounds=100
debug: ideas=3
debug: run

pygame: CFLAGS += -DDRAW
# pygame: CFLAGS += -DDRAW
pygame: setup_draw_data_folder
pygame: build_executable
pygame: procs=2
pygame: x=50
pygame: y=50
pygame: rounds=1000
pygame: ideas=30
pygame: run

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

