#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "order.h"
#include "customer.h"


int main(int argc, char **argv) {
  customer *ian;

  ian = new_customer(1, "ian", 450.0, "426 rosemont ringoes rd", "new jersey", "08559");
  add_customer(ian);

  LL_PREPEND(ian->orders,  new_order(5, "the land before time", 5.5));


  remove_customer(ian);
  free(ian);

  return 0;
}

