#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INITARRAY 30
#define INPUTSIZE 1028


//Stack_node *firstUndo = NULL;

//contatore per le istruzioni nulle
//puntatore per le stringhe
//usare scanf! controllare 0,0d o 0,0c

char** text;
char** memory;
char** undoList;
char** logger;
char input[INPUTSIZE];

// OKAY
//size_t arrsize = number;


//change the size -> realloc(text, arrSize * sizeof(int))

//initialized array or malloc(numberEl* sizeof()int);



// free(text)











int used= 0;
int size = INITARRAY;
int sizeLog = 0;
int lastCommand = 0;
size_t lengthText = 0;
size_t lengthLog = 0;

void parsing(char* command);
void changed(int index1, int index2);
void delete(int index1, int index2);
void print(int index1, int index2);
void undo(int nTimes);
void redo(int nTimes);
void operation(int index1, int index2, char o);



int main(){
    text = malloc(INITARRAY * sizeof(char*));
    logger = malloc(INITARRAY * sizeof(char*));


    while (1) {
        fgets(input, INPUTSIZE,stdin);
        parsing(input);

    }
    return 0;
}

void parsing(char* command){
    int index1=0,index2= 0,i,offset = 48,maxNum =57;
    char decision;// *parsed1,*parsed2;

    /*if(command[0] == 'q'){
        end = 1;
        return;                    //NOT USEFUL
    }
    index1 = atoi(command);
    if(strchr(command,',') != NULL){
        parsed = strchr(command, ',');
       // strncpy(parsed,parsed,strlen(parsed)-2);

        index2 = atoi(++parsed);

    }
    /*
    for (i = 0;command[i] < 9; ++i) {
        index1 = 10*index1+command[i];
    }
    if(command[i]==','){
       // index1 = index1- command[i];
        //index1 = index1/10;
        for (j = i+1; command[j]< 9; ++j) {
            index2 = 10*index2+command[j];
        }
        index2 = index2 - command[j];
        index2 = index2/10;
        operation(index1,index2,command[j]);

    }else{
        index1 = index1 - command[i];
        index1 = index1/10;
        operation(index1,index2,command[i]);
    }*/
    for (i = 0; command[i] <= maxNum ; ++i) {
        if(command[i] == ','){
            index1 = index2;
            index2 = 0;
        } else{
            index2 = index2*10 + (command[i]-offset);
        }
    }
       // index2 = index2 - (command[--i] -offset);
        decision = command[i];
        operation(index1, index2, decision);


}

void operation(int index1, int index2, char o){
    switch (o) {
        case 'p' : print(index1,index2); break;

        case 'c' : changed(index1,index2); break;

        case 'd' : delete(index1,index2); break;

//        case 'u' : undo(index2); break;

  //      case 'r' : redo(index2); break;


        case 'q' : exit(0);  break;

        default:
            break;
    }
}

void changed(int index1,int index2) {
    char point[3];  //fai una define sensata
    char row[INPUTSIZE]; // richiamo questa parte più volte?!


    //save input than if rows > used then .
   /* if(sizeLog == lastCommand){
        sizeLog += INITARRAY;
        logger = realloc(logger,sizeLog * sizeof(char*));

    }

    lengthLog += strlen(row)* sizeof(char);    //FIND OUT WHY +=
    logger[lastCommand] = malloc (lengthLog);
    strcpy(logger[lastCommand],input);
    ++lastCommand;*/


    for (int i = index1; i <= index2; ++i) {
        fgets(row,INPUTSIZE,stdin);
        if(i > size){
            size += INITARRAY;
            text = realloc(text,size * sizeof(char*));
        }


        // text = malloc(strlen(row) * sizeof(char));
        lengthText += strlen(row)* sizeof(char);    //FIND OUT WHY +=
        text[i] = malloc (lengthText);
        strcpy(text[i],row);
        //++used;

    }
    if(index2> used)
        used = index2;
    fgets(point,3,stdin);
}

void delete(int index1, int index2) {
    size_t len;
    //free
   // for (int i= index1;i<=index2; ++i){  //devo scalare gli indici?1,2
   if(index1<= used){

        if(index2>used) {  //check the condition   index validi if index <= used if (index2<= used) this else used è il max
            index2 = used;
        }
        for(int i= index1;i<=index2;++i)
            len = strlen(text[i]);   //and others

        memmove(text+index1,text+index2+1 ,len*sizeof(char));

        //char** a = realloc(text,(--size) * sizeof(char*));
        text = realloc(text,(size-(index2-index1+1)) * sizeof(char*));

        //text = a;

        //free(text[i]);
        used -= (index2-index1+1);

   }
   //printf("%d",used);
}

void print(int index1, int index2){
   if(index1>=0) {
       for (int i = index1; i <= index2; ++i) {
           //if doesnt exists (>= max numero valido )
           if(i>used || i == 0){
               fputs(".\n",stdout);
           } else{
               fputs(text[i],stdout);
           }

       }
   }

}

void undo(int nTimes){

}

void redo(int nTimes){

}







/*
 * 1undo list
 * 1changelist
 *                  DELETE/CHANGE free undo list! and modify the change list
 *
 *                  for the CHANGE list logger (ind1,ind2,command) i parsed this like an input so i have a logger and a change list (arrays)
 *                  for the UNDO list(array) better than a free of the array i can forbit the access to this and re write the memory (->free the string )
 *                  just using a int to ensure that i dont redo something not useful
 *
 *
 *
 *
 *                  COULD BE BETTER:
 *                      initialized 1025 for every rows and then after some realloc
 *
 *
 */




