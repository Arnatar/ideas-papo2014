#define DEBUG
#include <stdlib.h>
#include <stdio.h>
#define COLOR    "\x1b[32m"
#define RESET   "\x1b[0m"

// GENERAL ---------------------------------------------------------------------

#define for_every(i, size, f) for(int i=0; i<size; i++) { f; }

#define with_file(f, action) \
  fp=fopen(f, "w"); \
  action \
  fclose(fp);


#define write_newline(...) fprintf(fp, "\n")

// print array of ints
#define pra(arr)                     \
    printf("[ ");                    \
    foreach(int *v, arr) {           \
      printf("%d ", *v);             \
      }                              \
    printf("]\n");                  \
  
#define length(array) (sizeof(array)/sizeof(*(array)))
#define copy(a,b) memcpy(b,a,sizeof(b))

#define pr(...) printf(__VA_ARGS__); printf("\n")


#define fill_matrix_with(arr, rows, cols, fill) \
  for(int y=0; y<rows; y++) {       \
    for(int x=0; x<cols; x++) {     \
      arr[y][x] = fill;             \
    }                               \
  }                                 \

#define fill_array_with(arr, size, fill) \
for(int x=0; x<size; x++) {              \
    arr[x] = fill;                       \
  }                                      \


#define malloc_idea_matrix(name)                            \
  Idea **name = (Idea **)malloc(num_rows * sizeof(Idea *)); \
  for (int i = 0; i < num_rows; ++i)                        \
      name[i] = (Idea *)malloc(num_cols * sizeof(Idea));    \

#define free_idea_matrix(name) \
  for_every(i, num_rows, {     \
    free(name[i]);             \
  });                          \
  free(name);

// MPI -------------------------------------------------------------------------

#define mpi_define_idea_type()                                               \
  int          blocklengths[5] = {1,1,1,1,1};                                  \
  MPI_Datatype types[5] = {MPI_INT,MPI_INT,MPI_INT, MPI_INT, MPI_INT};               \
  MPI_Datatype mpi_idea_type;                                                \
  MPI_Aint     offsets[5];                                                   \
  offsets[0] = offsetof(Idea, a);                                            \
  offsets[1] = offsetof(Idea, b);                                            \
  offsets[2] = offsetof(Idea, c);                                            \
  offsets[3] = offsetof(Idea, h);                                            \
  offsets[4] = offsetof(Idea, empty);                                        \
    MPI_Type_create_struct(5, blocklengths, offsets, types, &mpi_idea_type); \
    MPI_Type_commit(&mpi_idea_type);                                         \

#define send(idea, to, req) \
  MPI_Isend(&idea, 1, mpi_idea_type, to, to, MPI_COMM_WORLD, &req) 

#define send_ideas(ideas_arr, to, tag, req) \
  MPI_Isend(ideas_arr, num_cols, mpi_idea_type, to, tag, MPI_COMM_WORLD, &req) 

#define receive_ideas_into(ideas_arr, from, tag, req) \
  MPI_Irecv(ideas_arr, num_cols, mpi_idea_type, from, tag, MPI_COMM_WORLD, &req) 

#define receive_into(buf, from, req) \
  MPI_Irecv(&buf, 1, mpi_idea_type, from, rank, MPI_COMM_WORLD, &req) 

#define receive_into_and_wait(buf, from) \
  receive_into(buf, from, req);                       \
  wait_for(req)                                       \

#define wait_for(req) \
    MPI_Wait(&req, MPI_STATUS_IGNORE)

#define barrier() \
  MPI_Barrier(MPI_COMM_WORLD)

#define mpi_init()                       \
	MPI_Init(NULL, NULL);                  \
	int rank;                              \
	int num_ranks;                         \
	MPI_Comm_rank(MPI_COMM_WORLD, & rank); \
	MPI_Comm_size(MPI_COMM_WORLD, & num_ranks); 

#define master(f) if (rank == 0) { f; }
#define worker(f) if (rank != 0) { f; }
#define even_ranks(f) if (rank % 2 == 0) { f; }
#define uneven_ranks(f) if (rank % 2 != 0) { f; }


// OUTPUT
#define pr_idea(idea)                             \
   printf("(Qual: %d, Complx: %d, IWV: %d, HWV: %d) ",idea.a, idea.b, idea.c, idea.h) \


#ifdef DEBUG

#define write(...) fprintf(fp, __VA_ARGS__)

#define write_idea(idea)                             \
  write("(%d,%d,%d,%d) ",idea.a, idea.b, idea.c, idea.h) \

// print array of ideas
#define write_field()                                   \
    for(int i=0; i<num_rows; i++) {                     \
      for(int j=0; j<num_cols; j++) {                     \
        Idea idea = field[i][j];                        \
        if (!idea.empty) {                              \
          write(COLOR); write_idea(idea); write(RESET); \
        } else {                                        \
          write_idea(idea);                             \
        }                                               \
      }                                                 \
      write_newline();                                  \
    }                                                   \

#define pr_field()            \
  barrier();                  \
  get_fname(fname, rank);     \
  with_file(fname, {          \
    write_field();            \
  });                         \
  barrier();                  \
  master(                     \
    for_every(i, num_ranks, { \
      get_fname(fname, i);    \
      prfile(fname);          \
      pre();                  \
    });                       \
  );                          

#define pr_logs() \
  barrier(); \
  master(                     \
    for_every(i, num_ranks, { \
      pr("[%d] ================================\n", i); \
      get_log_fname(fname, i);    \
      prfile(fname);          \
      pre();                  \
    });                       \
      prs("result:\n");\
  );                          

