ORDERDIS
========
CS 214 Assignment 5 -- Book Order System
========
Ian Lozinski (iml22), Edward Zaneski(epz5)


Usage: ```make; ./order path/to/database path/to/orders```

The goal of this assignment was to produce a multithreaded book order system 
consisting of the producer-consumer model. 


The program takes in a "database" file consisting of customers and an orders file
consisting of orders. 

Each book order is handled by a separate book order processor thread 
(the consumers). We opted to put the book orders into separate queues, one for 
each category and created and stored in a hashtable which uses the category name 
as the hash key. Each consumer thread  has exclusive access to its specialized 
queue. The threads are dynamically created as new categories are seen in the 
input file.

###Architecture:
A thread is spawned and assigned a category with a queue. If the queue is empty,
the thread checks if the producer is done taking in orders. If it is, the thread
dies. If the producer is still processing orders, the thread goes into a conditional
wait. Every time the producer adds an item to the queue, it sends a wakeup signal
to the thread. The producer then joins on each consumer thread while they work to
prevent it from ending early.



###Complexity analysis:
Inserting customer into database O(1) time.
Inserting order into database O(1/q) where q is the number of queues.
