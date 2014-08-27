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
int can_convince(Idea i1, Idea i2) {
  int convinceable = 1;
  if(abs((i1.c - i2.h)) > 4 || abs((i2.c - i1.h)) > 4) {
    convinceable = 0;
  } else if(abs(i1.b - i2.b) > 5) {
    convinceable = 0;
  }
  return convinceable;
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

          // if neighbor-cell is empty, move there. if not test competition constraints & compete
          if (neighbor_idea.empty) {
            write("neighbor empty\n");
            field_new[new_y][new_x] = idea;
            field_new[y][x] = idea_empty();
          } else {
            write("neighbor not empty, communicate with "); 
            write_idea(neighbor_idea);
            write("... -> ");
            
            // TODO: correct winner-copying (4th var)
            if(can_convince(idea, neighbor_idea)) { 
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
            else write("can't compete.\n");
          }
          write("\n");

        }
      }
    }
  }
  close_logfile();

}

