#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"
#include "order.h"
#include "customer.h"
#include "category.h"


void *worker(void *queue) {
  customer *c;
  order *o;
  int err;
  category *q = (category*)queue;

/*  pthread_detach(pthread_self());*/
  while(1) {
    pthread_mutex_trylock( &(q->lock) );          // LOCK QUEUE

    /* check to see if queue is empty and producer is done producing */
    if ( q->next != NULL && q->producing) {
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
      printf("C: %d\n", err);
      err = pthread_mutex_unlock(&(q->lock)); // UNLOCK QUEUE
      printf("Q: %d\n", err);
    }
    else if (q->producing == 0) {
      printf("Done consuming in %s\n", q->cat);
      pthread_mutex_unlock(&(q->lock));
      pthread_exit(0);
    }
    else {
      /* pthread_cond_wait */
      printf("Waiting for signal in %s\n", q->cat);
      pthread_cond_wait(&(q->waiter), &(q->lock)); // WAITS, BLOCKS
      printf("Got signal in %s, int is: %d\n", q->cat, q->producing);
    }
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
    pthread_mutex_init(&c->lock, NULL);      /* Initializes the lock */
    add_customer(c);
  }

  fclose(f);                                /* Close database.txt */

  f = fopen("./data/orders.txt", "r");

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

  sleep(5);

  for(q = queues; q != NULL; q = q->hh.next) {
    /* say hey i am done reading orders in */
    pthread_mutex_lock(&q->lock);
    q->producing = 0;
    pthread_cond_signal(&q->waiter);
    pthread_mutex_unlock(&q->lock);
    /* pthread_cond_signal(q->conditioanl) */
    printf("Sending wakeup call to %s\n", q->cat);
  }

  for(q = queues; q != NULL; q = q->hh.next) {
    pthread_join(*(pthread_t*)q->val, NULL);
  }

  print_summary(c);

  return 0;
}
