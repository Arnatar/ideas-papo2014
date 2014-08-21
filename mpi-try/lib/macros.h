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

  // print array of ideas
#define pr_field()                                     \
    for(int i=0; i<SIZE; i++) {                        \
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


#define fill_matrix_with(arr, fill) \
  for(int y=0; y<SIZE; y++) {       \
    for(int x=0; x<SIZE; x++) {     \
      arr[y][x] = fill;             \
    }                               \
  }                                 \

#define fill_array_with(arr, size, fill) \
  for(int x=0; x<size; x++) {       \
      arr[x] = fill;             \
    }                               \

// #define new_idea() \
  // {rand_int(IDEA_MAX, 0), rand_int(IDEA_MAX, 0), rand_int(IDEA_MAX, 0)}

