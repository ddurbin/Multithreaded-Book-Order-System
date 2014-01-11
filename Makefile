CC = gcc
CCFLAGS = -Wall -pthread -g
ARKEYS = rv
TARGET = book_order_system

all: book_order_system
book_order_system: book_order.o category_threads.o queue.o
	$(CC) $(CCFLAGS) -o book_order_system book_order.o category_threads.o queue.o
book_order.o: book_order.c
	$(CC) $(CCFLAGS) -c book_order.c
category_threads.o: category_threads.c
	$(CC) $(CCFLAGS) -c category_threads.c
queue.o: queue.c
	$(CC) $(CCFLAGS) -c queue.c
clean:
	rm -f sl *.o book_order_system
