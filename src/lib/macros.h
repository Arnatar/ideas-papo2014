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

#define fill_matrix_with(arr, rows, cols, fill) \
  for(int y=0; y<rows; y++) {       \
    for(int x=0; x<cols; x++) {     \
      arr[y][x] = fill;             \
    }                               \
  }                                 \

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

#define send_ideas(ideas_arr, to, tag, req) \
  MPI_Isend(ideas_arr, num_cols, mpi_idea_type, to, tag, MPI_COMM_WORLD, &req) 

#define receive_ideas_into(ideas_arr, from, tag, req) \
  MPI_Irecv(ideas_arr, num_cols, mpi_idea_type, from, tag, MPI_COMM_WORLD, &req) 

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

// OUTPUT

#define pr_idea(idea)                             \
   printf("(%d %d %d %d)\n ",idea.a, idea.b, idea.c, idea.h) \

#ifdef DEBUG

#define write(...) fprintf(fp, __VA_ARGS__)

#define write_idea(idea)                             \
  write("(%d,%d,%d,%d) ",idea.a, idea.b, idea.c, idea.h) \

// print array of ideas
#define write_field(field)                                   \
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

#define pr_field(field)            \
  barrier();                  \
  get_fname(fname, rank);     \
  with_file(fname, {          \
    write_field(field);            \
  });                         \
  barrier();                  \
  master(                     \
    for_every(i, num_ranks, { \
      get_fname(fname, i);    \
      prfile(fname);          \
      pre();                  \
    });                       \
  pre();                  \
  );                          

#define pr_logs() \
  barrier(); \
  master(                     \
    for_every(i, num_ranks, { \
      pr("[%d]..............\n", i); \
      get_log_fname(fname, i);    \
      prfile(fname);          \
      pre();                  \
    });                       \
  );                          


#define open_logfile_for_writing() \
  char log_fname[100];             \
  get_log_fname(log_fname, rank);  \
  FILE *fp;                        \
  fp=fopen(log_fname, "w");        \

#define close_logfile() \
  fclose(fp);

#define prs(str) _prs(str)
#define pr(...) printf(__VA_ARGS__); printf("\n")

#else
#define write(...)
#define write_idea(...)
#define write_field(...)
#define pr_field(...)
#define pr_logs()
#define pr_specific_logs(...)
#define prs(...)
#define pr(...)

#define open_logfile_for_writing()
#define close_logfile();
#endif
// PROGRAM LOGIC ---------------------------------------------------------------
#define Rank(n) n
#define Master 0

// these tags define the semantics of the *sender's* message content
#define GHOST 7777
#define REAL 6666


#define send_real_rows_to_ghost_rows(field) \
  /* send our last real row into top ghost row of the next rank */             \
  send_ideas(field[num_rows-2], next_rank, REAL, req);                         \
  /* send our first real row into the bottom ghost row of the previous rank */ \
  send_ideas(field[1], prev_rank, REAL, req2);                                 \

#define receive_real_rows_into_ghost_rows(field)                             \
  /* receive last real row from previous rank into our top ghost row */ \
  receive_ideas_into(field[0], prev_rank, REAL, req);                   \
  /* receive first real row from next rank into our bottom ghost row */ \
  receive_ideas_into(field[num_rows-1], next_rank, REAL, req2);         \

#define send_top_rows(field) \
  /* send our first ghost row into the bottom real row of the previous rank */ \
  send_ideas(field[0], prev_rank, GHOST, req);                                \
  /* send our first real row into the bottom ghost row of the previous rank */ \
  send_ideas(field[1], prev_rank, REAL, req2);                                 \

#define receive_into_bottom_rows(field) \
  /* receive first ghost row from next rank into our bottom real row */ \
  receive_ideas_into(field[num_rows-2], next_rank, GHOST, req);        \
  /* receive first real row from next rank into our bottom ghost row */ \
  receive_ideas_into(field[num_rows-1], next_rank, REAL, req2);         \

#define send_bottom_rows(field) \
  /* send our last real row into top ghost row of the next rank */             \
  send_ideas(field[num_rows-2], next_rank, REAL, req3);                         \
  /* send our last ghost row into the top real row of the next rank */         \
  send_ideas(field[num_rows-1], next_rank, GHOST, req4);                       \

#define receive_into_top_rows(field) \
  /* receive last real row from previous rank into our top ghost row */ \
  receive_ideas_into(field[0], prev_rank, REAL, req3);                   \
  /* receive last ghost row from prev rank into our top real row */     \
  receive_ideas_into(field[1], prev_rank, GHOST, req4);                 \


#define move_ideas(start_row, num_rows) \
  _move_ideas(field, field_new, start_row, num_rows, num_cols, rank); \

// measure ---------------------------------------------------------------------
#define init_time_measurement_variables() \
  struct timeval start_time;              \
  struct timeval end_time;                \
  double start_dtime, end_dtime, diff;

#define tic()                                                                 \
  gettimeofday(&start_time,NULL);                                             \
  start_dtime=(double)start_time.tv_sec+(double)start_time.tv_usec/1000000.0; \

#ifdef MEASURE
#define toc()                                                           \
  gettimeofday(&end_time,NULL);                                         \
  end_dtime=(double)end_time.tv_sec+(double)end_time.tv_usec/1000000.0; \
  diff=end_dtime-start_dtime;                                           \
  master(printf("%f\n", diff));

#else
#define toc()                                                           \
  gettimeofday(&end_time,NULL);                                         \
  end_dtime=(double)end_time.tv_sec+(double)end_time.tv_usec/1000000.0; \
  diff=end_dtime-start_dtime;                                           \
  master(printf("\nIt took %f seconds.\n\n", diff));

#endif

// ------------------- the rest ----------

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

// DRAWING ---------------------------------------------------------------------
#ifdef DRAW

#define write_draw(...) fprintf(fp, __VA_ARGS__)

#define write_idea_draw(idea)                             \
  write_draw("%d %d %d %d, ",idea.a, idea.b, idea.c, idea.h) \

#define save_local_field_for_drawing() \
    for(int j=1; j<num_rows-1;j++) {  \
      for(int k=0; k<num_cols; k++) {  \
        Idea idea = field[j][k];       \
        write_idea_draw(idea);         \
      }                                \
      write_newline();                 \
    }                                  \

#define generate_draw_files()                    \
  barrier();                                     \
  get_draw_fname(fname, rank,i);               \
  with_file(fname, {                             \
    save_local_field_for_drawing();              \
  });                                            \

#else
#define write_idea_draw()
#define save_local_field_for_drawing()
#define generate_draw_files()
#endif
