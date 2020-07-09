#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ROWSIZE 1024
#define INPUTSIZE 1024
#define NEWROWS 30
#define STACKSIZE 30
#define debugUNDO 1
#define debug 0

/*typedef struct{
    int key;
    char* row;
    int usable;  //short?
}Hash_Map;
typedef struct{
    char instruction;
    int firstRow;
    int lastRow;
}Memory;
typedef struct{
    int top;
    int capacity;
    Memory *memory;
}Stack;
typedef struct{
    char *row;
    int top;
    int capacity;
}Stack_Mem;
*/
typedef struct node{
    char row[ROWSIZE];
    struct node* next;
}node;

typedef struct Stack_node{
    int index1; //gli offest
    int index2;
    char instruction;
    node *mem;
    struct Stack_node* next;
}Stack_node;



//Hash_Map *hashArray[NEWROWS];
//int maxLength=0;
//Stack *stack;
//Stack_Mem stackMem;

node *head; //head of the list
Stack_node *first;  //LOGGER
Stack_node *firstUndo; //how much undoDone
int undoPossible = 0;




void parsing(char* command);
void changed(int index1, int index2);
void delete(int index1, int index2);
void print(int index1, int index2);
void undo(int nTimes);
void redo(int nTimes);
void operation(int index1, int index2, char o);
//int changeRow(int index);
//void push(Stack *s,Memory newElem);
//Memory pop(Stack *s);
//int isEmpty(Stack *s);
//Stack* createStack(int size);
node* searchNode(node* head_list,int index);
Stack_node* createStackNode(node* node,char instruction,int index1,int index2,Stack_node* stackNode);
node* createList(node* head_list,int index1,int index2,node* next);
node* createHead(node* newHead);


//TODO: varie free e controllare certi undo che non fanno il loro lavoro

int main(){
    char input[INPUTSIZE];
   // createStack(STACKSIZE);
    //head = (node*)malloc(sizeof(node));
    head = NULL;
    //head->next = NULL;
    //first = (Stack_node*)malloc(sizeof(Stack_node));
   // first->mem->next = NULL;
    first = NULL;
    //firstUndo = (Stack_node*)malloc(sizeof(Stack_node));
    firstUndo = NULL;


    while (1) {
        fgets(input, INPUTSIZE, stdin);
        if(strcmp(input,"q\n") != 0)
            parsing(input);
        else
            return 0;

    }

    //printf("Hello World!\n");
}

void parsing(char* command){
    int index1,index2= -1;
    char decision, *parsed;

    index1 = atoi(command);
    if(strchr(command,',') != NULL){
        parsed = strchr(command, ',');
        strncpy(parsed,parsed,strlen(parsed)-2);
        //printf("%d\n",strlen(++parsed)-2);
        //printf("%s",parsed);
        index2 = atoi(++parsed);
       // printf("%d\n",index2);

    }

        //decision = strchr(command, ' ');
        decision =  command[strlen(command)-2];
        //printf("%c",decision);
        operation(index1, index2, decision);

}

void operation(int index1, int index2, char o){
    switch (o) {
        case 'c' : changed(index1,index2); break;

        case 'd' : delete(index1,index2); break;

        case 'p' : print(index1,index2); break;

        case 'u' : undo(index1); break;

        case 'r' : redo(index1); break;

        default:
            return;
    }
}

void changed(int index1,int index2) {
        char row[ROWSIZE];
        node *next,*prev,*curr;
        prev = searchNode(head, index1 - 1);
        next = searchNode(head, index2 + 1);
        free(firstUndo);
        //firstUndo->next = NULL;
        firstUndo = NULL;
        if(prev == NULL){

            first = createStackNode(NULL,'c',index1,index2,first);
        }
        else{
            first = createStackNode(prev->next, 'c', index1, index2, first);

        }
    if(debugUNDO){
        undoPossible++;
        printf("%d",undoPossible);
    }
        curr = createList(head,index1,index2,next);
        if(prev == head){
            head = curr;
        } else{
            prev->next = curr;
        }
        //printf("%s",->next->row);
        fgets(row,ROWSIZE,stdin);
}
//COMANDI INUTILI FARE UNDO
void delete(int index1, int index2){
    node *prev,*next;
    prev = searchNode(head,index1 - 1);
    next = searchNode(head, index2 + 1);
    free(firstUndo);
    //firstUndo->next = NULL;
    firstUndo = NULL;
    if(prev == NULL){

        first = createStackNode(NULL,'d',index1,index2,first);
    }
    else{
        first = createStackNode(prev->next, 'd', index1, index2, first);

    }
    if(debugUNDO){
        undoPossible++;
        printf("%d",undoPossible);
    }

    prev->next = next;
}

void print(int index1, int index2){
   node *initial = searchNode(head,index1);
    for (int i = index1; i <=index2; ++i) {
        if(initial == NULL || initial == head){
            fprintf(stdout,".\n");

        } else{
            fprintf(stdout,initial->row);
            initial = initial->next;
        }

    }

}

