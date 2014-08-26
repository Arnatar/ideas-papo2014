#include "constants.h"
#include "idea.h"
#include "helpers.h"


int wins_over(Idea i1, Idea i2) {
  int first_wins = (i1.a > i2.a);
  return first_wins;
}

void _move_ideas(Idea** field, Idea** field_new, int start_row, 
                int num_rows, int num_cols, int rank) {

  open_logfile_for_writing();

  for(int y=start_row+1; y<start_row+num_rows; y++) {
      // printf("rank %d, y=%d\n", rank, y);
    for(int x=0; x<num_cols; x++) {

      Idea idea = field[y][x]; 

      // if cell is inhabited, possibly move
      if (!idea.empty) {
        int move_x = rand_int(3,-1);
        int move_y = rand_int(3,-1);

        // if cell actually moves 
        if (move_x || move_y) {

          int new_x = x+move_x;
          int new_y = y+move_y;
          new_x = new_x == num_cols ? 0 : new_x == -1 ? num_cols-1 : new_x;
          // new_y = new_y == X_SIZE ? 0 : new_y == -1 ? X_SIZE-1 : new_y;
          Idea neighbor_idea = field_new[new_y][new_x];

          write_idea(idea); write(": row %d, col %d -> ",  y+1,x+1);
          write(move_x == 1? "right" : move_x == -1? " left" : "");
          write(move_y == 1? " down" : move_y == -1? " up" : "");
          write(" (%d, %d)\n", new_y+1, new_x+1);

          // if neighbor-cell is empty, move there
          if (neighbor_idea.empty) {
            write("neighbor empty\n");
            field_new[new_y][new_x] = idea;
            field_new[y][x] = idea_empty();
          } else {
            write("neighbor not empty, communicate with "); write_idea(neighbor_idea);
            write("... -> ");
            if (wins_over(neighbor_idea, idea)) {
              write_idea(neighbor_idea); write("wins.\n");
              field[y][x] = neighbor_idea;
              field_new[y][x] = neighbor_idea;

            } else if (wins_over(idea, neighbor_idea)) {
              write_idea(idea); write("wins.\n");
              field_new[new_y][new_x] = idea;
              field[new_y][new_x] = idea;
            }
          }
          write("\n");

        }
      }
    }
  }
  close_logfile();

}

