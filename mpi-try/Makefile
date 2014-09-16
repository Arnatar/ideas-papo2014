f=main.c lib/helpers.c logic.c idea.c

# for headers
include_paths=-Ilib/ -Iheaders/lib/ -Iheaders/ -I.

library_paths=-lrt -Llib
procs=4

CFLAGS=-std=c99

# this is the default config, runs when you call "make"
all: debug

debug: CFLAGS += -DDEBUG -g
debug: CFLAGS += -DFAIRCHECKING
debug: CFLAGS += -DFAIRMOVEMENT
debug: build_executable
debug: procs=2
debug: x=4
debug: y=20
debug: rounds=100
debug: ideas=10
debug: run


build_executable: setup_folders
	mpicc $(CFLAGS) $(f) $(library_paths) $(include_paths) -include allheads.h 

setup_folders:
	mkdir -p out 
	mkdir -p log 
	rm -rf log/* 
	rm -rf out/*

run:
	x=$(x) y=$(y) ideas=$(ideas) rounds=$(rounds) mpirun -np $(procs) ./a.out

