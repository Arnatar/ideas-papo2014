
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



#define foreach(item, array)                           \
      for(int keep = 1,                                \
              count = 0,                               \
              size = sizeof (array) / sizeof *(array); \
          keep && count != size;                       \
          keep = !keep, count++)                       \
        for(item = (array) + count; keep; keep = !keep)

