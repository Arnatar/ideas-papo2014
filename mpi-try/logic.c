#include "constants.h"
#include "idea.h"
#include "helpers.h"

// a idea quali
int wins_over(Idea i1, Idea i2) {
  int i1quali = i1.a + rand_int(4, -2);
  int i2quali = i2.a + rand_int(4, -2);
  int first_wins = i1quali >= i2quali;
  return first_wins;
}

// b idea complex
// c idea worldview
// h human worldview
// TODO: implement
int can_convince(Idea i1, Idea i2) {
  int convinceable = 0;
  return convinceable;
} 

void move_ideas(Idea field[][X_SIZE], int rows, int rank) {
  Idea old[rows][X_SIZE];
  copy(field,old);

  open_logfile_for_writing();

  for(int y=1; y<rows-1; y++) {
    for(int x=0; x<X_SIZE; x++) {

      Idea idea = old[y][x]; 

      // if cell is inhabited, possibly move
      if (!idea.empty) {
        int move_x = rand_int(3,-1);
        int move_y = rand_int(3,-1);

        // if cell actually moves 
        if (move_x || move_y) {

          int new_x = x+move_x;
          int new_y = y+move_y;
          new_x = new_x == X_SIZE ? 0 : new_x == -1 ? X_SIZE-1 : new_x;
          // new_y = new_y == X_SIZE ? 0 : new_y == -1 ? X_SIZE-1 : new_y;
          Idea neighbor_idea = field[new_y][new_x];

          write_idea(idea); write(": row %d, col %d -> ",  y+1,x+1);
          write(move_x == 1? "right" : move_x == -1? " left" : "");
          write(move_y == 1? " down" : move_y == -1? " up" : "");
          write(" (%d, %d)\n", new_y+1, new_x+1);

          // if neighbor-cell is empty, move there. if not test competition constraints & compete
          if (neighbor_idea.empty) {
            write("neighbor empty\n");
            field[new_y][new_x] = idea;
            field[y][x] = idea_empty();
          } else {
            write("neighbor not empty, communicate with "); write_idea(neighbor_idea);
            write("... -> ");
            
            // TODO: insert competition constraint test & winner-copying
            if (wins_over(neighbor_idea, idea)) {
              write_idea(neighbor_idea); write("wins.\n");
              old[y][x] = neighbor_idea;
              field[y][x] = neighbor_idea;

            } else if (wins_over(idea, neighbor_idea)) {
              write_idea(idea); write("wins.\n");
              field[new_y][new_x] = idea;
              old[new_y][new_x] = idea;
            }
          }
          write("\n");

        }
      }
    }
  }
  close_logfile();

}

