#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "order.h"
#include "customer.h"
#include "category.h"


int main(int argc, char **argv) {
  customer *c;
  category *q;
  order *o;
  lnode *n;
  FILE *f;
  char line[300];

  /* add_queue(new_category("yo", NULL));
  * q = find_queue("yo");
  * o = malloc(sizeof(order));
  * strcpy(o->title, "Yest");
  * enqueue(q, new_category("yo", o));
  * printf("%s\n", dequeue("yo")->title);
  */

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
    strcpy(o->category, strtok(NULL, " \n"));
    o->success = PENDING;

    /* If it's a new category, add new dummy node queue to hashtable */
    if (!(q = find_queue(o->category))) {
      q = new_category(o->category, NULL);
      add_queue(q);
    }

    enqueue(q, new_category(o->category, o));
    printf("%s\n", dequeue(o->category)->title);
  }


  return 0;
}
