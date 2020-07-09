#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ROWSIZE 1024
#define INPUTSIZE 30
//#define NEWROWS 30
//#define STACKSIZE 30
//#define debugUNDO 0
//#define debug 0


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


node *head = NULL; //head of the list
Stack_node *first = NULL;  //LOGGER
Stack_node *firstUndo = NULL; //how much undoDone





void parsing(char* command);
void changed(int index1, int index2);
void delete(int index1, int index2);
void print(int index1, int index2);
void undo(int nTimes);
void redo(int nTimes);
void operation(int index1, int index2, char o);
node* searchNode(node* head_list,int index);
Stack_node* createStackNode(node* node,char instruction,int index1,int index2,Stack_node* stackNode);
node* createList(node* head_list,int index1,int index2,node* next);



int main(){
    char input[INPUTSIZE];
   // createStack(STACKSIZE);
    //head = (node*)malloc(sizeof(node));
    //head = NULL;
    //head->next = NULL;
    //first = (Stack_node*)malloc(sizeof(Stack_node));
   // first->mem->next = NULL;
    //first = NULL;
    //firstUndo = (Stack_node*)malloc(sizeof(Stack_node));
    //firstUndo = NULL;


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
    /*if(debugUNDO){
        undoPossible++;
        printf("%d",undoPossible);
    }*/
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
    /*if(debugUNDO){
        undoPossible++;
        printf("%d",undoPossible);
    }*/

    prev->next = next;
}

void print(int index1, int index2){
   node *initial = searchNode(head,index1);
    for (int i = index1; i <=index2; ++i) {
        if(initial == NULL || initial == head){
            fputs(".\n",stdout);

        } else{
            fputs(initial->row,stdout);

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

            firstUndo = createStackNode(prev->next,'c', first->index1, first->index2, firstUndo);
            /*if(debug){
                printf("%s",prev->next->row);
            }*/

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
                firstUndo = createStackNode(prev->next, 'd', first->index1, first->index2, firstUndo);
                /*if(debug){
                    printf("%s",prev->next->row);
                }*/

                lastMem = searchNode(first->mem, first->index2 - first->index1);
                if (lastMem != NULL) {
                    lastMem->next = next; //check here
                }
                prev->next = first->mem;
            }

        first = first->next;
            /*if(debugUNDO){
                undoPossible--;
                printf("%d",undoPossible);
            }*/

        }

    }

}
//redo undo al prossimo c o d rimetto a 0 il first

void redo(int nTimes){
    node* prev, *next,*lastMem;
    for (int i = 0; i < nTimes ; ++i) {

        if (firstUndo != NULL) {
            if (firstUndo->instruction == 'c') {
                prev = searchNode(head, firstUndo->index1 - 1);
                //printf("%s\n", prev->row);

                next = searchNode(head, firstUndo->index2 + 1); // next

                if(prev == NULL){

                    first = createStackNode(NULL,'c',firstUndo->index1,firstUndo->index2,first);
                }
                else{
                    first = createStackNode(prev->next, 'c', firstUndo->index1, firstUndo->index2, first);

                }
                prev->next = firstUndo->mem;

                //printf("qua?\n");

                lastMem = searchNode(firstUndo->mem, firstUndo->index2 - firstUndo->index1);
               /*curr = redoList(head,firstUndo->index1,firstUndo->index2,next,firstUndo);

               if(prev == head){
                   head = curr;
               } else{
                   prev->next = curr;
               }*/
               // if(debugUNDO)
                 //   printf("%s",lastMem->row);
                if(lastMem != NULL) {
                    lastMem->next = next;
                } else{
                    prev->next = lastMem;
                }

                //printf("%s\n", lastMem->row); //?
                //free?

                //rifare una lista?

            } else if (firstUndo->instruction == 'd') {
                prev = searchNode(head, firstUndo->index1 - 1);
                next = searchNode(head, firstUndo->index2 + 1);
                if(prev == NULL){

                    first = createStackNode(NULL,'d',firstUndo->index1,firstUndo->index2,first);
                }
                else{
                    first = createStackNode(prev->next, 'd', firstUndo->index1, firstUndo->index2, first);

                }
                prev->next = next;
            }
            firstUndo = firstUndo->next;
            /*if(debugUNDO){
                undoPossible++;
                printf("%d",undoPossible);
            }*/
        }
    }

}


node* searchNode(node *head_list,int index){
    node* curr = head_list;
    for (int i = 0; i < index && curr!= NULL; ++i) {
        curr = curr->next;
        /*if(debugUNDO){
            printf("%d\n",i);
        }*/
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


