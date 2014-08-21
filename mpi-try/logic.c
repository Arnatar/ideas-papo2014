#include "constants.h"
#include "headers/idea.h"


void move_ideas(Idea field[][SIZE]) {
  Idea old[SIZE][SIZE];
  copy(field,old);

  for(int y=0; y<SIZE; y++) {
    for(int x=0; x<SIZE; x++) {

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
          new_x = new_x == SIZE ? 0 : new_x == -1 ? SIZE-1 : new_x;
          new_y = new_y == SIZE ? 0 : new_y == -1 ? SIZE-1 : new_y;
          Idea neighbor_idea = field[new_y][new_x];

          pr_idea(idea);
          pr(": (%d, %d) -> (%d,%d)",  x+1,y+1, new_x+1,new_y+1);

          // if neighbor-cell is empty, move there
          if (neighbor_idea.empty) {
            prs("neighbor empty");
            field[new_y][new_x] = idea;
            field[y][x] = idea_empty();
          } else {
            printf("neighbor not empty, communicate with "); pr_idea(neighbor_idea);
            printf("... -> ");
            if (neighbor_idea.a > idea.a) {
              old[y][x] = neighbor_idea;
              field[y][x] = neighbor_idea;

              pr_idea(neighbor_idea); prs("wins. result:");
            } else if (idea.a > neighbor_idea.a) {
              field[new_y][new_x] = idea;
              old[new_y][new_x] = idea;
              pr_idea(idea); prs(" wins.");
            }

          }
          pr_field();

        }
      }
    }
  }
}

