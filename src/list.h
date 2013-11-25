#ifndef LIST_H
#define LIST_H

#include "utlist.h"

typedef struct lnode_ {
  void *val;
  struct lnode_ *next;
  struct lnode_ *prev;
} lnode;


#endif
