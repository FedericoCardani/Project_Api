#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INITARRAY 300000
#define INPUTSIZE 1028
#define INPUTCOMANDS 50


char** text;
char** memory;
char** undoList;
char** moreUpdatedText;

int* nRowsUndo; //how many rows i have to change for undo
int* nRowsRedo;
int* usedRows;

char input[INPUTCOMANDS];
int used = 0;           //used text
int size = INITARRAY; //sizeText
int fakeZeroUsed=0;
int fakeZeroLastIndexNRows =0;
int moreUpdatedTextSize = INITARRAY;
int updatedTextInUse = 0;
int maxSize=0;



int memorySize = INITARRAY;  //for undo
int undoSize = INITARRAY;   //for redo
int lastMemory = 0;    //memory
int lastUndo = 0;     //undo
int nUndoDone = 0;



int fakeZeroStatus =0;

int lastIndexNRows = 0;
int sizeNRows = INITARRAY;

int lastIndexRedo =0;
int sizeNRowsRedo = INITARRAY;


typedef struct input{
    int index1;
    int index2;
    char op;
    struct input* next;
    struct input* prev;
}log;

/*
 * REMEMBER
 *          IN LETTURA index-1
 *          IN SCRITTURA index
 *
 */

log *log1= NULL;



int nUndo = 0;
int intIndex [2];

size_t lengthText = 0;
size_t lengthMemory = 0;

void parsing(char* command);
void changed(int index1, int index2);
void delete(int index1, int index2);
void print(int index1, int index2);
void undo(int nTimes);
void redo(int nTimes);
void operation(int index1, int index2, char o);
void incrementingUndo(int nTimes);
void decrementingUndo(int nTimes);
char parsedMem();
void saveInput(int index1,int index2,char o);




int main(){
    text = malloc(INITARRAY * sizeof(char*));
    memory = malloc(INITARRAY * sizeof(char*));
    undoList = malloc(INITARRAY * sizeof(char*));
    nRowsUndo = malloc(INITARRAY * sizeof(int));
    nRowsRedo = malloc(INITARRAY * sizeof(int));
    usedRows = malloc(INITARRAY * sizeof(int));
    moreUpdatedText = malloc(INITARRAY * sizeof(char*));



    while (1) {
        fgets(input, INPUTCOMANDS, stdin);
        parsing(input);

    }
    return 0;
}

void parsing(char* command){
    int index1=0,index2= 0,i,offset = 48,maxNum =57;
    char decision;

    for (i = 0; command[i] <= maxNum ; ++i) {
        if(command[i] == ','){
            index1 = index2;
            index2 = 0;
        } else{
            index2 = index2*10 + (command[i]-offset);
        }
    }
    decision = command[i];
    operation(index1, index2, decision);
}

void operation(int index1, int index2, char o){
    switch (o) {
        case 'p' : print(index1,index2); break;

        case 'c' : changed(index1,index2); break;

        case 'd' : delete(index1,index2); break;

        case 'u' : incrementingUndo(index2); break;

        case 'r' : decrementingUndo(index2); break;

        case 'q' : exit(0);  break;

        default:
            break;
    }
}

void changed(int index1,int index2) {
    char point[3];
    char row[INPUTSIZE];
    int sizeRows = 0;
    if(nUndo != 0 || fakeZeroStatus){
        if(nUndo > 0){
            fakeZeroStatus = 0;
            undo(nUndo);
        } else {
            if (fakeZeroStatus) {
                int nTimes = fakeZeroLastIndexNRows + nUndo;
                used = fakeZeroUsed;
                lastIndexNRows = fakeZeroLastIndexNRows;
                if (nTimes > 0) {
                    fakeZeroStatus = 0;
                    undo(nTimes);
                } else {
                    redo(nTimes);
                }
                nUndo = 0;
            }
            redo(nUndo);
        }
        nUndo=0;
    }
    nUndoDone =0;
    lastUndo =0;
    lastIndexRedo=0;
    if(lastIndexNRows ==0){
       used=0;
       log1=NULL;
    }
    saveInput(index1,index2,'c');


    for (int i = index1; i <= index2; ++i) {

        fgets(row,INPUTSIZE,stdin);
        if(i > size){
            size += INITARRAY;
            text = realloc(text,size * sizeof(char*));
        }

        if(lastMemory >= memorySize){
            memorySize += INITARRAY;
            memory = realloc(memory,memorySize * sizeof(char*));
        }
        if(lastIndexNRows == sizeNRows){
            sizeNRows += INITARRAY;
            nRowsUndo = realloc(nRowsUndo, sizeNRows * sizeof(int ));
            usedRows = realloc(usedRows, sizeNRows * sizeof(int ));
        }

        if(i <= used){
            memory[lastMemory] = text[i];
            ++sizeRows;
            ++lastMemory; //increase the "Stack" of the memory ( x UNDO)

        }

        lengthText = strlen(row)+2;
        text[i] = malloc (++lengthText);
        strcpy(text[i],row);

    }
    if(index2 > used)
        used = index2;   //last index of the dynamic array of rows

    usedRows[lastIndexNRows] = used;
    fgets(point,3,stdin);
    nRowsUndo[lastIndexNRows] = sizeRows;
    ++lastIndexNRows;
    updatedTextInUse = 0;
    maxSize=lastIndexNRows;
}

