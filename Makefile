# f=main.c
f=array.c lib/helpers.c logic.c idea.c
# f=randarray.c

# for headers
include_paths=-Ilib/ -Iheaders/lib/ -Iheaders/

library_paths=-Llib
args=
folder=mpi-try
num_threads=1

# julian:
	# cd $(folder) && mpicc -std=c99 $(f) && mpirun -np $(num_threads) ./a.out | sort -n

all:
	cd $(folder) && mpicc -std=c99 $(f) $(library_paths) $(include_paths) -include allheads.h && mpirun -np $(num_threads) ./a.out $(args)


