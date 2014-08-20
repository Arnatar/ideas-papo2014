#include "constants.h"

void fill_field(int field[][SIZE]) {
  for(int y=0; y<SIZE; y++) {
    for(int x=0; x<SIZE; x++) {
      field[y][x] = 0;
    }
  }


}

