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

  // initializations -----------------------------------------------------------

  mpi_define_idea_type();

  srand(time(NULL)+rank);

  init_time_measurement_variables();

  char* _x = getenv("x");
  char* _y = getenv("y");
  char* _rounds = getenv("rounds");
  char* _ideas = getenv("ideas");

  int global_num_rows= _y ? atoi(_y) : 44;
  // it segfaults for big col values
  int global_num_cols= _x ? atoi(_x) : 4;
  int rounds= _rounds ? atoi(_rounds) : 4;
  int global_num_ideas= _ideas ? atoi(_ideas) : global_num_cols*global_num_rows/3;

  int row_amount_distribution[num_ranks];
  get_distribution(row_amount_distribution, num_ranks, global_num_rows);

  int ideas_amount_distribution[num_ranks];
  get_distribution(ideas_amount_distribution, num_ranks, global_num_ideas);

  // add space for ghost rows on top and bottom
  int num_rows = row_amount_distribution[rank] + 2;
  int num_ideas = ideas_amount_distribution[rank];
  int num_cols = global_num_cols;

  if (num_rows == 3) {
    master(fputs("num_rows=3 for a rank is not supported in the current implementation\n\n", stderr));
    exit(-1);
  }

  // init fields ---------------------------------------------------------------
  malloc_idea_matrix(field)
  malloc_idea_matrix(field_new)

  fill_matrix_with(field, num_rows, num_cols, idea_empty());

  // // spawn ideas (the random y-value for field excludes the ghost rows
  for_every(i, num_ideas, 
      field[rand_int(num_rows-2,1)][rand_int(num_cols,0)] = idea_new());

  // rank wraparound -----------------------------------------------------------
	const int next_rank = rank == num_ranks - 1 ? 0 : rank + 1;
	const int prev_rank = rank == 0 ? num_ranks -1 : rank - 1;


  // // fill all ghost rows
  MPI_Request req, req2, req3, req4;
  send_real_rows_to_ghost_rows(field);
  receive_real_rows_into_ghost_rows(field);

  barrier();

  copy_field_into_field_new();

  // get filename for rank ("out/$rank")
  char fname[100];
  get_fname(fname, rank);
  FILE *fp;
  char fname_draw[100];
  FILE *fp_draw;

  tic();

  for_every(i, rounds, {
    master(pr("ROUND %d =================================", i));
    generate_draw_files();
    pr_field(field_new);
  // // movement ------------------------------------------------------------------
  // // serial: move all ideas which do not depend on other ranks. 
  // // then: 1) move all outer ideas from even ranks. 
  // //       1b) communicate to uneven ranks.
  // //       2) move all outer ideas from uneven ranks
  // //       2b) communicate to even ranks.
  // // send ideas ----------------------------------------------------------------


  // // INDEPENDENT MOVEMENT
  // // reason for 7: 1 row in center, can move up or down without touching a dependent
  // // row. from the outer sides: 1 ghost row, 2 dependent rows each = 3*2. the next
  // // row above 6 is the first independent row. 
  // // [2]: give the independent movement procedure all rows starting from the 
  // //      third, until the third last. if num_rows = 8, num_rows-4 = 4. that means:
  // // take 3 rows from the array (the for loop is not inclusive).
  // // so for a matrix with 8 rows we take (0-based) rows 2,3,4,5. 0+1 and 6+7 are 
  // // left out.



  if (num_rows >= 7) {
    // move_ideas_down(2, num_rows-5);
    move_ideas(2, num_rows-5);
  }
  barrier();


  // master(prs("INDEPENDENT ROWS"));
  // pr_logs();
  // pr_field(field_new);

  // move_ideas_down(0, 3); 
  move_ideas(0, 3); 
  barrier();

  send_top_rows(field_new);
  receive_into_bottom_rows(field_new);
  barrier();
  send_top_rows(field);
  receive_into_bottom_rows(field);
  barrier();

  // master(prs("DEPENDENT ROWS TOP"));
  // pr_logs();
  // pr_field(field_new);

  // move_ideas_down(num_rows - 4, 3);  
  move_ideas(num_rows - 4, 3);  
  barrier();

  send_bottom_rows(field_new);
  receive_into_top_rows(field_new);
  barrier();


  // master(prs("DEPENDENT ROWS BOTTOM"));
  // pr_logs();
  // pr_field(field_new);

  copy_field_new_into_field();
  barrier();

  // master(prs("RESULT"));
  // pr_field(field);


  }); //endloop

  toc();

  free_idea_matrix(field);
  free_idea_matrix(field_new);
  MPI_Type_free(&mpi_idea_type);
  MPI_Finalize();
 }


int main() {
  mpi();
}



