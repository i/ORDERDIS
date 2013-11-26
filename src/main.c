#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"
#include "order.h"
#include "customer.h"
#include "category.h"

float total, subtotal;

void *worker(void *queue) {
  return NULL;
  customer *c;
  order *o;
  /* check to see if queue is empty */
  while (((category*)queue)->next != NULL) {
    /* if queue not empty, get first item, remove from queue */
    o = dequeue(((category*)queue)->cat);
    printf("%s\n", o->title);

    /* get customer from database using find_customer */
    c = find_customer(o->customer_id);

    /* validate customer ID */
    if (c == NULL) {
      fprintf(stderr, "invalid customer id %d\n", o->customer_id);
      continue;
    }

    /* check customer balance */
    if (c->balance >= o->price) { /* if balance is > transaction amount */
      c->balance -= o->price;     /* remove amount from customer balance */
      o->success = SUCCESS;
      o->remaining_balance = c->balance;
    } else {                      /* else if balance < transaction amount */
      o->success = FAILURE;
    }
    add_order(c, o);              /* add order to customer's proper order list */
  }
  return NULL;
}

int main(int argc, char **argv) {
  customer *c;
  category *q;
  order *o;
  lnode *n;
  FILE *f;
  char line[300];
  int err;
  pthread_t *tid;


  f = fopen("./data/database.txt", "r");

  /* Add customers to database */
  while (fgets(line, 300, f)) {
    c = malloc(sizeof(customer));
    strcpy(c->name, strtok(line, "\""));     /* Assign name */
    c->id = atoi(strtok(NULL, "|"));         /* Assign ID */
    c->balance = atof(strtok(NULL, "|"));    /* Assign balance */
    strcpy(c->address, strtok(NULL, "|"));   /* Assign address */
    strcpy(c->state, strtok(NULL, "|"));     /* Assign state */
    strcpy(c->zip, strtok(NULL, "|"));       /* Assign zipcode */

    add_customer(c);
  }

  fclose(f);                                /* Close database.txt */

  f = fopen("./data/orders.txt", "r");

  while (fgets(line, 300, f)) {
    o = malloc(sizeof(order));
    strcpy(o->title, strtok(line, "\""));
    o->price = atof(strtok(NULL, "|"));
    o->customer_id = atoi(strtok(NULL, "|"));
    strcpy(o->category, strtok(NULL, " \r\n"));
    o->success = PENDING;

    /* If it's a new category, add new dummy node queue to hashtable */
    if (!(q = find_queue(o->category))) {

      q = new_category(o->category, NULL);
      tid = malloc(sizeof(pthread_t));
      err = pthread_create(tid, NULL, &worker, q);
      q->val = tid;
      if (err != 0) {
        fprintf(stderr, "Thread could not be created!\n");
        exit(1);
      }
      add_queue(q);
    }


    enqueue(q, new_category(o->category, o));
  }


  print_summary(c);


  return 0;
}
