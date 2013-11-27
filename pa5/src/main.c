#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "order.h"
#include "customer.h"
#include "category.h"


void *worker(void *queue) {
  customer *c;
  order *o;
  int err;
  category *q = (category*)queue;

  while(1) {
    /* Get lock */
    pthread_mutex_lock(&q->lock);

    /* check to see if queue is empty */ 
    if (q->next != NULL) {
      /* if queue not empty, get first item, remove from queue */
      o = dequeue(q->cat);

      /* get customer from database using find_customer */
      c = find_customer(o->customer_id);

      /* validate customer ID */
      if (c == NULL) {
        fprintf(stderr, "invalid customer id %d\n", o->customer_id);
        continue;
      }

      pthread_mutex_lock(&(c->lock));         // LOCK CUSTOMER

      /* check customer balance */

      if (c->balance >= o->price) { /* if balance is > transaction amount */
        c->balance -= o->price;     /* remove amount from customer balance */
        o->success = SUCCESS;       /* set order status to successful */
        o->remaining_balance = c->balance; /* show customer's balance on order */
      } else {
        o->success = FAILURE;       /* set order status to failure if order doesn't succeed */
      }
      add_order(c, o);              /* add order to customer's proper order list */
      err = pthread_mutex_unlock(&(c->lock)); // UNLOCK CUSTOMER
      pthread_mutex_unlock(&q->lock);
    }
    else if (q->producing) {
      /* Wait for signal */
      pthread_cond_wait(&(q->waiter), &(q->lock)); // WAITS, BLOCKS
      /* Got signal */
      pthread_mutex_unlock(&(q->lock));
    }
    else {
      /* Done consuming this queue */
      pthread_mutex_unlock(&q->lock);
      return NULL;
    }
  }
  return NULL;
}

int main(int argc, char **argv) {
  customer *c;
  category *q;
  order *o;
  FILE *f;
  char line[300];
  int err;
  pthread_t *tid;

  if (argc != 3) {
    printf("Usage: ./order path/to/database path/to/orders\n");
    exit(1);
  }


  f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Customer file not found\n");
    exit(1);
  }

  /* Add customers to database */
  while (fgets(line, 300, f)) {
    c = malloc(sizeof(customer));
    strcpy(c->name, strtok(line, "\""));     /* Assign name */
    c->id = atoi(strtok(NULL, "|"));         /* Assign ID */
    c->balance = atof(strtok(NULL, "|"));    /* Assign balance */
    strcpy(c->address, strtok(NULL, "|"));   /* Assign address */
    strcpy(c->state, strtok(NULL, "|"));     /* Assign state */
    strcpy(c->zip, strtok(NULL, "|"));       /* Assign zipcode */
    pthread_mutex_init(&c->lock, NULL);      /* Initializes the lock */
    add_customer(c);
  }

  fclose(f);                                /* Close database.txt */

  f = fopen(argv[2], "r");
  if (f == NULL) {
    fprintf(stderr, "Order file not found\n");
    exit(1);
  }

  while (fgets(line, 300, f)) {
    o = malloc(sizeof(order));
    strcpy(o->title, strtok(line, "\""));       /* Assign title */
    o->price = atof(strtok(NULL, "|"));
    o->customer_id = atoi(strtok(NULL, "|"));
    strcpy(o->category, strtok(NULL, " \r\n"));
    o->success = PENDING;

    /* If it's a new category, add new dummy node queue to hashtable */
    if (!(q = find_queue(o->category))) {

      q = new_category(o->category, NULL);
      tid = malloc(sizeof(pthread_t));
      add_queue(q);
      err = pthread_create(tid, NULL, &worker, q);
      q->val = tid;
      if (err != 0) {
        fprintf(stderr, "Thread could not be created!\n");
        exit(1);
      }
    }

    enqueue(q, new_category(o->category, o));
  }
  fclose(f);

  /* Send wakeup call to queues */
  for(q = queues; q != NULL; q = q->hh.next) {
    pthread_mutex_lock(&q->lock);
    q->producing = 0;
    pthread_cond_signal(&q->waiter);
    pthread_mutex_unlock(&q->lock);
  }

  for(q = queues; q != NULL; q = q->hh.next) {
    pthread_join(*(pthread_t*)q->val, NULL);
  }

  print_summary(c);

  destroy_db();

  return 0;
}
