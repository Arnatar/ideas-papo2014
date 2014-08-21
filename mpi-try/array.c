// #define NDEBUG // no debugging
#include "constants.h"
#include "headers/logic.h"
#include "headers/idea.h"

int start_simulation() {
  int field[SIZE][SIZE];
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

  srand(time(NULL));
  // start_simulation();
  
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










  // pra2(field,SIZE);



  // Idea* ideas[] = {&i,&j};
  // pra(ideas);




  // pre();
}

