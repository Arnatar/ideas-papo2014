// #define NDEBUG // no debugging
#include "constants.h"

int main() {
  pre();
  srand(time(NULL));

  int field[SIZE][SIZE];
  fill_field(field);
  pra2(field,SIZE);


  pre();
}

