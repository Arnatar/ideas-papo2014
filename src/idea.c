#ifndef __idea_h__
#define __idea_h__

#include "constants.h"

// idea contains:
// a idea quali
// b idea complex
// c idea worldview
// h human worldview
typedef struct {
  int a, b, c, h, empty;
} Idea;

// -----idea creation-----
// possible values depend on IDEA_MAX constant
Idea idea_new()
{
  Idea i;
  // -----quality evaluation-----
  // 3 classes of possible ideas, chances adjustable
  int chance = rand_int(1000, 0);
  if(chance < BEST_IDEA_CHANCE){
    i.a = rand_int(IDEA_MAX, 0);
  }
  else if(chance < MED_IDEA_CHANCE) {
    i.a = rand_int((int) (IDEA_MAX * 0.66), 0);
  }
  else {
    i.a = rand_int((int) (IDEA_MAX * 0.33), 0);
  }

  // -----complexity evaluation-----
  // complexity depends on qual
  int tempb = i.a + rand_int(2 * QUAL_CMPLXTY_DEP_RANGE + 1, - QUAL_CMPLXTY_DEP_RANGE);
  if (0 <= tempb) {
    if (tempb < IDEA_MAX) {
      i.b = tempb;
    } 
    else i.b = IDEA_MAX - 1;
  } 
  else i.b = 0;

  // -----idea world view evaluation-----
  // worldview arbitrary chosen
  i.c = rand_int(IDEA_MAX, 0);

  // -----human world view evaluation-----
  // init human worldview based on its init idea worldview
  int temph = i.c + rand_int(2 * WV_DIFF_RANGE + 1, - WV_DIFF_RANGE);
  if (0 <= temph) {
    if (temph < IDEA_MAX) {
      i.h = temph;
    } 
    else i.h = IDEA_MAX - 1;
  } 
  else i.h = 0;

  i.empty = 0;
  return i;
}

// -----build empty idea with empty-flag set-----
Idea idea_empty() {
  Idea i;
  i.a = 0;
  i.b = 0;
  i.c = 0;
  i.h = 0;
  i.empty = 1;
  return i;
}

// -----duplicate idea-----
Idea idea_dupl(Idea i2) {
  Idea i;
  i.a = i2.a;
  i.b = i2.b;
  i.c = i2.c;
  i.h = i2.h;
  i.empty = i2.empty;
  return i;
}

#endif
