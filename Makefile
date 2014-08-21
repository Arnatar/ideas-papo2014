# f=main.c
f=array.c lib/helpers.c logic.c idea.c
# f=randarray.c
headers=lib/
include_paths=lib/
args=
folder=mpi-try
num_threads=1

# julian:
	# cd $(folder) && mpicc -std=c99 $(f) && mpirun -np $(num_threads) ./a.out | sort -n

all:
	cd $(folder) && mpicc -std=c99 $(f) -L$(include_paths) -I$(headers) -include allheads.h && mpirun -np $(num_threads) ./a.out $(args)


