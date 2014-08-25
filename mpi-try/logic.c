#include "constants.h"
#include "idea.h"
#include "helpers.h"


int wins_over(Idea i1, Idea i2) {
  int first_wins = (i1.a > i2.a);
  return first_wins;
}

void move_ideas(Idea field[][X_SIZE], int rows, int rank) {
  Idea old[rows][X_SIZE];
  copy(field,old);

  // open our rank log file for logging
  char log_fname[100];
  get_log_fname(log_fname, rank);
  FILE *fp;
  fp=fopen(log_fname, "w"); 

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

          // if neighbor-cell is empty, move there
          if (neighbor_idea.empty) {
            write("neighbor empty\n");
            field[new_y][new_x] = idea;
            field[y][x] = idea_empty();
          } else {
            write("neighbor not empty, communicate with "); write_idea(neighbor_idea);
            write("... -> ");
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
  fclose(fp);

}

