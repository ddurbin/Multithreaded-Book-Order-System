#include "book_order.h"

//function creates customer array database
customer **processDatabase(FILE *database){
        char c;
        char *string = (char*)malloc(sizeof(char));
        string = "";
    c = getc(database);
    while(c != EOF){  //turn file into string
            
        string = Concat(string, c);
        c = getc(database);
    }
    if(strlen(string) == 0){

        return NULL;
    }    
        customer **customerArr = (customer**)malloc(200*sizeof(customer*));

        char *token, *hold, *seps;
        hold = NULL;
        seps = "|\n";
        token = strtok_r(string, seps, &hold);
        while(token != NULL){ // tokenize string into individual customers

                customer *new = (customer*)malloc(sizeof(customer));
                new->name = token;
                token = strtok_r(NULL, seps, &hold);
                new->customerID = token;
                token = strtok_r(NULL, seps, &hold);
                new->balance = atoi(token);
                token = strtok_r(NULL, seps, &hold);
                new->address = token;
                token = strtok_r(NULL, seps, &hold);
                new->state = token;
                token = strtok_r(NULL, seps, &hold);
                new->zip = token;
                new->purchasedList = initQueue();
                new->rejectedList = initQueue();
                customerArr[getIndex(new->customerID)] = new;
                token = strtok_r(NULL, seps, &hold);
        }
        return customerArr;
}

//function gets the index of the customer array from CustomerID
int getIndex(char *customerID){

        return atoi(customerID) % 200;
}

//funciton determines the number of categories from the categories argument string
int numCategories(char *categories){

    int num = 0;
    int i;
    char c;
    for(i=0; i<strlen(categories); i++){ //loop through string

        c = categories[i];
        if(c == ' '){ //separator parse category
            num++;
        }
    }
    num++;
    return num;
}

//function that each thread calls, processing the orders for their individual category
void *processCategory(void *arg){
    pthread_mutex_lock(&lock); //lock mutex
    char* category = (char*)arg;
    customer *tempCustomer;
    orderNode *tempOrder = queue->front;
    int queueSize = queue->size;
    int match;
    while(queueSize != 0){ //loop through order queue attaining each order matching the category 
                 
        match = strcmp(category, tempOrder->category);
        if(match == 0){ //the order matches the thread category  
            tempCustomer = customerArr[getIndex(tempOrder->customerID)]; //get customer
            if(tempCustomer->balance >= atof(tempOrder->price)){ //customer has enough money to purchase order
               	tempCustomer->balance = tempCustomer->balance - atoi(tempOrder->price);
               	orderNode *purchased = (orderNode*)malloc(sizeof(orderNode));
               	purchased->title = tempOrder->title;
               	purchased->price = tempOrder->price;
               	purchased->customerID = tempOrder->customerID;
               	purchased->category = tempOrder->category;
               	purchased->remainingBalance = tempCustomer->balance;
               	tempCustomer->purchasedList = insertOrderNode(tempCustomer, purchased, 1);
               	totalRevenue += atof(tempOrder->price);
               	printf("\nBook Purchase Successfull:\n\t%s|$%s|%s|%s|%s|%s\n", purchased->title, purchased->price, tempCustomer->name, tempCustomer->address, tempCustomer->state, tempCustomer->zip);
            }else{ //customer does not hace sufficient funds
              	orderNode *rejected = (orderNode*)malloc(sizeof(orderNode));
                rejected->title = tempOrder->title;
               	rejected->price = tempOrder->price;
               	rejected->customerID = tempOrder->customerID;
               	rejected->category = tempOrder->category;
               	rejected->remainingBalance = tempCustomer->balance;
             	tempCustomer->rejectedList = insertOrderNode(tempCustomer, rejected, 2);
               	printf("\nRejected Order: \n\t%s|%s|$%s|%s\n", tempCustomer->name, tempOrder->title, tempOrder->price, tempOrder->category);
            }

        }
    	tempOrder = tempOrder->next; //get next order
    	queueSize--;
    }
    pthread_mutex_unlock(&lock); //unlock mutex
    return NULL;
}

//function prints out the final report
void output(customer **customerArray){
    int i;
    int size;
    orderNode *ptr;
    for(i=0;i<200;i++){
        if(customerArray[i] != NULL){
            printf("=== BEGIN CUSTOMER INFO ===\n### BALANCE ###\n");
            printf("Customer name: %s\nCustomer ID: %s\nRemaining credit balance after all purchases: $%.2f\n", customerArray[i]->name, customerArray[i]->customerID, customerArray[i]->balance);
            printf("\n### SUCCESSFUL ORDERS ###\n");
            size = customerArray[i]->purchasedList->size;
            if(size > 0){
                ptr = customerArray[i]->purchasedList->front;
                while(size > 0){
                    printf("%s|$%s|$%.2f\n", ptr->title, ptr->price, ptr->remainingBalance);
                    ptr = ptr->next;
                    size--;
                }
            }
            printf("### REJECTED ORDERS ###\n");
            size = customerArray[i]->rejectedList->size;
            if(size > 0){
                ptr = customerArray[i]->rejectedList->front;
                while(size > 0){
                    printf("%s|$%s\n", ptr->title, ptr->price);
                    ptr = ptr->next;
                    size--;
                }
            }
            
            printf("=== END CUSTOMER INFO ===\n\n");
        }
    }
    printf("***Total Revenue: $%.2f***\n", totalRevenue);
    return;
}

//function frees all allocated customers
void freeCustomers(customer **customerArr){

	int i;
	for(i=0; i<200; i++){

		if(customerArr[i] != NULL){

			free(customerArr[i]);
		}
	}
	return;
}




