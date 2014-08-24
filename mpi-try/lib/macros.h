#include <stdlib.h>
#include <stdio.h>
#define COLOR    "\x1b[32m"
#define RESET   "\x1b[0m"


#define foreach(item, array)                           \
      for(int keep = 1,                                \
              count = 0,                               \
              size = sizeof (array) / sizeof *(array); \
          keep && count != size;                       \
          keep = !keep, count++)                       \
        for(item = (array) + count; keep; keep = !keep)

#define pr_idea(idea)                             \
  printf("(%d,%d,%d) ",idea.a, idea.b, idea.c) \

// print array of ints
#define pra(arr)                     \
    printf("[ ");                    \
    foreach(int *v, arr) {           \
      printf("%d ", *v);             \
      }                              \
    printf("]\n");                  \
  

  // print array of ideas
#define pr_field(num_rows)                                     \
    for(int i=0; i<num_rows; i++) {                        \
      for(int j=0; j<SIZE; j++) {                      \
        Idea idea = field[i][j];                      \
        if (!idea.empty) {                            \
          printf(COLOR); pr_idea(idea); printf(RESET); \
        } else {                                       \
          pr_idea(idea);                               \
        }                                              \
      }                                                \
      pre();                                           \
    }                                                  \
    pre();                                             \


#define length(array) (sizeof(array)/sizeof(*(array)))
#define copy(a,b) memcpy(b,a,sizeof(b))


#define pr(...) printf(__VA_ARGS__); printf("\n")


#define fill_matrix_with(arr, rows, fill) \
  for(int y=0; y<rows; y++) {       \
    for(int x=0; x<SIZE; x++) {     \
      arr[y][x] = fill;             \
    }                               \
  }                                 \

#define fill_array_with(arr, size, fill) \
for(int x=0; x<size; x++) {            \
    arr[x] = fill;                     \
  }                                    \

// #define new_idea() \
  // {rand_int(IDEA_MAX, 0), rand_int(IDEA_MAX, 0), rand_int(IDEA_MAX, 0)}


#define mpi_init()                       \
	MPI_Init(NULL, NULL);                  \
	int rank;                              \
	int num_ranks;                         \
	MPI_Comm_rank(MPI_COMM_WORLD, & rank); \
	MPI_Comm_size(MPI_COMM_WORLD, & num_ranks); 

#define master(f) if (rank == 0) { f; }
#define worker(f) if (rank != 0) { f; }

// MPI -------------------------------------------------------------------------
// vim: ft=c: 

// MPI_Send(void* data, int count, MPI_Datatype datatype, int destination,
//          int tag, MPI_Comm communicator)

// MPI_Recv(void* data, int count, MPI_Datatype datatype, int source,
//          int tag, MPI_Comm communicator, MPI_Status* status)

// int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
//               int tag, MPI_Comm comm, MPI_Request *request)

// int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
//               MPI_Comm comm, MPI_Request *request)

	MPI_Request recvPrev;
// MPI_Irecv(& old[0], 1, MPI_FLOAT, prevRank, 4711, MPI_COMM_WORLD, & recvPrev);
// MPI_Isend(& new[elementsPerProcess], 1, MPI_FLOAT, nextRank, 4711, MPI_COMM_WORLD, & sendNext);

// get_into(buf, 1, 
			// MPI_Wait(& recvPrev, MPI_STATUS_IGNORE);


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

// struct send_args {
//   Idea
//   char name[25];
// };

#define send(idea, to, req) \
  MPI_Isend(&idea, 1, mpi_idea_type, to, to, MPI_COMM_WORLD, &req) 

// MPI_Irecv(& old[0], 1, MPI_FLOAT, prevRank, 4711, MPI_COMM_WORLD, & recvPrev);
#define receive_into(buf, from, req) \
  MPI_Irecv(&buf, 1, mpi_idea_type, from, rank, MPI_COMM_WORLD, &req) 

#define receive_into_and_wait(buf, from) \
  receive_into(buf, from, req);                       \
  wait_for(req)                                       \


#define wait_for(req) \
    MPI_Wait(&req, MPI_STATUS_IGNORE)

// #define wait_for_all(reqs) \
//     MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE)


// #define foo(struc) pri(struc.to)
// #define my(a) pri(a)

// #define _send_idea(a,b,c,d)                                               \
    // pri(a);

  // struct args a = _struct;                                                \
  // MPI_Isend(a.idea, 1, mpi_idea_type, a.to, a.tag, MPI_COMM_WORLD, a.req) \


// #define send_idea(...) _send_idea((struct args){ __VA_ARGS__ })

        // MPI_Send(&send,   1, mpi_idea_type, dest, tag, MPI_COMM_WORLD);

// #define foo(...) struct_try((struct args){ __VA_ARGS__ })


#define Rank(n) n
#define Master 0

#define for_every(i, size, f) for(int i=0; i<size; i++) { f; }