void delete(int index1, int index2) {
    int lastIndex = index2;

    if(nUndo != 0 || fakeZeroStatus){
        if(nUndo > 0){
            fakeZeroStatus = 0;
            undo(nUndo);
        } else{
            if(fakeZeroStatus){
                int nTimes = fakeZeroLastIndexNRows + nUndo;
                used = fakeZeroUsed;
                lastIndexNRows = fakeZeroLastIndexNRows;
                if(nTimes>0){
                    fakeZeroStatus =0;
                    undo(nTimes);
                } else{

                    redo(nTimes);
                }
                nUndo=0;
            }
            redo(nUndo);
        }
        nUndo=0;
    }

    nUndoDone =0;
    lastIndexRedo=0;
    if(lastIndexNRows ==0){
        log1=NULL;
        used =0;

    }
    lastUndo =0;


    saveInput(index1,index2,'d');

    if(lastIndexNRows >= sizeNRows){
        sizeNRows += INITARRAY;
        nRowsUndo = realloc(nRowsUndo, sizeNRows * sizeof(int ));
        usedRows = realloc(usedRows, sizeNRows * sizeof(int ));
    }

    if(index1 <= used && used > 0){


        if(index2 > used) {
            lastIndex = used;
        }

        if((lastMemory+used) >= memorySize){
            memorySize += INITARRAY+used;
            memory = realloc(memory,memorySize* sizeof(char *));
        }


        for (int j = index1; j <= lastIndex; ++j) {
            memory[lastMemory] = text[j];
            ++lastMemory;
        }


        for(int i= lastIndex+1; i<= used;++i){
            int offset = index1-lastIndex-1;
            text[i+offset] = text[i];
        }



        nRowsUndo[lastIndexNRows] = lastIndex - index1 + 1;
        used -= (lastIndex-index1+1);
    }

    else{
        log1->op = 'n';
        nRowsUndo[lastIndexNRows]= 0;
    }
    usedRows[lastIndexNRows] = used;
    ++lastIndexNRows;
    updatedTextInUse = 0;
    maxSize=lastIndexNRows;
}

void print(int index1, int index2){

    if(nUndo != 0){
        if (usedRows[lastIndexNRows-nUndo-1]==0){
            for (int i = index1; i <=index2 ; ++i) {
                fputs(".\n",stdout);
            }
            return;
        }
        if(nUndo > 0){
            fakeZeroStatus = 1;
            undo(nUndo);
        } else{
            if(fakeZeroStatus){
                used = fakeZeroUsed;
                lastIndexNRows = fakeZeroLastIndexNRows;
                int nTimes = fakeZeroLastIndexNRows + nUndo;
                if(nTimes>0){
                    undo(nTimes);
                } else{
                    int howManyRedo = nTimes*-1;
                    if(howManyRedo+lastIndexNRows >= maxSize){
                        for (int i = index1; i <= index2; ++i) {
                                if(i>usedRows[maxSize] || i <= 0){
                                    fputs(".\n",stdout);
                                } else{
                                    fputs(moreUpdatedText[i],stdout);
                                }
                        }
                            return;
                    } else{
                        redo(nTimes);
                    }
                }
                nUndo=0;
            }
            int howManyRedo = nUndo*-1;
            if(howManyRedo+lastIndexNRows >= maxSize) {
                for (int i = index1; i <= index2; ++i) {
                    if(i>usedRows[maxSize-1] || i <= 0){
                        fputs(".\n",stdout);
                    } else{
                        fputs(moreUpdatedText[i],stdout);
                    }
                }
                return;
            }
            else{
                redo(nUndo);
            }
        }
        nUndo=0;
    }
    for (int i = index1; i <= index2; ++i) {

        if(i>used || i <= 0){
            fputs(".\n",stdout);
        } else{
            fputs(text[i],stdout);
        }

    }
}

