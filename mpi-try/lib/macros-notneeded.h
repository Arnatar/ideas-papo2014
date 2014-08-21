
#define pra2(arr,y_length)        \
  for(int i=0; i<y_length; i++) { \
    pra(arr[i]);                  \
  }                               \

// print array of ints
#define pra(arr)                     \
    printf("[ ");                    \
    foreach(int *v, arr) {           \
      if(*v > 0) {                   \
      printf(COLOR "%d " RESET, *v); \
      } else {                       \
      printf("%d ", *v);             \
      }                              \
    }                                \
    printf("]\ n");                  \

#define sum(arr)             \
    ({                       \
      int s = 0;             \
      foreach(int* v, arr) { \
        s += *v;             \
      }                      \
      s;                     \
      })

