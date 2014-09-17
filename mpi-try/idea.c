#ifndef __idea_h__
#define __idea_h__
//#define HTEST

#include "constants.h"

// a idea quali
// b idea complex
// c idea worldview
// h human worldview
typedef struct {
  int a, b, c, h, empty;
} Idea;

Idea idea_new()
{
  Idea i;
  int chance = rand_int(1000, 0);
  if(chance < 50){
    i.a = rand_int(IDEA_MAX, 0);
  }
  else if(chance < 100) {
    i.a = rand_int((int) (IDEA_MAX * 0.75), 0);
  }
  else {
    i.a = rand_int((int) (IDEA_MAX * 0.25), 0);
  }

  // complex should depend on qual
  int tempb = i.a + rand_int(3, -1);
  if (0 <= tempb) {
    if (tempb < IDEA_MAX) {
      i.b = tempb;
    } 
    else i.b = IDEA_MAX - 1;
  } 
  else i.b = 0;

  #ifndef HTEST
  // first human worldview based on its init idea worldview
  i.c = rand_int(IDEA_MAX, 0);  
  int temph = i.c + rand_int(5, -2);
  if (0 <= temph) {
    if (temph < IDEA_MAX) {
      i.h = temph;
    } 
    else i.h = IDEA_MAX - 1;
  } 
  else i.h = 0;

  i.empty = 0;
  #endif
  #ifdef HTEST
  int test = rand_int(3, 0);
  if (test == 0) {
    i.c = 1;
    i.h = 1;
  } else if (test == 1) {
    i.c = 5;
    i.h = 5;
  } else if (test == 2) {
    i.c = 9;
    i.h = 9;
  }
  #endif
  return i;
}

Idea idea_empty() {
  Idea i;
  i.a = 0;
  i.b = 0;
  i.c = 0;
  i.h = 0;
  i.empty = 1;
  return i;
}

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
