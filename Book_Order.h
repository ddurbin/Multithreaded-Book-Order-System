#ifndef book_order_book_order_h
#define book_order_book_order_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//customer structure which holds pertinent information
struct customer{
    char *name;
    char *customerID;
    float balance;
    char *address;
    char *state;
    char *zip;
    struct orderQueue *purchasedList; //list of sucessfully purchased books
    struct orderQueue *rejectedList; //list of unsucessfully purchased books
};
typedef struct customer customer;

//linked list structure for a queue holding the orders
struct orderQueue{
    struct orderNode *front;
    struct orderNode *rear;
    int size;
};
typedef struct orderQueue orderQueue;

//orderNode for order linked list, holds order and node next
struct orderNode{
    char *title;
    char *price;
    char *customerID;
    char *category;
    float remainingBalance;
    struct orderNode *next;
};
typedef struct orderNode orderNode;

//FILES for database and Orders args
FILE *database, *order;
//array of customers
customer **customerArr;
//queue of orders
orderQueue *queue;
//mutex lock for threads
pthread_mutex_t lock;
//pthread_t array index
int tidIndex;
float totalRevenue;

//category_threads.c Functions
char *Concat(char *string, char letter);
customer **processDatabase(FILE *database);
int getIndex(char *customerID);
int numCategories(char *categories);
void *processCategory(void *category);
void output(customer **customerArray);
void freeCustomers(customer **customerArr);

//queue.c Functions
orderQueue *initQueue();
orderQueue *buildQueue(FILE *order);
orderQueue *insertOrderNode(customer *customer, orderNode *newNode, int list);
void freeQueue(orderQueue *queue);

#endif
