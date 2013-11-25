#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "order.h"
#include "customer.h"


/*int main(int argc, char **argv) {*/
/*  customer *ian;*/
/*  order *order;*/
/*  lnode *temp_node;*/
/**/
/*  ian = new_customer(1, "ian", 450.0, "426 rosemont ringoes rd", "new jersey", "08559");*/
/*  add_customer(ian);                                 /* add ian to hashtable */*/
/*  order = new_order(5, "the land before time", 5.5); /* Create new order object */*/
/*  LL_PREPEND(ian->orders,  new_lnode(order));        /* Add order to the list of Ian's orders */*/
/**/
/*  remove_customer(ian);*/
/*  free(ian);*/
/**/
/*  return 0;*/
/*}*/
/**/


int main() {

  read customer database;  database.txt

  store all the customers in hash table;

  read all categories from categories.txt;
  for each category
    create a new queue category; // a queue contains a list of orders that need to be processed
    create a thread for category; // thread will be responsible for processing orders from queue

  read in the orders.txt
  for each order:
    place order in proper queue;


  After main has read all orders and place them in queues
  main will pthread_join each thread;
  Write out database to finalreport
}

*order.customer_id === order->customer_id => true

void worker_thread() {
  if (can get order item from Q) {
    order = Q.pop
    customer = find_customer(order.customer_id); /* get customer from hash table */
    if customer can afford book {
      decrement customer balance
      add order to customer.successful_orders
    } else {
      order status is unsuccessful
      add order to customer.unsuccessful_orders
    }
  }

}
