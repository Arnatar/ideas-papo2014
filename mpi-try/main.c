#include "constants.h"
#include "idea.h"
#include "logic.h"

void get_distribution(int distribution[], int nranks, int global_amount) {
  int base_amount = global_amount / nranks;
  int rest_amount = global_amount % nranks;

  int rests[nranks];
  for_every(i, nranks, rests[i] = (i < rest_amount ? 1 : 0));

  for_every(i, nranks, distribution[i] = base_amount + rests[i]);
}

void mpi() {
  mpi_init(); // rank, num_ranks

  if (num_ranks % 2 != 0) {
    master(fputs("Use an even amount of processes for now.\n\n", stderr));
    exit(-1);
  }

  // initializations -----------------------------------------------------------
  mpi_define_idea_type();

  init_time_measurement_variables();


  int row_amount_distribution[num_ranks];
  get_distribution(row_amount_distribution, num_ranks, Y_SIZE);

  int ideas_amount_distribution[num_ranks];
  get_distribution(ideas_amount_distribution, num_ranks, NUM_IDEAS);

  // add space for ghost rows on top and bottom
  int num_rows = row_amount_distribution[rank] + 2;
  int num_ideas = ideas_amount_distribution[rank];

  // rank wraparound
	const int next_rank = rank == num_ranks - 1 ? 0 : rank + 1;
	const int prev_rank = rank == 0 ? num_ranks -1 : rank - 1;

  srand(time(NULL)+rank);

  tic();

  Idea field[num_rows][X_SIZE];
  fill_matrix_with(field, num_rows, idea_empty());

  toc("fill field with zero ideas");


  tic();
  // spawn ideas (the random y-value for field excludes the ghost rows
  for_every(i, num_ideas, 
      field[rand_int(num_rows-2,1)][rand_int(X_SIZE,0)] = idea_new());

  toc("fill with random ideas");

  // fill all ghost rows
  MPI_Request req, req2, req3, req4;
  tic();
  send_real_rows_to_ghost_rows();
  receive_real_rows_into_ghost_rows();
  toc("sending to ghost rows");

  barrier();

  // get filename for rank ("out/$rank")
  char fname[100];
  get_fname(fname, rank);
  FILE *fp;


#define ROUNDS 3
  for_every(i, ROUNDS, {
  tic();
  // movement ------------------------------------------------------------------
  // serial: move all ideas which do not depend on other ranks. 
  // then: 1) move all outer ideas from even ranks. 
  //       1b) communicate to uneven ranks.
  //       2) move all outer ideas from uneven ranks
  //       2b) communicate to even ranks.
  // send ideas ----------------------------------------------------------------


  // INDEPENDENT MOVEMENT
  // reason for 7: 1 row in center, can move up or down without touching a dependent
  // row. from the outer sides: 1 ghost row, 2 dependent rows each = 3*2. the next
  // row above 6 is the first independent row. 
  // [2]: give the independent movement procedure all rows starting from the 
  //      third, until the third last. if num_rows = 8, num_rows-4 = 4. that means:
  // take 3 rows from the array (the for loop is not inclusive).
  // so for a matrix with 8 rows we take (0-based) rows 2,3,4,5. 0+1 and 6+7 are 
  // left out.
  if (num_rows >= 7) {
    move_ideas(field[2], num_rows - 4, rank);
  }

  pr_field();


  // DEPENDENT MOVEMENT: first move+send from even ranks, then from uneven ranks
  for(int l=0; l<2; l++) {
    if (rank % 2 == l) {
        move_dependent_rows();
        send_rows();
    } else {
        receive_rows();
    }
    barrier();

    // master(pre();pre(););
    // this displays the bottom dependent row movements only for now!
    pr_specific_logs(l);
    pr_field();
  }

  toc("moving + communicating");
  });

  MPI_Type_free(&mpi_idea_type);
  MPI_Finalize();
}

int main() {
  pre(); mpi(); pre();







}