#define pr_specific_logs(j)                  \
  barrier();                                 \
  master(                                    \
    printf(j % 2 == 0? "EVEN" : "UNEVEN");     \
    prs(" ranks moving dependent rows now (just the bottom row's):"); \
    prs("-----------------------------------------"); \
    for_every(i, num_ranks, {                  \
        if(i % 2 == j) {                       \
          pr("[rank %d]", i); \
          get_log_fname(fname, i);             \
            prfile(fname);                     \
            pre();                             \
          }                                    \
      });                                      \
      prs("result:\n");\
  );                          


#define open_logfile_for_writing() \
  char log_fname[100];             \
  get_log_fname(log_fname, rank);  \
  FILE *fp;                        \
  fp=fopen(log_fname, "w");        \

#define close_logfile() \
  fclose(fp);

#else
#define write(...)
#define write_idea(...)
#define write_field()
#define pr_field()
#define pr_logs()
#define pr_specific_logs(...)

#define open_logfile_for_writing()
#define close_logfile();
#endif
// PROGRAM LOGIC ---------------------------------------------------------------
#define Rank(n) n
#define Master 0

// these tags define the semantics of the *sender's* message content
#define GHOST 7777
#define REAL 6666


#define send_real_rows_to_ghost_rows() \
  /* send our last real row into top ghost row of the next rank */             \
  send_ideas(field[num_rows-2], next_rank, REAL, req);                         \
  /* send our first real row into the bottom ghost row of the previous rank */ \
  send_ideas(field[1], prev_rank, REAL, req2);                                 \

#define receive_real_rows_into_ghost_rows()                             \
  /* receive last real row from previous rank into our top ghost row */ \
  receive_ideas_into(field[0], prev_rank, REAL, req);                   \
  /* receive first real row from next rank into our bottom ghost row */ \
  receive_ideas_into(field[num_rows-1], next_rank, REAL, req2);         \

#define send_top_rows() \
  /* send our first ghost row into the bottom real row of the previous rank */ \
  send_ideas(field[0], prev_rank, GHOST, req);                                \
  /* send our first real row into the bottom ghost row of the previous rank */ \
  send_ideas(field[1], prev_rank, REAL, req2);                                 \

#define receive_into_bottom_rows() \
  /* receive first ghost row from next rank into our bottom real row */ \
  receive_ideas_into(field[num_rows-2], next_rank, GHOST, req);        \
  /* receive first real row from next rank into our bottom ghost row */ \
  receive_ideas_into(field[num_rows-1], next_rank, REAL, req2);         \

#define send_bottom_rows() \
  /* send our last real row into top ghost row of the next rank */             \
  send_ideas(field[num_rows-2], next_rank, REAL, req3);                         \
  /* send our last ghost row into the top real row of the next rank */         \
  send_ideas(field[num_rows-1], next_rank, GHOST, req4);                       \

#define receive_into_top_rows() \
  /* receive last real row from previous rank into our top ghost row */ \
  receive_ideas_into(field[0], prev_rank, REAL, req3);                   \
  /* receive last ghost row from prev rank into our top real row */     \
  receive_ideas_into(field[1], prev_rank, GHOST, req4);                 \


// void _move_ideas(Idea** field, Idea** field_new, int start_row, 
//                 int num_rows, int num_cols, int rank) {

// num_rows is not inclusive. e.g.: 0,3 would iterate over 1 and 2 in move_ideas,
// because the for loop starts with start_row+1 and the stop condition is '< num_rows'
// TODO: optimize: not the whole field has to be copied (?)
#define move_ideas(start_row, num_rows) \
  _move_ideas(field, field_new, start_row, num_rows, num_cols, rank); \
  copy_field_new_into_field();


// #define move_dependent_rows()    \
//   move_ideas(0, 3); \
//   move_ideas(num_rows - 4, 3); 

#define move_top_rows() \
  move_ideas(0, 3);

#define move_bottom_rows() \
  move_ideas(num_rows - 4, 3); 

#define send_rows()               \
  send_real_rows_to_ghost_rows(); \
  send_ghost_rows_to_real_rows();

#define receive_rows()                 \
  receive_real_rows_into_ghost_rows(); \
  receive_ghost_rows_into_real_rows();



// measure ---------------------------------------------------------------------
#define init_time_measurement_variables() \
  struct timeval start_time;              \
  struct timeval end_time;                \
  double start_dtime, end_dtime, diff;

#define tic()                                                                 \
  gettimeofday(&start_time,NULL);                                             \
  start_dtime=(double)start_time.tv_sec+(double)start_time.tv_usec/1000000.0; \



#define tocs(description)                                                           \
  gettimeofday(&end_time,NULL);                                         \
  end_dtime=(double)end_time.tv_sec+(double)end_time.tv_usec/1000000.0; \
  diff=end_dtime-start_dtime;                                           \
  master(pr("%s: %f", description, diff));


#define toc()                                                           \
  gettimeofday(&end_time,NULL);                                         \
  end_dtime=(double)end_time.tv_sec+(double)end_time.tv_usec/1000000.0; \
  diff=end_dtime-start_dtime;                                           \
  master(pr("%f", diff));
// temp
#define prf(field) \
  for_every(i, num_rows, { \
      for_every(j, num_cols, { \
          pr_idea(field[i][j]); \
      }); \
      pre(); \
  }); \
      pre(); \


#define copy_field_into_field_new()    \
  for_every(i, num_rows, {             \
    for_every(j, num_cols, {           \
        field_new[i][j] = field[i][j]; \
      });                              \
  }); 

#define copy_field_new_into_field()    \
  for_every(i, num_rows, {             \
    for_every(j, num_cols, {           \
        field[i][j] = field_new[i][j]; \
      });                              \
  }); 
