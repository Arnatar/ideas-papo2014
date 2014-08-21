// #define NDEBUG // no debugging
#include "constants.h"
#include "idea.h"
#include "logic.h"

int start_simulation() {
  srand(time(NULL));
  
  Idea field[SIZE][SIZE];
  fill_matrix_with(field, idea_empty());

  
  // spawn ideas
  for(int i=0; i<IDEAS; i++) {
    int rand_x = rand_int(SIZE, 0);
    int rand_y = rand_int(SIZE, 0);
    field[rand_y][rand_x] = idea_new();
  }
  pr_field();

  move_ideas(field);

  pr_field();

  // fill_matrix_with_zeros(field);
  // fill_matrix_with_ideas(field);

#define ROUNDS 1
  // prfield();
  // for(int i=0; i<ROUNDS; i++) {
    // move_ideas(field);
  // }
}


int main() {
  pre();
  start_simulation();

}
