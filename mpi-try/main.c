// #define NDEBUG // no debugging
#include "constants.h"
#include "idea.h"
#include "logic.h"
#include <stddef.h>

#define P99_PROTECT(...) __VA_ARGS__ 

void get_fname(char* fname, int rank) {
  sprintf(fname, "out/%d", rank);
}

void get_distribution(int distribution[], int nranks, int global_amount) {
  int base_amount = global_amount / nranks;
  int rest_amount = global_amount % nranks;

  int rests[nranks];
  for_every(i, nranks, rests[i] = (i < rest_amount ? 1 : 0));

  for_every(i, nranks, distribution[i] = base_amount + rests[i]);
}



int start_simulation(int num_rows, int num_ideas) {
    // fill_matrix_with_zeros(field);
  // fill_matrix_with_ideas(field);

#define ROUNDS 1
  // prfield();
  // for(int i=0; i<ROUNDS; i++) {
    // move_ideas(field);
  // }
}


void mpi() {
  mpi_init(); // rank, num_ranks


  // if (Y_SIZE / num_ranks
  // initializations -----------------------------------------------------------
  mpi_define_idea_type();

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

  Idea field[num_rows][X_SIZE];
  fill_matrix_with(field, num_rows, idea_empty());

  // spawn ideas (the random y-value for field excludes the ghost rows
  for_every(i, num_ideas, field[rand_int(num_rows-2,1)][rand_int(X_SIZE,0)] = idea_new());

  // fill all ghost rows
  MPI_Request req, req2;

  send_border_rows();
  receive_border_rows();

  barrier();

  // get filename for rank ("out/$rank")
  char fname[100];
  get_fname(fname, rank);
  FILE *fp;


  // movement ------------------------------------------------------------------
  // serial: move all ideas which do not depend on other ranks. 
  // then: 1) move all outer ideas from even ranks. 
  //       1b) communicate to uneven ranks.
  //       2) move all outer ideas from uneven ranks
  //       2b) communicate to even ranks.
  // send ideas ----------------------------------------------------------------

  pr_field();

  // INDEPENDENT MOVEMENT
  if (num_rows > 6) {
    move_ideas(field[2], num_rows - 4, rank);
  }

  // barrier();
  // printf("rank %d: %d %d %d\n", rank, field[3][0].a, field[3][0].b, field[3][0].c);
  pr_logs();
  pr_field();



  // master(prs("-----------"));
  // pr_field();

  // even_ranks(send_border_rows());
  // uneven_ranks(receive_border_rows());


  // print out -----------------------------------------------------------------



  // pr_field();

  // move_ideas(field);

  // pr_field();




  // MPI_Request req, req2;
  // MPI_Request reqs[] = {req,req2};

  // master(
  //   Idea idea = idea_new();
  //   Idea idea2 = idea_new();
  //   pr_idea(idea);
  //   pr_idea(idea2);

  //   send(idea, Rank(1), req);
  //   send(idea2, Rank(2), req2);
  //   wait_for(req);
  //   wait_for(req2);

  //   receive_into(idea, Rank(1), req);
  //   receive_into(idea2, Rank(2), req2);

  //   wait_for(req); wait_for(req2);
  //   pr_idea(idea); pr_idea(idea2);
  //   );

  // worker(
  //   Idea idea;

  //   receive_into_and_wait(idea, Rank(0));
  //   pr("\nfrom worker: %d, idea:%d", rank, idea.a);
  //   idea.a += 1;
  //   send(idea, Rank(0), req);

  //   // wait_for(req);
  //   // pr_idea(idea);
  //   );

  // mpi();
  // example();
  // start_simulation();
  MPI_Type_free(&mpi_idea_type);
  MPI_Finalize();
}

// char[] repr(int rank) {
//   return "[" + "]"; 



int main() {
  pre();
  mpi();
  // pri(4 % 2);
  // int num_ranks = 4;
  // int rank=;
  // int row_amount_distribution[num_ranks];
  // get_distribution(row_amount_distribution, num_ranks, X_SIZE);

  // int ideas_amount_distribution[num_ranks];
  // get_distribution(ideas_amount_distribution, num_ranks, NUM_IDEAS);

  // // add space for ghost rows on top and bottom
  // int num_rows = row_amount_distribution[rank] + 2;
  // int num_ideas = ideas_amount_distribution[rank];
  // srand(time(NULL));

  // pri(rand_int(4-2,1));
  // start_simulation();

  // pri(base_amount);

  // char* fname="text.txt";
  // with_file(fname, {
  //     write("test");
  // });






  pre();
}






