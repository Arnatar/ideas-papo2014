#include "constants.h"
#include "idea.h"
#include "helpers.h"


// competition check with
// b idea complex
// c idea worldview
// h human worldview
// slowed with chance
int can_compete(Idea i1, Idea i2) {
  int convinceable = 1;
  int complxdif = abs(i1.b - i2.b);
  int chance = (complxdif + 1) * rand_int(10000, 0);
  if(abs((i1.c - i2.h)) > 4 || abs((i2.c - i1.h)) > 4) {
    convinceable = 0;
  } else if(complxdif > 4) {
    convinceable = 0;
  }
  if(chance > 2000) convinceable = 0;
  return convinceable;
} 

// winner check with
// a idea quali
int wins_over(Idea i1, Idea i2) {
  int i1quali = i1.a + rand_int(15, -8);
  int i2quali = i2.a + rand_int(15, -8);
  int first_wins = i1quali >= i2quali;
  return first_wins;
}

//construction of new idea of loosing idea
Idea construct_looser_idea(Idea winner, Idea loser) {
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

/*
Test Method
void _move_ideas_down(Idea** field, Idea** field_new, int start_row, 
                int num_rows, int num_cols, int rank) {
  
  open_logfile_for_writing();
  for(int y=start_row+1; y<start_row+num_rows; y++) {
    for(int x=0; x<num_cols; x++) {
      if (!field[y][x].empty) {
        Idea idea = field[y][x];
        field_new[y+1][x] = idea_dupl(idea);
          write_idea(idea); write(": row %d, col %d -> ",  y+1,x+1);
          write(" (%d, %d)\n", y+2, x+1);
        field_new[y][x] = idea_empty();
      }
    }
  }
  close_logfile();
} */


// Idea-Movement, 1 rank
void _move_ideas(Idea** field, Idea** field_new, int start_row, 
                int num_rows, int num_cols, int rank) {

  open_logfile_for_writing();

  for(int y=start_row+1; y<start_row+num_rows; y++) {
    for(int x=0; x<num_cols; x++) {

      Idea idea = field[y][x]; 

      // if cell is inhabited, first neighbor & competion, then moving
      if (!idea.empty) {
        // used vars & flags
        int new_x = 0; 
        int new_y = 0; 
        int having_neighbors = 0;

        // ----------mutation---------
        // quali & cmplxty
        if(rand_int(100000, 0) < 50) {
          int direction = rand_int(4, -2);
          if(direction <= 0) direction = -1;
          // quali
          int adjust = idea.a + direction * rand_int(3, 1);
          if (0 <= adjust) {
            if (adjust < IDEA_MAX) {
              idea.a = adjust;
            } 
            else idea.a = IDEA_MAX - 1;
          } 
          else idea.a = 0;
          // complxty
          adjust = idea.b + direction * rand_int(3, 1);
          if (0 <= adjust) {
            if (adjust < IDEA_MAX) {
              idea.b = adjust;
            } 
            else idea.b = IDEA_MAX - 1;
          } 
          else idea.b = 0;
        }
        // worldviews
        if(rand_int(100000, 0) < 10) {
          int direction = rand_int(3, -1);
          // worldview idea
          int adjust = idea.c + direction * rand_int(2, 1);
          int tempc = 0;
          if (0 <= adjust) {
            if (adjust < IDEA_MAX) {
              tempc = adjust;
            } 
            else tempc = IDEA_MAX - 1;
          } 
          else tempc = 0;

          //worldview human
          adjust = idea.h + direction * rand_int(2, 1);
          int temph = 0;
          if (0 <= adjust) {
            if (adjust < IDEA_MAX) {
              temph = adjust;
            } 
            else temph = IDEA_MAX - 1;
          } 
          else temph = 0;

          if(abs(tempc - temph) < 4) {
            idea.c = tempc;
            idea.h = temph;
          }
        } 

        // ----------check for neighbors----------
        // unfair checking, first found, first serve
        #ifndef FAIRCHECKING
        for (int move_x = -1; move_x < 2; move_x++) {
          if (having_neighbors) break;
          new_x = x + move_x;
          new_x = new_x == num_cols ? 0 : new_x == -1 ? num_cols-1 : new_x;
          for (int move_y = -1; move_y < 2; move_y++) {
            if (! (move_x || move_y)) continue;
            new_y = y + move_y;
            if (!(field_new[new_y][new_x].empty)) {
              having_neighbors = 1;
              break;
            }
          }
        }
        #endif

        // fair & complete checking
        #ifdef FAIRCHECKING
        int neighbor_coord_set[16];
        int neighbor_coord_set_count = 0;
        for (int move_x = -1; move_x < 2; move_x++) {
          new_x = x + move_x;
          new_x = new_x == num_cols ? 0 : new_x == -1 ? num_cols-1 : new_x;
          for (int move_y = -1; move_y < 2; move_y++) {
            if (! (move_x || move_y)) continue;
            new_y = y + move_y;
            if (!(field_new[new_y][new_x].empty)) {
              having_neighbors = 1;
              neighbor_coord_set[neighbor_coord_set_count] = new_y; 
              neighbor_coord_set[neighbor_coord_set_count + 1] = new_x;
              neighbor_coord_set_count += 2;
            }
          }
        }
        #endif 

        // ----------competition evaluation----------
        // if cell has neighbor(s) 
        if (having_neighbors) {
          // fair checking coord-search
          #ifdef FAIRCHECKING
            int competition_target = 2 * rand_int(neighbor_coord_set_count / 2, 0);
            new_y = neighbor_coord_set[competition_target];
            new_x = neighbor_coord_set[competition_target + 1];
          #endif

          Idea neighbor_idea = field_new[new_y][new_x];

          // competition output
          write_idea(idea); write("at %dx, %dy", x, y);
          write(" has neighbors \n communicates with ");
          write_idea(neighbor_idea); write(" at %dx, %dy -> ", new_x, new_y);
          
          // competition evaluation
          // TODO if cant compete && neighbors are there => try again
          if(can_compete(idea, neighbor_idea)) { 
            if (wins_over(neighbor_idea, idea)) {
              write_idea(neighbor_idea); write("wins.\n");
              Idea tempIdea = construct_looser_idea(neighbor_idea, idea);
              field[y][x] = tempIdea;
              field_new[y][x] = idea_dupl(tempIdea);
            } 
            else {
              write_idea(idea); write("wins.\n");
              Idea tempIdea = construct_looser_idea(idea, neighbor_idea);
              field_new[new_y][new_x] = tempIdea;
              field[new_y][new_x] = idea_dupl(tempIdea);
            }
          }
          else write("can't compete.\n");
        }
        write("\n");

        // ----------movement----------
        // fair movement
        #ifdef FAIRMOVEMENT
        int target_set[16];
        int target_set_count = 0;
        for (int move_x = -1; move_x < 2; move_x++) {
          new_x = x + move_x;
          new_x = new_x == num_cols ? 0 : new_x == -1 ? num_cols-1 : new_x;
          for (int move_y = -1; move_y < 2; move_y++) {
            new_y = y + move_y;
            if (field_new[new_y][new_x].empty) {
              target_set[target_set_count] = new_y; 
              target_set[target_set_count + 1] = new_x;
              target_set_count += 2;
            }
          }
        } 

        if (!(new_y == y && new_x == x)) {
          int movement_target = 2 * rand_int(target_set_count / 2, 0);
          new_y = target_set[movement_target];
          new_x = target_set[movement_target + 1];
          field_new[new_y][new_x] = idea_dupl(idea);
          field_new[y][x] = idea_empty();
          write_idea(idea);
          write("moved to %dx, %dy\n", new_x + 1, new_y + 1);
        }
        else {
          write_idea(idea);
          write("did not move\n");
        }
        #endif

        // no fair movement (higher chance of standing still while sorounded)
        #ifndef FAIRMOVEMENT
        int try_movement = 1;
        int movement_count = 0;
        // find an empty field and move there
        while (try_movement && movement_count < 8) {
          movement_count++;
          int move_x = rand_int(3, -1);
          int move_y = rand_int(3, -1);
          new_x = x + move_x;
          new_y = y + move_y;
          new_x = new_x == num_cols ? 0 : new_x == -1 ? num_cols-1 : new_x;
          if ((move_x || move_y) && field_new[new_y][new_x].empty) {
            field_new[new_y][new_x] = idea_dupl(idea);
            field_new[y][x] = idea_empty();
            try_movement = 0;
            write_idea(idea);
            write("moved to %dx, %dy\n", new_x + 1, new_y + 1);
          }
        }
        if (try_movement) {
          write_idea(idea);
          write("did not move\n");
        }  
        #endif
      }
    }
  }
  close_logfile();
}

