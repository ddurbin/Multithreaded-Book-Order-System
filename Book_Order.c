#include "book_order.h"

static pthread_t *tids; //static array of thread ID's for pthread_wait function calls

char *Concat(char *string, char letter){    //concatenate character to end of a string
    char *str = string;
    char let = letter;
    size_t len = strlen(str);
    char *result = (char*) malloc((len + 2) * sizeof(char));
    strcpy(result, str);
    result[len] = let;
    return result;
}


int main(int argc, char** argv){
    totalRevenue = 0;
    database = fopen(argv[1], "r"); order = fopen(argv[2], "r");
    if(database == NULL || order == NULL){ //file error checking

        printf("***Error opening files***\n");
        return 1;
    }

    if(pthread_mutex_init(&lock, NULL) != 0){ //initialize mutex lock
                
        printf("***Mutex init failed***\n");
    }

    customerArr = processDatabase(database); //customer array initialization
    if(customerArr == NULL){ //check for empty database
        printf("***Error creating database***\n");
        return 1;
    }
    queue = buildQueue(order); //build order queue with order file
    
    int i;
    char *categories = (char*) argv[3];
    tids = (pthread_t*)malloc(numCategories(categories)*sizeof(pthread_t)); //initialize pthread array for each category
    tidIndex = 0; //thread ID array index
    int index = 0; //Categories string index
    char *string = malloc(sizeof(char));
    string = "";
    char c;
        
    do{ //create thread for each category
        c = categories[index];
        if(c  == ' ' || c == '\0'){ // new category, create a thread
            if(pthread_create(&tids[tidIndex], NULL, processCategory, (void*)string) != 0){  //create thread passing category argument
                                
                printf("***Error creating new thread***\n");
                return 1;
            }
        tidIndex++;
        string = "";
        }else{ //not a new category, add character to string
            string = Concat(string, c);
        }
    index++;       
    }while(c != '\0');

    for(i=0; i<numCategories(categories); i++){ // Main thread joins all the category threads and waits until finished
       pthread_join(tids[i],NULL);
    }
    
    output(customerArr); //print out final report
    //free allocated memroy
    freeCustomers(customerArr);
    freeQueue(queue);
    free(tids);
    fclose(database);
    fclose(order);
    return 0;
}
