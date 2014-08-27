f=main.c lib/helpers.c logic.c idea.c

# for headers
include_paths=-Ilib/ -Iheaders/lib/ -Iheaders/ -I.

library_paths=-lrt -Llib
args=
folder=mpi-try
procs=4

# julian:
	# cd $(folder) && mpicc -std=c99 $(f) && mpirun -np $(num_threads) ./a.out | sort -n

all:
	cd $(folder) && mkdir -p out && mkdir -p log && rm -rf log/* && rm -rf out/* && mpicc -pg -std=c99 $(f) $(library_paths) $(include_paths) -include allheads.h && mpirun -np $(procs) ./a.out $(args)

slurm:
	cd $(folder) && mkdir -p out && mkdir -p log && rm -rf log/* && rm -rf out/* && mpicc -pg -std=c99 $(f) $(library_paths) $(include_paths) -include allheads.h
