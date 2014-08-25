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

#define write(...) fprintf(fp, __VA_ARGS__)

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


#define fill_matrix_with(arr, rows, fill) \
  for(int y=0; y<rows; y++) {       \
    for(int x=0; x<X_SIZE; x++) {     \
      arr[y][x] = fill;             \
    }                               \
  }                                 \

#define fill_array_with(arr, size, fill) \
for(int x=0; x<size; x++) {            \
    arr[x] = fill;                     \
  }                                    \


// MPI -------------------------------------------------------------------------

#define mpi_define_idea_type()                                               \
  int          blocklengths[4] = {1,1,1,1};                                  \
  MPI_Datatype types[4] = {MPI_INT,MPI_INT, MPI_INT, MPI_INT};               \
  MPI_Datatype mpi_idea_type;                                                \
  MPI_Aint     offsets[4];                                                   \
  offsets[0] = offsetof(Idea, a);                                            \
  offsets[1] = offsetof(Idea, b);                                            \
  offsets[2] = offsetof(Idea, c);                                            \
  offsets[3] = offsetof(Idea, empty);                                        \
    MPI_Type_create_struct(4, blocklengths, offsets, types, &mpi_idea_type); \
    MPI_Type_commit(&mpi_idea_type);                                         \

#define send(idea, to, req) \
  MPI_Isend(&idea, 1, mpi_idea_type, to, to, MPI_COMM_WORLD, &req) 

#define send_ideas(ideas_arr, to, tag, req) \
  MPI_Isend(ideas_arr, X_SIZE, mpi_idea_type, to, tag, MPI_COMM_WORLD, &req) 

#define receive_ideas_into(ideas_arr, from, tag, req) \
  MPI_Irecv(ideas_arr, X_SIZE, mpi_idea_type, from, tag, MPI_COMM_WORLD, &req) 

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



// PROGRAM LOGIC ---------------------------------------------------------------
#define Rank(n) n
#define Master 0


#define pr_idea(idea)                             \
  printf("(%d,%d,%d) ",idea.a, idea.b, idea.c) \



/* // print array of ideas */
/* #define pr_field(num_rows)                                     \ */
/*     for(int i=0; i<num_rows; i++) {                        \ */
/*       for(int j=0; j<X_SIZE; j++) {                      \ */
/*         Idea idea = field[i][j];                      \ */
/*         if (!idea.empty) {                            \ */
/*           printf(COLOR); pr_idea(idea); printf(RESET); \ */
/*         } else {                                       \ */
/*           pr_idea(idea);                               \ */
/*         }                                              \ */
/*       }                                                \ */
/*       pre();                                           \ */
/*     }                                                  \ */
/*     pre();                                             \ */

#define write_idea(idea)                             \
  write("(%d,%d,%d) ",idea.a, idea.b, idea.c) \

// print array of ideas
#define write_field()                                   \
    for(int i=0; i<num_rows; i++) {                     \
      for(int j=0; j<X_SIZE; j++) {                     \
        Idea idea = field[i][j];                        \
        if (!idea.empty) {                              \
          write(COLOR); write_idea(idea); write(RESET); \
        } else {                                        \
          write_idea(idea);                             \
        }                                               \
      }                                                 \
      write_newline();                                  \
    }                                                   \
    /* write_newline(); */                                           

// these tags define the semantics of the *sender's* message content
#define GHOST 7777
#define REAL 6666

#define send_real_rows_to_ghost_rows() \
  /* send our last real row into top ghost row of the next rank */             \
  send_ideas(field[num_rows-2], next_rank, REAL, req);                         \
  /* send our first real row into the bottom ghost row of the previous rank */ \
  send_ideas(field[1], prev_rank, REAL, req2);                                 \

#define send_ghost_rows_to_real_rows()                                         \
  /* send our first ghost row into the bottom real row of the previous rank */ \
  send_ideas(field[0], next_rank, GHOST, req3);                                \
  /* send our last ghost row into the top real row of the next rank */         \
  send_ideas(field[num_rows-1], next_rank, GHOST, req4);                       \

#define receive_real_rows_into_ghost_rows()                             \
  /* receive last real row from previous rank into our top ghost row */ \
  receive_ideas_into(field[0], prev_rank, REAL, req);                   \
  /* receive first real row from next rank into our bottom ghost row */ \
  receive_ideas_into(field[num_rows-1], next_rank, REAL, req2);         \

#define receive_ghost_rows_into_real_rows()                             \
  /* receive first ghost row from next rank into our bottom real row */ \
  receive_ideas_into(field[num_rows-2], next_rank, GHOST, req3);        \
  /* receive last ghost row from prev rank into our top real row */     \
  receive_ideas_into(field[1], prev_rank, GHOST, req4);                 \

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

#define delete_logs() \
  system("exec rm -r /code/mpi-try/log/*")

#define pr_logs() \
  barrier(); \
  master(                     \
    for_every(i, num_ranks, { \
      pr("[%d] ================================\n", i); \
      get_log_fname(fname, i);    \
      prfile(fname);          \
      pre();                  \
    });                       \
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
  );                          


#define move_dependent_rows()             \
  move_ideas(field[0], 3, rank);          \
  move_ideas(field[num_rows-3], 3, rank); 

#define send_rows()               \
  send_real_rows_to_ghost_rows(); \
  send_ghost_rows_to_real_rows();

#define receive_rows()                 \
  receive_real_rows_into_ghost_rows(); \
  receive_ghost_rows_into_real_rows();
