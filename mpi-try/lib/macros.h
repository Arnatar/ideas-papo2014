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

#define send_ideas(ideas_arr, to, req) \
  MPI_Isend(ideas_arr, X_SIZE, mpi_idea_type, to, to, MPI_COMM_WORLD, &req) 

#define receive_ideas_into(ideas_arr, from, req) \
  MPI_Irecv(ideas_arr, X_SIZE, mpi_idea_type, from, rank, MPI_COMM_WORLD, &req) 

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


#define send_border_rows()                                                \
  /* send our last row into top ghost row of the next rank */             \
  send_ideas(field[num_rows-2], next_rank, req);                          \
  /* send our first row into the bottom ghost row of the previous rank */ \
  send_ideas(field[1], prev_rank, req2); 

#define receive_border_rows()                                      \
  /* receive last row from previous rank into our top ghost row */ \
  receive_ideas_into(field[0], prev_rank, req);                    \
  /* receive first row from next rank into our bottom ghost row */ \
  receive_ideas_into(field[num_rows-1], next_rank, req);           \

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
  );                          

