#include "constants.h"
#include "idea.h"


int wins_over(Idea i1, Idea i2) {
  int first_wins = (i1.a > i2.a);
  if (first_wins) { pr_idea((first_wins? i1 : i2)); prs("wins. result:\n"); }
  return first_wins;
}

void move_ideas(Idea field[][X_SIZE]) {
  Idea old[X_SIZE][X_SIZE];
  copy(field,old);

  for(int y=0; y<X_SIZE; y++) {
    for(int x=0; x<X_SIZE; x++) {

      Idea idea = old[y][x]; 

      // if cell is inhabited, possibly move
      if (!idea.empty) {
        int move_x = rand_int(3,-1);
        int move_y = rand_int(3,-1);

        // if cell actually moves 
        if (move_x || move_y) {
          prs("---");

          int new_x = x+move_x;
          int new_y = y+move_y;
          new_x = new_x == X_SIZE ? 0 : new_x == -1 ? X_SIZE-1 : new_x;
          new_y = new_y == X_SIZE ? 0 : new_y == -1 ? X_SIZE-1 : new_y;
          Idea neighbor_idea = field[new_y][new_x];

          pr_idea(idea); pr(": (%d, %d) -> (%d,%d)",  x+1,y+1, new_x+1,new_y+1);

          // if neighbor-cell is empty, move there
          if (neighbor_idea.empty) {
            prs("neighbor empty");
            field[new_y][new_x] = idea;
            field[y][x] = idea_empty();
          } else {
            printf("neighbor not empty, communicate with "); pr_idea(neighbor_idea);
            prs("... -> ");
            if (wins_over(neighbor_idea, idea)) {
              old[y][x] = neighbor_idea;
              field[y][x] = neighbor_idea;

            } else if (wins_over(idea, neighbor_idea)) {
              field[new_y][new_x] = idea;
              old[new_y][new_x] = idea;
            }
          }
          // pr_field();

        }
      }
    }
  }
}

