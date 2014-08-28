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
  i.a = rand_int(IDEA_MAX, 0);
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

#endif
