#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdlib.h>
#include <string.h>

#include "order.h"
#include "uthash.h"
#include "utlist.h"
#include "category.h"


/* Customer struct */
typedef struct {
  int id;             /* key */
  char name[20];
  float balance;
  char address[20];
  char state[20];
  char zip[10];
  order *successful_orders;
  order *failed_orders;

  pthread_mutex_t lock;
  UT_hash_handle hh;  /* makes this structure hashable */
} customer;

/* Global hashtable for customers */
customer *customers = NULL;

/* Add order to customer's list of orders */
void add_order(customer *c, order *o) {
  if (o->success == SUCCESS) {
    if (c->successful_orders == NULL) {
      c->successful_orders = o;
      o->next = NULL;
    }
    else {
      o->next = c->successful_orders;
      c->successful_orders = o;
    }
  }
  else if (o->success == FAILURE) {
    if (c->failed_orders == NULL) {
      c->failed_orders = o;
      o->next = NULL;
    }
    else {
      o->next = c->failed_orders;
      c->failed_orders = o;
    }
  } else {
    fprintf(stderr, "tried adding incomplete order to %s's order list\n", c->name);
  }
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


void print_summary(customer *c) {
  order *o;
  float subtotal, total = 0;;

  /* Print out order summary for each user */
  for(c = customers; c != NULL; c = c->hh.next) {
    subtotal = 0;
    printf("=== BEGIN CUSTOMER INFO ===\n");
    printf("### BALANCE ###\n");
    printf("Customer name: %s\n", c->name);
    printf("Customer ID number: %d\n", c->id);
    printf("Remaining credit balance after all purchases (a dollar amount): $%'.2f\n", c->balance);
    printf("### SUCCESSFUL ORDERS ###\n");
    for (o = c->successful_orders; o != NULL; o = o->next) {
      printf("\"%s\"| %'.2f| %'.2f\n", o->title, o->price, o->remaining_balance);
      subtotal += o->price;
    }
    printf("Total amount spent by customer: $%'.2f\n", subtotal);
    total += subtotal;
    printf("### REJECTED ORDERS ###\n");
    for (o = c->failed_orders; o != NULL; o = o->next) {
      printf("\"%s\"| %'.2f\n", o->title, o->price);
    }
    printf("### END CUSTOMER INFO ###\n");
  }
  printf("Total amount spent by all customer: $%'.2f\n", total);
}


void destroy_db() {
  customer *c, *d;
  order *o, *p;
  category *q, *r;

  /* Destroy all CUSTOMERS and their ORDERS */
  c = customers;
  while(c != NULL) {
    o = c->successful_orders;
    while (o != NULL) {
      p = o->next;
      free(o);
      o = p;
    }
    o = c->failed_orders;
    while (o != NULL) {
      p = o->next;
      free(o);
      o = p;
    }
    HASH_DEL(customers, c);
    d = c;
    c = c->hh.next;
    free(d);
  }

  /* Destroy all queues */
  q = queues;
  while (q != NULL) {
    HASH_DEL(queues, q);
    r = q;
    free(q->val);
    q = q->hh.next;
    free(r);
  }

}

#endif
