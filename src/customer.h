#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "order.h"
#include "uthash.h"
#include "utlist.h"


/* Customer struct */
typedef struct {
  int id;             /* key */
  char name[20];
  float balance;
  char address[20];
  char state[20];
  char zip[10];
  lnode *orders;

  UT_hash_handle hh;  /* makes this structure hashable */
} customer;

/* Global hashtable for customers */
customer *customers = NULL;

customer *new_customer(int id, char *name, float balance,
    char *address, char *state, char *zip) {

  customer *c = malloc(sizeof(customer));
  c->id = id;                  /* Id  */
  strcpy(c->name, name);       /* Name */
  c->balance = balance;        /* Balance */
  strcpy(c->address, address); /* Address */
  strcpy(c->state, state);     /* State */
  strcpy(c->zip, zip);         /* Zipcode */
  c->orders = NULL;

  return c;
}

/* Add order to customer's list of orders */
void add_order(customer *c, order *o) {

}

/* Add customer to hashtable */
void add_customer(customer *c) {
  HASH_ADD_INT( customers, id, c );  /* id: name of key field */
}

/* Get customer from hashtable */
/* s can be null on failure */
customer *find_customer(int cust_id) {
  customer *s;

  HASH_FIND_INT( customers, &cust_id, s);
  return s;
}

/* Remove customer from hashtable */
void remove_customer(customer *c) {
  HASH_DEL( customers, c);
}
#endif
