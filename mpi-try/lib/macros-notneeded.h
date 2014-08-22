
#define pra2(arr,y_length)        \
  for(int i=0; i<y_length; i++) { \
    pra(arr[i]);                  \
  }                               \


#define sum(arr)             \
    ({                       \
      int s = 0;             \
      foreach(int* v, arr) { \
        s += *v;             \
      }                      \
      s;                     \
      })

