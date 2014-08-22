/*
Author Julian
To compile run:
	mpicc -g mpi-par.c -std=c99
To run it:
 mpiexec -np 2 ./a.out |sort -n
 
 You can use only a number of processes that divides the x_size correctly, here it is 10,
 so you can run with -np 1, 2, 5 and 10

Output should always be:
new[10] = 111.111115
new[1] = 185.185181
new[2] = 185.185181
new[3] = 111.111115
new[4] = 37.037037
new[5] = 0.000000
new[6] = 0.000000
new[7] = 0.000000
new[8] = 0.000000
new[9] = 37.037037

*/

#include <mpi.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define DEBUG
// C pre-processor macro to ease printf.
#ifdef DEBUG
	#define debug(...) printf(__VA_ARGS__);
#else
	#define debug(...) 
#endif

int main(int argc, char ** argv){
	MPI_Init(& argc, & argv);
	int rank;
	int size;
  int comm = MPI_COMM_WORLD;
	MPI_Comm_rank(MPI_COMM_WORLD, & rank);
	MPI_Comm_size(MPI_COMM_WORLD, & size);

	// assume we have an array with 10 elements that should be processed in parallel.
	// note that we do not handle leftovers
	const int x_size = 10;

	int ret; // return values of functions.

	// we'll have 2 matrices, one with values from the old timestep and one with the new timestep
	// TODO fix the allocation to alloc only data needed by THIS  process.
	float * m1 = malloc(sizeof(float) * (x_size + 2));
	float * m2 = malloc(sizeof(float) * (x_size + 2));

	float * new = m1; // point to the matrix to update
	float * old = m2; // old matrix from which we read


	// pre-fill the array with 0
	memset(old, 0, sizeof(float) * (x_size + 2) );

	// the first element will be set to 1000 (assume e.g. a water wave with a high front that is leveled)	
	// data is aligned in a ring, thus element m[0] has the neighbor m[x_size-1];
	if (rank == 0){
		old[2] = 1000;	
	}
	
	// we split the ring into N elements
	
	int elementsPerProcess = x_size / size;

	// TODO: handle leftovers correctly...
	if (x_size % size != 0){
		printf("Error, the number of rows is not divisible by the number of processes!\n");
		exit(1);
	}

	// For async communication

	MPI_Request sendNext;
	MPI_Request recvNext;
	MPI_Request sendPrev;
	MPI_Request recvPrev;

	// the next rank in the rank, make sure wraparound works
	const int nextRank = rank == size - 1 ? 0 : rank + 1;
	const int prevRank = rank == 0 ? size -1 : rank - 1;
	

	// timestep loop, here we do JUST 3 timesteps...
#define TIMESTEPS 3

	for(int t = 0; t < TIMESTEPS; t++){

		// we update our points

		// parallel code
		// TODO: only allocate the memory needed
		// element [0] and element [elementsPerProcess] are the ghost cells.

		// we receive the border element from the next processor
		if ( t > 0 ){ // first step we do not receive
			ret = MPI_Irecv(& old[0], 1, MPI_FLOAT, prevRank, 4711, MPI_COMM_WORLD, & recvPrev);
			assert(ret == MPI_SUCCESS);

			ret = MPI_Irecv(& old[elementsPerProcess + 1], 1, MPI_FLOAT, nextRank, 8888, MPI_COMM_WORLD, & recvNext);
			assert(ret == MPI_SUCCESS);

			// here we wait until all communication completes => there are more efficient ways to achieve this...
			MPI_Wait(& sendPrev, MPI_STATUS_IGNORE);
			MPI_Wait(& sendNext, MPI_STATUS_IGNORE);
			MPI_Wait(& recvNext, MPI_STATUS_IGNORE);
			MPI_Wait(& recvPrev, MPI_STATUS_IGNORE);
		}

		// we just use the first n elements of the vector at the moment.

		for(int x = 1; x <= elementsPerProcess; x++){ 
			new[x] = (old[x] + old[x+1] + old[x-1]) / 3;
			debug("rank %d: new[%d] = %f\n", rank, x, new[x]);			
		}

		// send the newly computed values
		// in the last step we do not send...
		if ( t < TIMESTEPS - 1 ){
			ret = MPI_Isend(& new[1], 1, MPI_FLOAT, prevRank, 8888, MPI_COMM_WORLD, & sendPrev);
			assert(ret == MPI_SUCCESS);

			ret = MPI_Isend(& new[elementsPerProcess], 1, MPI_FLOAT, nextRank, 4711, MPI_COMM_WORLD, & sendNext);
			assert(ret == MPI_SUCCESS);
		}

		float * tmp = old;
		old = new;
		new = tmp;
	}

	// control output:
	// TODO sort output or send it to rank 0
	for(int x = 1; x <= elementsPerProcess; x++){ 
		int real_pos = x + elementsPerProcess * rank;
		printf("new[%d] = %f\n", real_pos, old[x]);
	}


	free(m1);
	free(m2);

	MPI_Finalize();
}



