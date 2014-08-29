#include "constants.h"
#include "idea.h"
#include "helpers.h"
#define FAIRCHECKING

// a idea quali
int wins_over(Idea i1, Idea i2) {
  int i1quali = i1.a + rand_int(5, -2);
  int i2quali = i2.a + rand_int(5, -2);
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

Idea build_winner(Idea winner, Idea loser) {
  Idea tempIdea;
  tempIdea.a = winner.a;
  tempIdea.b = winner.b;
  tempIdea.c = winner.c;
  int adjust = tempIdea.c - loser.h;
  if (adjust > 0) {
    adjust = loser.h + adjust / 2 + 1 + rand_int(3, -1);
  } else if (adjust < 0) {
    adjust = loser.h + adjust / 2 - 1 + rand_int(3, -1);
  } else adjust = rand_int(3, -1);
  if (adjust >= 0) {
    if (adjust < IDEA_MAX) {
      tempIdea.h = adjust;
    } else tempIdea.h = IDEA_MAX - 1;
  } else tempIdea.h = 0;
  return tempIdea;
}

void _move_ideas(Idea** field, Idea** field_new, int start_row, 
                int num_rows, int num_cols, int rank) {

  open_logfile_for_writing();

  for(int y=start_row+1; y<start_row+num_rows; y++) {
    for(int x=0; x<num_cols; x++) {

      Idea idea = field[y][x]; 

      // if cell is inhabited, first neighbor & competion, then moving
      if (!idea.empty) {
        int move_x = -1; 
        int move_y = -1;
        int having_neighbors = 0;
        int new_x = 0; 
        int new_y = 0; 

        //check for neighbors

        #ifndef FAIRCHECKING
        //int count = 0;
        //write("idea:"); write_idea(idea); write("\n");
        for (move_x; move_x < 2; move_x++) {
          if (having_neighbors) break;
          new_x = x + move_x;
          new_x = new_x == num_cols ? 0 : new_x == -1 ? num_cols-1 : new_x;
          move_y = -1;
          for (move_y; move_y < 2; move_y++) {
        //    count++;
        //    write("move_x: %d, move_y: %d \n", move_x, move_y);
            if (! (move_x || move_y)) break;
            new_y = y + move_y;
            if (!(field_new[new_y][new_x].empty)) {
              having_neighbors = 1;
        //      write("neighbor spotted\n");
              break;
            }
          }
        }
        //write("Runcount: %d\n",count);
        #endif

        //fair checking
        #ifdef FAIRCHECKING
        Idea neighborset[8];
        int neighbor_count = 0;
        for (move_x; move_x < 2; move_x++) {
          new_x = x + move_x;
          new_x = new_x == num_cols ? 0 : new_x == -1 ? num_cols-1 : new_x;
          move_y = -1;
          for (move_y; move_y < 2; move_y++) {
        //    write("move_x: %d, move_y: %d \n", move_x, move_y);
            if (! (move_x || move_y)) break;
            new_y = y + move_y;
            if (!(field_new[new_y][new_x].empty)) {
              having_neighbors = 1;
              neighborset[neighbor_count] = field_new[new_y][new_x]; 
              neighbor_count++;
              break;
            }
          }
        }
        /*
        for (int i = 0; i < neighbor_count; i++) {
          write_idea(idea); write("\'s neighborset: ");write_idea(neighborset[i]);write("neighbor_count: %d \n", neighbor_count);
        }*/
        #endif 

        // if cell has neighbor(s) 
        if (having_neighbors) {          
          // new_y = new_y == X_SIZE ? 0 : new_y == -1 ? X_SIZE-1 : new_y;
          #ifndef FAIRCHECKING
          Idea neighbor_idea = field_new[new_y][new_x];
          #else
          Idea neighbor_idea = neighborset[rand_int(neighbor_count,0)];
          #endif

          write_idea(idea); write("at %dx, %dy", x, y);
          write(" has neighbors \n communicates with ");
          write_idea(neighbor_idea); write(" -> ");
          
          if(can_convince(idea, neighbor_idea)) { 
            if (wins_over(neighbor_idea, idea)) {
              write_idea(neighbor_idea); write("wins.\n");
              Idea tempIdea = build_winner(neighbor_idea, idea);
              field[y][x] = tempIdea;
              field_new[y][x] = tempIdea;

            } else if (wins_over(idea, neighbor_idea)) {
              write_idea(idea); write("wins.\n");
              Idea tempIdea = build_winner(idea, neighbor_idea);
              field_new[new_y][new_x] = tempIdea;
              field[new_y][new_x] = tempIdea;
            }
          }
            else write("can't compete.\n");
        }
        write("\n");

        // movement
        int try_movement = 1;
        int movement_count = 0;
        // find an empty field and move there
        while (try_movement && movement_count < 8) {
          movement_count++;
          move_x = rand_int(3, -1);
          move_y = rand_int(3, -1);
          new_x = x + move_x;
          new_y = y + move_y;
          new_x = new_x == num_cols ? 0 : new_x == -1 ? num_cols-1 : new_x;
          if ((move_x || move_y) && field_new[new_y][new_x].empty) {
            //field[new_y][new_x] = idea;
            //field[y][x] = idea_empty();
            field_new[new_y][new_x] = idea;
            field_new[y][x] = idea_empty();
            try_movement = 0;
            write_idea(idea);
            write("moved to %dx, %dy", new_x + 1, new_y + 1);
          }
        }
        if (try_movement) {
          write_idea(idea);
          write("did not move");
        }
      }
    }
  }
  close_logfile();

}

