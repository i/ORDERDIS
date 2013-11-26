#ifndef CATEGORY_H
#define CATEGORY_H

#include "utlist.h"
#include "uthash.h"

typedef struct category_{
  char *cat;
  void *val;
  struct category_ *next;
  struct category_ *prev;
  UT_hash_handle hh;  /* makes this structure hashable */
} category;

/* Returns a new category queue node */
category *new_category (char *cat, void *val) {
  category *q = malloc(sizeof(category));

  q->cat = strdup(cat);
  q->val = val;
  q->prev = NULL;
  q->next = NULL;
  return q;
}

/* Global hashtable for queues */
category *queues = NULL;

/* Add category to hashtable */
void add_queue(category *q) {
  HASH_ADD_KEYPTR(hh, queues, q->cat, strlen(q->cat), q);  /* name: name of key field */
}

/* Get queue from hashtable */
/* q can be null on failure */
category *find_queue(char *name) {
  category *q;

  HASH_FIND_STR(queues, name, q);
  return q;
}

void enqueue(category *head, category *add) {
  LL_APPEND(head, add);
}


/* Gets an order object and deletes it from front of queue */
order *dequeue(char *category_name) {
  order *o;
  category *q = find_queue(category_name);

  /* Sanity checks */
  if (q != NULL) {
    if(q->next != NULL && q->next->val != NULL) {
      o = (order *)q->next->val;
      LL_DELETE(q, q->next);
      return o;
    }
    else {
      printf("Found queue, but nothing in it.\n");
      return NULL;
    }
  } else {
    fprintf(stderr, "Couldn't find queue\n", category_name);
    return NULL;
  }
}

#endif
