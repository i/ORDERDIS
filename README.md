ORDERDIS
========
Assignment 5 -- Book Order System
========
Ian Lozinski (iml22), Edward Zaneski(epz5)
CS 214

The goal of this assignment was to produce a a multithreaded book order system 
consisting of the producer-consumer model. 


The program takes in a "database" file consiting of customers and an orders file
consiting of orders. 

Each book order is handled by a separate book order processor thread 
(the consumers). We opted to put the book orders into separate queues, one for 
each category and created and stored in a hashtable which uses the category name 
as the hash key. Each consumer thread  has exclusive access to its specialized 
queue. The threads are dynamically created as new categories are seen in the 
input file.


###Complexity analysis:
  Inserting customer into database O(1) time.
  Inserting order into database O(1/q) where q is the number of queues.
