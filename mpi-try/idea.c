#ifndef __idea_h__
#define __idea_h__

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
    if (tempb <= IDEA_MAX) {
      i.b = tempb;
    } 
    else i.b = IDEA_MAX;
  } 
  else i.b = 0;

  i.c = rand_int(IDEA_MAX, 0);

  // first human worldview based on its init idea worldview
  int temph = i.c + rand_int(4, -2);
  if (0 <= temph) {
    if (temph <= IDEA_MAX) {
      i.b = temph;
    } 
    else i.b = IDEA_MAX;
  } 
  else i.b = 0;

  i.empty = 0;
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
