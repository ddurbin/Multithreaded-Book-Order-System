#ifndef Book_Order_Book_Order_h
#define Book_Order_Book_Order_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//customer structure which holds pertinent information
struct customer{
    char *name;
    char *customerID;
    char *balance;
    char *address;
    char *state;
    char *zip;
    struct bookNode *purchasedList; //list of sucessfully purchased books
    struct bookNode *rejectedList; //list of unsucessfully purchased books
    
};
typedef struct customer customer;

//linked list structure for a queue holding the orders
struct orderQueue{
    struct orderNode *front;
    struct orderNode *rear;
};
typedef struct orderQueue orderQueue;

//orderNode for order linked list, holds order and node next
struct orderNode{
    char *title;
    char *price;
    char *customerID;
    char *category;
    struct orderNode *next;
};
typedef struct orderNode orderNode;

FILE *database, *order, *categories;
char *category;
customer **customerArr;
pthread_mutex_t lock;
int numThreads;

char *Concat(char *string, char letter);
customer *buildCustomer(FILE *fp);
int getIndex(customer *x);
int numCategories(FILE *fp);
unsigned int createThread();
void *processCategory(void *category);

#endif
