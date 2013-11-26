#ifndef ORDER_H
#define ORDER_H

#include <stdlib.h>
#include <string.h>

#include "uthash.h"


/* Order types */
enum order_types { FAILURE, SUCCESS, PENDING };

/* Order struct */
typedef struct {
  char title[300];
  float price;
  int customer_id;
  char category[50];  /* hash key for queues */
  int success;        /* 1: successful, -1: failed, 0: pending */

} order;

/* Global hashtable for customers */
order *orders = NULL;

order *new_order(int customer_id, char *title, float price) {

  order *o = malloc(sizeof(order));
  o->customer_id = customer_id;  /* Customer ID */
  strcpy(o->title, title);       /* Book title */
  o->price = price;              /* Price of book */
  o->success = PENDING;          /* Success of order */

  return o;
}

#endif
