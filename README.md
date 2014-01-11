Multithreaded-Book-Order-System
===============================
This program simulates a book order system. It creates a producer which reads in all the orders and then spawns multiple consumer threads which process their individual book orders.


We start by the main thread processing the database and orders FILES. Once processed the database of customers is stored in an array of max size 200. The customers can then be accessed by using the getIndex function which determines the index by modding the customer ID by 200. Once processed the orders are stored in a queue which is built using a linked list. Upon completion of the initial setup, the main thread takes the categories argument and creates a string which stores the text. A do-while loop runs through the categories string and creates a thread for each category passing the category string as a parameter, upon creation it stores a thread ID into an array of type pthread_t. Upon creation of all threads, the main thread runs a for loop calling pthread_join, to wait on all the threads. Once the thread is created it calls the processCategory function. Inside the function it locks the mutex and then begins processing all orders from the order queue which match the same category. Upon retrieving a matching order it accesses the customer from the customer array using it’s ID. If the customer has sufficient funds to purchase the order the book is purchased and the order is added to the customers purchased order queue, if the customer does not have sufficient funds then the order is added to the rejected order queue, their balance is adjusted as necessary. After each matching order is accessed, a printout of it’s status is given. Upon completion of the processCategory function the thread unlocks the mutex and finishes. After all threads have completed their processing, the main thread takes over and calls the output function which prints a overall report of the program. M!ain then frees all allocated memory and the program ends.!
**Notes: !
• The customer array is a fixed size of 200, undefined behavior with customers in excess of
200.!
• The getIndex function mods the customerID by 200, undefined behavior and possible
collisions for customerID’s greater than 200.

