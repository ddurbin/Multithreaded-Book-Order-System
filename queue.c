#include "book_order.h"

//function initializes an empty queue
orderQueue *initQueue(){
    orderQueue *new = (orderQueue*)malloc(sizeof(orderQueue));
    new->front = NULL;
    new->rear = NULL;
    new->size = 0;
    return new;
}

//function builds the queue from the order FILE
orderQueue *buildQueue(FILE *order){
    char c;
    char *string = (char*)malloc(sizeof(char));
    string = "";
    c = getc(order);
    while(c != EOF){ //create string out of the FILE
        string = Concat(string, c);
        c = getc(order);
    }
    orderQueue *queue = (orderQueue*)malloc(sizeof(orderQueue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    
    char *token, *hold, *seps;
    hold = NULL;
    seps = "|\n";
    token = strtok_r(string, seps, &hold);
    while(token != NULL){ //create orderNodes from the string
        
        orderNode *new = (orderNode*)malloc(sizeof(orderNode));
        new->title = token;
        token = strtok_r(NULL, seps, &hold);
        new->price = token;
        token = strtok_r(NULL, seps, &hold);
        new->customerID = token;
        token = strtok_r(NULL, seps, &hold);
        new->category = token;
        token = strtok_r(NULL, seps, &hold);
        new->remainingBalance = 0;
        new->next = NULL;
        if(queue->front == NULL){ //first node
            queue->front = new;
            queue->rear = new;
            queue->size++;
        }
        else { // not the first node
            queue->rear->next = new;
            queue->rear = new;
            queue->size++;
        }
    }
    return queue;
}

//function inserts an order into the given customer's queues. list arg determines whether adding to purchased or rejected queue
orderQueue *insertOrderNode(customer *customer, orderNode *newNode, int list){
    
    if(list == 1){ //insert into purchased list

        if(customer->purchasedList->front == NULL){ //first add

            customer->purchasedList->front = newNode;
            customer->purchasedList->rear = newNode;

        }else{ //add to existing list

            customer->purchasedList->rear->next = newNode;
            customer->purchasedList->rear = newNode;

        }
        customer->purchasedList->size++;
        return customer->purchasedList;

    }else{ //adding to rejected list

        if(customer->rejectedList->front == NULL){ //first add

            customer->rejectedList->front = newNode;
            customer->rejectedList->rear = newNode;

            }else{ //add to existing list

                customer->rejectedList->rear->next = newNode;
                customer->rejectedList->rear = newNode;
            }
            customer->rejectedList->size++;
            return customer->rejectedList;
    }
}

//function frees allocated nodes in a given queue
void freeQueue(orderQueue *queue){
	int count = queue->size;
	int i;
	orderNode *ptr, *prev;
	ptr = queue->front;
	prev = NULL;
	for(i=0; i<count; i++){
		prev = ptr;
		ptr = ptr->next;
		free(prev);
	}
	return;
}



