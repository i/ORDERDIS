#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utlist.h"
#include "uthash.h"

typedef struct A_ {
  char s[20];
  struct A_ *next;
  struct A_ *prev;

  UT_hash_handle hh;
} A;


A *new_A(char *str) {
  A *a = malloc(sizeof(A));
  strcpy(a->s, str);
  a->next = NULL;
  a->prev = NULL;

  return a;
}

A *hashtable = NULL;

void add_to_hash(A *a) {
  HASH_ADD_STR ( hashtable, s, a);
}

int main(int argc, char **argv) {

  A *head = NULL;
  add_to_hash(head);
  A *add = new_A("foo");
  A *add2 = new_A("bar");
  A *add3 = new_A("baz");
  A **ptr = &head;
  printf("%p\n", ptr);

  LL_PREPEND(head, add);
  printf("%s\n", head->s);
  ptr = &head;
  printf("%p\n", ptr);

  LL_PREPEND(head, add2);
  printf("%s\n", head->s);
  ptr = &head;
  printf("%p\n", ptr);

  LL_PREPEND(head, add3);
  printf("%s\n", head->s);
  ptr = &head;
  printf("%p\n", ptr);



}
