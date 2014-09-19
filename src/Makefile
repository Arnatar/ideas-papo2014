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

# for gif export: dependency: "imagemagick"
pygame: CFLAGS += -DDRAW
pygame: CFLAGS += -DFAIRMOVEMENT
pygame: CFLAGS += -DFAIRCHECKING
pygame: setup_draw_data_folder
pygame: procs=4
pygame: x=100
pygame: y=100
pygame: rounds=200
pygame: ideas=500
pygame: framerate=25
pygame: export_gif=1
pygame: outfile=out.gif
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


# MEASURING ------------------------------------------------------------
# setup instructions:
# 1. add the following to ~/.ssh/config :
# Host cluster
    # HostName cluster.wr.informatik.uni-hamburg.de
    # User <your-cluster-user-name>
# 2. Add your SSH-key to the cluster
# 2.1 if you don't have a ssh key:
#   ssh-keygen -t rsa -C "<your-email>"
# 2.2 ssh-copy-id cluster
# 3. set the right username ~10 lines down (cluster_username), and the other options optionally
# 4. Set a python_bin that has pygal installed

# is called by the measure script
cluster_build_measure: setup_draw_data_folder
# cluster_build_measure: CFLAGS += -DDRAW
cluster_build_measure: CFLAGS += -DMEASURE
cluster_build_measure: CFLAGS += -DFAIRMOVEMENT
cluster_build_measure: CFLAGS += -DFAIRCHECKING
cluster_measure cluster_build_measure: x=2000
cluster_measure cluster_build_measure: y=4000
cluster_measure cluster_build_measure: rounds=520
cluster_measure cluster_build_measure: ideas=200
cluster_build_measure: build_executable

# is called by YOU
cluster_measure: cluster_username=werner

cluster_measure: outfile=foo.csv
cluster_measure: outfile_graph=chart.svg

cluster_measure: absolute_scale=1
cluster_measure: relative_scale=0

cluster_measure: min_procs=2
cluster_measure: max_procs=32
cluster_measure: proc_increase_step=2

cluster_measure: python_bin=./.virtualenv/bin/python
cluster_measure: svg_viewer=google-chrome-stable
cluster_measure:
	mkdir -p measurements
	proc_increase_step=$(proc_increase_step) cluster_username=$(cluster_username) x=$(x) y=$(y) rounds=$(rounds) ideas=$(ideas) outfile=$(outfile) min_procs=$(min_procs) max_procs=$(max_procs) ./measure.sh
	absolute_scale=$(absolute_scale) relative_scale=$(relative_scale) $(python_bin) measurements/graph.py measurements/$(outfile_graph)
	$(svg_viewer) measurements/$(outfile_graph)

# -----------------------------------------------------------------------------------

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