void undo(int nTimes){
    if(nTimes >=lastIndexNRows && fakeZeroStatus){
        fakeZeroUsed = used;
        fakeZeroLastIndexNRows = lastIndexNRows;
        lastIndexNRows = 0;
        used=0;
        return;
    } else{
        fakeZeroStatus = 0;
    }

    if(lastIndexRedo +nTimes  >= sizeNRowsRedo) {
        sizeNRowsRedo += INITARRAY;
        nRowsRedo = realloc(nRowsRedo, sizeNRowsRedo * sizeof(int));
    }

    if(log1->next == NULL && !updatedTextInUse){
        if(moreUpdatedTextSize < size){
            moreUpdatedTextSize = size;
            moreUpdatedText = realloc(moreUpdatedText,moreUpdatedTextSize * sizeof(char *));
        }
        for (int i = 1; i <= used ; ++i) {
            moreUpdatedText[i] = text[i];
        }
        updatedTextInUse = 1;
    }
    for (int i = 0; i < nTimes; ++i) {

        int redo = 0;
        if(lastUndo + used  >= undoSize) {
            undoSize += INITARRAY;
            undoList = realloc( undoList , undoSize * sizeof(char *));  //rethink
        }

        char instr = parsedMem();
        if(log1->prev != NULL){
            log1 = log1->prev;
        } else{
            if(lastIndexNRows > 1){
                nRowsUndo[lastIndexNRows - 2] = 0;
                usedRows[lastIndexNRows - 2] =0;
            }
        }

        int usable = intIndex[1]-intIndex[0]+1-nRowsUndo[lastIndexNRows - 1];


        if(lastMemory == 0 &&  (usable-used) == 0){

                for (int j = 1; j <= used; ++j) {
                    undoList[lastUndo] = text[j];

                    lastUndo++;
                }

            redo = used;
            used = 0;
            instr = 'n';
        }

        if(instr == 'c'){
            lastMemory-= nRowsUndo[lastIndexNRows -1];
            for (int j = intIndex[0]; j <= intIndex[1]; ++j) {
                undoList[lastUndo] = text[j];
                lastUndo++;
                redo++;

                if(j < nRowsUndo[lastIndexNRows -1] + intIndex[0] ){
                    text[j] = memory[lastMemory];
                    ++lastMemory;

                }

            }
            if(intIndex[1] == used){
                used -= usable;
            }
            lastMemory-= nRowsUndo[lastIndexNRows -1];

        } else if(instr == 'd'){
            lastMemory -= nRowsUndo[lastIndexNRows -1];

            for (int j = used; j>= intIndex[0]; --j) {
                text[j+nRowsUndo[lastIndexNRows-1]] = text[j];
            }
            for (int k = 0; k < nRowsUndo[lastIndexNRows -1]; ++k) {
                text[intIndex[0]+k] = memory[lastMemory];
                ++lastMemory;
            }
            used += nRowsUndo[lastIndexNRows -1];
            redo = nRowsUndo[lastIndexNRows -1];
            lastMemory-= nRowsUndo[lastIndexNRows -1];

        }
        nRowsRedo[lastIndexRedo] = redo;
        ++lastIndexRedo;
        --lastIndexNRows;
    }

}

void redo(int nTimes) {
    char command;
    for (int i = nTimes; i < 0; ++i) {

        int undo = 0;
        if (lastIndexNRows == 0) {

            command = parsedMem();
        } else {
            if(log1->next != NULL )
                log1 = log1->next;
            else{
                nRowsRedo[lastIndexRedo - 1] = 0;
            }
            command = parsedMem();
        }

        if (command == 'c') {
              lastUndo-= nRowsRedo[lastIndexRedo -1];
            for (int k = intIndex[0]; k <= intIndex[1]; ++k) {

                if (k <= used) {
                    memory[lastMemory] = text[k];

                    ++lastMemory;
                    ++undo;
                }
                text[k] =  undoList[k-intIndex[0] + lastUndo];
            }

            if (intIndex[1] > used) {
                used = intIndex[1];
            }
        } else if (command == 'd') {
            int lastIndex = intIndex[1];
            if (intIndex[1] > used) {
                lastIndex = used;
            }

            for (int k = intIndex[0]; k <= lastIndex; ++k) {
                memory[lastMemory] = text[k];
                ++lastMemory;
            }

            for (int j = lastIndex+1; j <= used; ++j) {
                int offset = intIndex[0]-lastIndex-1;
                text[j+offset]= text[j];
            }

            undo = lastIndex - intIndex[0] + 1;
            used -= lastIndex - intIndex[0] + 1;
        }
        nRowsUndo[lastIndexNRows] = undo;
        lastIndexRedo--;
        ++lastIndexNRows;
    }
    fakeZeroStatus=0;
}

void incrementingUndo(int nTimes){
    int howMany = lastIndexNRows-nUndo;
    if (howMany >0){
        if(nTimes > howMany){
            nTimes = howMany;
        }
        nUndo += nTimes;
        nUndoDone += nTimes;
    }
}

void decrementingUndo(int nTimes){
    if(nUndoDone > 0){
        if (nUndoDone - nTimes < 0 ){
            nTimes = nUndoDone;
        }
        nUndo -= nTimes;
        nUndoDone -= nTimes;
    }
}

void saveInput(int index1,int index2,char o) {

        log *newLog = (log *) malloc(sizeof(log));
        newLog->index1 = index1;
        newLog->index2 = index2;
        newLog->op = o;
        newLog->next = NULL;
        newLog->prev = log1;
        if (log1 != NULL) {
            log1->next = newLog;
        }
        log1 = newLog;

}

char parsedMem(){

    intIndex[0]= log1->index1;
    intIndex[1]= log1->index2;
    return log1->op;
}
