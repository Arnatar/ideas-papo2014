// #define NDEBUG // no debugging
#include "constants.h"
#include "idea.h"
#include "logic.h"
#include <stddef.h>

#define P99_PROTECT(...) __VA_ARGS__ 

void get_workloads(int workload[], int nranks, int size) {
  int base_amount = size / nranks;
  int rest_amount = size % nranks;

  int rests[nranks];
  for_every(i, nranks, rests[i] = (i < rest_amount ? 1 : 0));

  for_every(i, nranks, workload[i] = base_amount + rests[i]);
}



int start_simulation() {
  srand(time(NULL));
  
  Idea field[SIZE][SIZE];
  fill_matrix_with(field, idea_empty());

  // spawn ideas
  for_every(i, NUM_IDEAS, field[rand_int(SIZE,0)][rand_int(SIZE,0)] = idea_new());

  pr_field();

  move_ideas(field);

  pr_field();

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
  mpi_define_idea_type();

  int workload[num_ranks];
  get_workloads(workload, num_ranks, SIZE);

  MPI_Request req, req2;
  MPI_Request reqs[] = {req,req2};

  master(
    Idea idea = idea_new();
    Idea idea2 = idea_new();
    pr_idea(idea);
    pr_idea(idea2);

    send(idea, Rank(1), req);
    send(idea2, Rank(2), req2);
    wait_for(req);
    wait_for(req2);

    receive_into(idea, Rank(1), req);
    receive_into(idea2, Rank(2), req2);

    wait_for(req); wait_for(req2);
    pr_idea(idea); pr_idea(idea2);
    );

  worker(
    Idea idea;

    receive_into_and_wait(idea, Rank(0));
    pr("\nfrom worker: %d, idea:%d", rank, idea.a);
    idea.a += 1;
    send(idea, Rank(0), req);

    // wait_for(req);
    // pr_idea(idea);
    );

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
  // start_simulation();
  int nranks = 2;

  // pri(base_amount);
  // pri(rest_amount);



  pre();
}