void undo(int nTimes){
    node* prev, *next, *lastMem;
    //leggo il first lo rimetto nella mia memoria e semmai lo sostituisco con uno nuovo

    //ma per la d?! prendo first leggo dove andrebbe e scalo!!! quello dopo

    for (int i = 0; i < nTimes; ++i) {

        if(first!= NULL) {
            if(first->instruction == 'c'){
            prev = searchNode(head, first->index1 - 1);
            //printf("%s\n", prev->row);

            firstUndo = createStackNode(prev->next, first->instruction, first->index1, first->index2, firstUndo);
            if(debug){
                printf("%s",prev->next->row);
            }

            next = searchNode(head, first->index2 + 1); // next
            prev->next = first->mem;

            //printf("qua?\n");

            lastMem = searchNode(first->mem, first->index2 - first->index1);
            if(lastMem!= NULL){
                lastMem->next = next;
            }

            //printf("%s\n", lastMem->row); //?
            //free?

            }else if(first->instruction == 'd') {

                prev = searchNode(head, first->index1 - 1);
                next = prev->next;
                //printf("%s\n", prev->row);

                //printf("qua?\n");
                firstUndo = createStackNode(prev->next, first->instruction, first->index1, first->index2, firstUndo);
                if(debug){
                    printf("%s",prev->next->row);
                }

                lastMem = searchNode(first->mem, first->index2 - first->index1);
                if (lastMem != NULL) {
                    lastMem->next = next; //check here
                }
                prev->next = first->mem;
            }

        first = first->next;
            if(debugUNDO){
                undoPossible--;
                printf("%d",undoPossible);
            }

        }

    }

}
//redo undo al prossimo c o d rimetto a 0 il first

void redo(int nTimes){
    node* prev, *next, *lastMem;
    for (int i = 0; i < nTimes ; ++i) {

        if (firstUndo != NULL) {
            if (firstUndo->instruction == 'c') {
                prev = searchNode(head, firstUndo->index1 - 1);
                //printf("%s\n", prev->row);

                next = searchNode(head, firstUndo->index2 + 1); // next

                if(prev == NULL){

                    first = createStackNode(NULL,firstUndo->instruction,firstUndo->index1,firstUndo->index2,first);
                }
                else{
                    first = createStackNode(prev->next, firstUndo->instruction, firstUndo->index1, firstUndo->index1, first);

                }
                prev->next = firstUndo->mem;

                //printf("qua?\n");

                lastMem = searchNode(firstUndo->mem, firstUndo->index2 - firstUndo->index1);
                if (lastMem != NULL) {
                    lastMem->next = next;
                }

                //printf("%s\n", lastMem->row); //?
                //free?

                //rifare una lista?

            } else if (firstUndo->instruction == 'd') {
                prev = searchNode(head, firstUndo->index1 - 1);
                next = searchNode(head, firstUndo->index2 + 1);
                if(prev == NULL){

                    first = createStackNode(NULL,firstUndo->instruction,firstUndo->index1,firstUndo->index2,first);
                }
                else{
                    first = createStackNode(prev->next, firstUndo->instruction, firstUndo->index1, firstUndo->index1, first);

                }
                prev->next = next;
            }
            firstUndo = firstUndo->next;
            if(debugUNDO){
                undoPossible++;
                printf("%d",undoPossible);
            }
        }
    }

}

/*int changeRow(int index) {
/*
    char row[ROWSIZE];
    fgets(row,ROWSIZE,stdin);

    if(strcmp(row,".\n") != 0) {  //usefull?

        /*Hash_Map *hash = malloc(sizeof(Hash_Map));//(strlen(row))*sizeof(char) + sizeof(int)

        hash->row = row;
        hash->key = index;
        hash->usable = 1;
        ++maxLength;
        node *newNode = malloc(sizeof(node));
        if(head->next == NULL){
            head->next = newNode;
        }
        else {
            newNode->row = row;
            newNode->next = NULL;
        }
        current = newNode;
        printf("save %s", row);


       /* if (hashArray[index] != NULL) {
            push(stack,)


            // save first into memory
        }
        //hashArray[index] = hash;

        return 1;
    }
    return 0;

}

void push(Stack* s, Memory newElem){

    if(s->capacity == s->top){ //
        //aumenta la size
    } else{
        s->memory[++s->top] = newElem;
    }
}

Memory pop(Stack* s){
    Memory error;
    if(!isEmpty(s))
        return error;

    --s->top;
    return s->memory[s->top];
}
int isEmpty(Stack* s){
    if(s->top == -1){
        return 1;
    }
    else
        return 0;
}

Stack* createStack(int size){
    stack = malloc(sizeof(Stack));
    stack->top = -1;
    stack->capacity = size;
    stack->memory = (Memory*)malloc(sizeof(Memory));
}

Memory* createMemoryElem(char *content,int firstRow,int lastRow, char operation){

}*/

node* searchNode(node *head_list,int index){
    node* curr = head_list;
    if(index<0){
        return NULL;
    }
    for (int i = 0; i < index && curr!= NULL; ++i) {
        curr = curr->next;
    }
    return curr;

}

node* createList(node* head_list,int index1,int index2,node* next){
    node* myHead,*prev;
    char row[ROWSIZE];
    short code = 0;

    if(index1 == 1){
        myHead = (node*)malloc(sizeof(node));
        code= 1;
    } else{
        myHead = searchNode(head_list, index1 - 1);
    }
    prev = myHead;
    for (int i = index1; i <= index2; ++i) {   //search prev node -> next newNode etc
        fgets(row, ROWSIZE, stdin);
        node* newNode = (node *) malloc(sizeof(node));
        strcpy(newNode->row, row);
        newNode->next = NULL;
        prev->next = newNode;
        prev = prev->next;
    }
    prev->next = next;
    if(code){
        return myHead;
    }
    return myHead->next;
}

Stack_node* createStackNode(node* node,char instruction,int index1,int index2,Stack_node* stackNode){
    Stack_node *new = (Stack_node*) malloc(sizeof(Stack_node));
    new->index1 = index1;
    new->index2 = index2;
    new->instruction = instruction;
    new->mem = node;
   // if(stackNode == NULL){
     //   stackNode = (Stack_node*)malloc(sizeof(Stack_node)); //?
    //}
    new->next = stackNode;
    return new;
}










/*


// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

/* UTILITY FUNCTIONS */
/* Function to print an array
void printArray(int A[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}

*/