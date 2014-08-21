#ifndef __idea_h__
#define __idea_h__

#include "constants.h"

typedef struct {
  int a, b, c,empty;
} Idea;

Idea idea_new()
{
  Idea i;
  i.a = rand_int(IDEA_MAX,0);
  i.b = rand_int(IDEA_MAX,0);
  i.c = rand_int(IDEA_MAX,0);
  i.empty = 0;
  return i;
}

Idea idea_empty() {
  Idea i;
  i.a = 0;
  i.b = 0;
  i.c = 0;
  i.empty = 1;
  return i;
}

#endif
