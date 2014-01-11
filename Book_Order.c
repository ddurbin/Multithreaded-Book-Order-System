#include "Book_Order.h"

customer **processDatabase(FILE *database){

	int i;
	char c;
	char *string = (char*)malloc(sizeof(char));
	string = "";
    c = getc(database);
    while(c != EOF){  
            
        string = Concat(string, c);
        c = getc(database);
    }
	
	customer **customerArr = (customer**)malloc(100*sizeof(customer*));

	char *token, *hold, *seps;
	int index;
	hold = NULL;
	seps = "|\n";
	token = strtok_r(string, seps, &hold);
	while(token != NULL){

		customer *new = (customer*)malloc(sizeof(customer));
		new->name = token;
		token = strtok_r(NULL, seps, &hold);
		new->customerID = token;
		token = strtok_r(NULL, seps, &hold);
		new->balance = token;
		token = strtok_r(NULL, seps, &hold);
		new->address = token;
		token = strtok_r(NULL, seps, &hold);
		new->state = token;
		token = strtok_r(NULL, seps, &hold);
		new->zip = token;
		new->purchasedList = NULL;
		new->rejectedList = NULL;;
		customerArr[getIndex(new)] = new;
		token = strtok_r(NULL, seps, &hold);
	}
	return customerArr;
}

orderQueue *buildQueue(FILE *order){
    char c;
    char *string = (char*)malloc(sizeof(char));
    string = "";
    c = getc(order);
    while(c != EOF){
        string = Concat(string, c);
        c = getc(order);
    }
    orderQueue *queue = (orderQueue*)malloc(sizeof(orderQueue));
    queue->front = NULL;
    queue->rear = NULL;
    
    char *token, *hold, *seps;
    hold = NULL;
    seps = "|\n";
    token = strtok_r(string, seps, &hold);
    while(token != NULL){
        
        orderNode *new = (orderNode*)malloc(sizeof(orderNode));
        new->title = token;
        token = strtok_r(NULL, seps, &hold);
        new->price = token;
        token = strtok_r(NULL, seps, &hold);
        new->customerID = token;
        token = strtok_r(NULL, seps, &hold);
        new->category = token;
        token = strtok_r(NULL, seps, &hold);
        new->next = NULL;
        if(queue->front == NULL){
            queue->front = new;
            queue->rear = new;
        }
        else {
            queue->rear->next = new;
            queue->rear = new;
        }
    }
    return queue;
}

//determine customer array index given new cutomer instance
int getIndex(customer *x){

	return atoi(x->customerID) % 100;
}

char *Concat(char *string, char letter){    //concatenate character to end of a string
    char *str = string;
    char let = letter;
    size_t len = strlen(str);
    char *result = (char*) malloc((len + 2) * sizeof(char));
    strcpy(result, str);
    result[len] = let;
    return result;
}

int numCategories(FILE *fp){

	 int ch;
	 int numLines= 0;
	do{

		ch = fgetc(fp);
		if(ch == '\n'){
			numLines++;
		}
	}while(ch != EOF);

	if(numLines != 0){
		numLines++;
	}
	return numLines;
}

unsigned int createThread(char *string){
	pthread_t newThread;
	numThreads++;
	return pthread_create(&newThread, NULL, processCategory, (void*)string);
}

void *processCategory(void *arg){
	char* category = (char*)arg;
	printf("Category is: %s\n", category);
	numThreads--;
	return NULL;
}



int main(int argc, char** argv){

	numThreads = 0;
	customerArr = (customer**)malloc(100*sizeof(customer*));
	database = fopen(argv[1], "r");
	order = fopen(argv[2], "r");
	categories = fopen(argv[3], "r");
	if(database == NULL || order == NULL || categories == NULL){

		printf("***Error opening files***\n");
		return 1;
	}
	customer **arr = processDatabase(database);
	orderQueue *queue = buildQueue(order);
    int i;
    for(i=0;i<100;i++){
        if(arr[i] != NULL){
            printf("name: %s\nID: %s\nBalance: %.2f\nAddress: %s\nState: %s\nZip: %s\n", arr[i]->name, arr[i]->customerID, atof(arr[i]->balance), arr[i]->address, arr[i]->state, arr[i]->zip);
        }
    }
    orderNode *ptr = queue->front;
    while(ptr != NULL){
        printf("title: %s\nprice: %s\nID: %s\ncategory: %s\n\n", ptr->title, ptr->price, ptr->customerID, ptr->category);
        ptr = ptr->next;
    }
	for(i=0;i<100;i++){
		if(arr[i] != NULL){

			printf("name: %s\nID: %s\nBalance: %.2f\nAddress: %s\nState: %s\nZip: %s\n", arr[i]->name, arr[i]->customerID, atof(arr[i]->balance), arr[i]->address, arr[i]->state, arr[i]->zip);
		}
	}
	
	int thread;
	char *string = malloc(sizeof(char));
	string = "";
	char c;
	int ch;
	
	do{
		c = getc(categories);
		ch = (int)c;
		if(ch  == '\n' || ch == EOF){
			thread = createThread(string);
			if(thread != 0){
				
				printf("***Error creating new thread***\n");
				return 1;
			}
			string = "";
		}else{
			string = Concat(string, c);
		}
		
	}while(ch != EOF);

	while(numThreads != 0){

		sleep(1);
	}

	return 0;
}
