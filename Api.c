#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INITARRAY 300000   //maybe possible improvement
#define INITARRAYUSILIARI 50000
#define INPUTSIZE 1028
#define INPUTCOMANDS 50
#define HELPFULLUNDO 5000


/*Stack_node *firstUndo = NULL;

//contatore per le istruzioni nulle
//puntatore per le stringhe
//controllare 0,0d o 0,0c


// OKAY
//size_t arrsize = number;


//change the size -> realloc(text, arrSize * sizeof(int))

//initialized array or malloc(numberEl* sizeof()int);*/


char** text;
char** memory;
char** undoList;
char** ausiliarText;
char** ausiliarMemory;
char** ausiliarUndoList;

int* nRowsUndo; //how many rows i have to change for undo
int* nRowsRedo;
int* ausiliarNRowsUndo; //how many rows i have to change for undo
int* ausiliarNRowsRedo;
int* usedRows;

char input[INPUTCOMANDS];
int used = 0;           //used text
int size = INITARRAY; //sizeText
int fakeZeroUsed=0;
int fakeZeroLastIndexNRows =0;
int ausiliarTextSize = INITARRAY;
int ausiliarMemorySize = INITARRAY;
int ausiliarUndoSize;
int ausiliarLastMemory;
int ausiliarUsed;
int ausiliarLastIndexNRows = HELPFULLUNDO;
int ausiliarLastUndo;
int ausiliarLastIndexRedo;
int dontLostInf =0;



int memorySize = INITARRAY;  //for undo
int undoSize = INITARRAY;   //for redo
int lastMemory = 0;    //memory
int lastUndo = 0;     //undo
int nUndoDone = 0;
int alreadyInUse =0;
int firstBlanck =0;
int onlyDelete =1;

//int CHECK =0;
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

//ogni 5k

/*
 * REMEMBER
 *          IN LETTURA index-1
 *          IN SCRITTURA index
 *
 */

log *log1= NULL;
log *ausiliarLog1 = NULL;



//int debug = 0;    // JUST FOR DEBUG

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
void savingInput();
void incrementingUndo(int nTimes);
void decrementingUndo(int nTimes);
char parsedMem();
void saveInput(int index1,int index2,char o);
int parsedInput();
/*void saveHelpFullUndo();
void savePrevStatus();
void changeToMiddle();
void changeToAusiliar();*/



int main(){
    text = malloc(INITARRAY * sizeof(char*));
    memory = malloc(INITARRAY * sizeof(char*));
    undoList = malloc(INITARRAY * sizeof(char*));
    nRowsUndo = malloc(INITARRAY * sizeof(int));
    nRowsRedo = malloc(INITARRAY * sizeof(int));
    usedRows = malloc(INITARRAY * sizeof(int));
   /* ausiliarText = malloc(INITARRAY * sizeof(char*));
    ausiliarMemory = malloc(INITARRAY * sizeof(char*));
    ausiliarNRowsRedo = malloc(INITARRAY * sizeof(int));*/


    while (1) {
        fgets(input, INPUTCOMANDS, stdin);
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
    char point[3];  //fai una define sensata
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
   /* for (int j = 0; j < lastUndo; ++j) {
        free(undoList[j]);
    }*/
    lastUndo =0;
    lastIndexRedo=0;
    if(lastIndexNRows ==0){
       // free(log1);
       // log1 =  malloc(sizeof(log));
       /* for (int i = 0; i < maxCommand; ++i) {
            free logger

        }*/
       used=0;
       log1=NULL;
       /* for (int i = 0; i < lastCommand; ++i) {
            log *temp = (log*)  malloc(sizeof(log));
            temp->next = NULL;
            temp->prev = log1;
            temp->index1 = 1;
            temp->index2 = 1;
            temp->op = 'n';
            log1 = temp;
        }*/
    }
    saveInput(index1,index2,'c');


    for (int i = index1; i <= index2; ++i) {

        fgets(row,INPUTSIZE,stdin);
        if(i > size){     //posso farlo all inizio e non ogni volta
            size += INITARRAY;
            text = realloc(text,size * sizeof(char*));
        }

        if(lastMemory >= memorySize){          //posso farlo all inizio e non ogni volta
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
           // lengthMemory = strlen(text[i])+2;
            //memory[lastMemory]= malloc(++lengthMemory);   //save the exact row in memory
           // strcpy(memory[lastMemory],text[i]);
            ++sizeRows;
            ++lastMemory; //increase the Stack of the memory ( x UNDO)
           // free(text[i]);
        }
        // text = malloc(strlen(row) * sizeof(char));
        lengthText = strlen(row)+2;   // lenght of the row saved
        text[i] = malloc (++lengthText);
        strcpy(text[i],row);

    }
    if(index2 > used)
        used = index2;   //last index of the dynamic array of rows

    usedRows[lastIndexNRows] = used;
    fgets(point,3,stdin);
    //devo aumentare nrows index
    nRowsUndo[lastIndexNRows] = sizeRows;
    ++lastIndexNRows;
   /* if(lastIndexNRows == HELPFULLUNDO && !alreadyInUse){
        saveHelpFullUndo();
    }*/
}

void delete(int index1, int index2) {
    size_t len;
    char temp[INPUTSIZE];
    int lastIndex = index2;     //still not get if useful or not
    //free the UNDO STACK cant write over because of the size

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
        /*free(log1);
         log1 =  malloc(sizeof(log));

        /* for (int i = 0; i < lastCommand; ++i) {
             log *temp = (log*)  malloc(sizeof(log));
             temp->next = NULL;
             temp->prev = log1;
             temp->index1 = 1;
             temp->index2 = 1;
             temp->op = 'n';
             log1 = temp;
         }*/
    }
    /*for (int j = 0; j < lastUndo; ++j) {
        free(undoList[j]);
    }*/
    lastUndo =0;
    //save something in case of delete unused
    //NOP how to write it smart usage!!!

    //savingInput();
    saveInput(index1,index2,'d');

    //printf("%d",used);
    if(lastIndexNRows >= sizeNRows){
        sizeNRows += INITARRAY;
        nRowsUndo = realloc(nRowsUndo, sizeNRows * sizeof(int ));
        usedRows = realloc(usedRows, sizeNRows * sizeof(int ));
    }

    if(index1 <= used && used > 0){

        /*
         * if(index1<=0){
             index1 = 1;   //check if useful later  POSSO TOGLIERLO
         }
         */

        if(index2 > used) {  //check the condition   index validi if index <= used if (index2<= used) this else used è il max
            lastIndex = used;
        }

        if((lastMemory+used) >= memorySize){          //posso farlo all inizio e non ogni volta
            memorySize += INITARRAY+used;
            memory = realloc(memory,memorySize* sizeof(char *));
        }

        //da index1 a lastindex salvo in mem
        for (int j = index1; j <= lastIndex; ++j) {
            memory[lastMemory] = text[j];
            //lengthMemory = strlen(text[j])+2;
            //memory[lastMemory] = malloc(++lengthMemory);
           // strcpy(memory[lastMemory],text[j]);
            ++lastMemory;
          //  free(text[j]);
        }


        for(int i= lastIndex+1; i<= used;++i){    //index2 -> used
            int offset = index1-lastIndex-1;
            text[i+offset] = text[i];
          //  len = strlen(text[i])+2; //      MAYBE HERE OPTIMIZE
          //  text[i+offset]= malloc(++len);
          //  strcpy(text[i+offset],text[i]);
        }

        //shiftare lo fa la memmove
     //   memcpy(text+index1,text+lastIndex+1 ,len);//*sizeof(char));
        //char** a = realloc(text,(--size) * sizeof(char*));


        nRowsUndo[lastIndexNRows] = lastIndex - index1 + 1;
        //printf("%d",nRowsUndo[lastIndexNRows]);
        used -= (lastIndex-index1+1);
    }

    else{
        log1->op = 'n';
        nRowsUndo[lastIndexNRows]= 0;
    }
    usedRows[lastIndexNRows] = used;
    ++lastIndexNRows;
   /* if(lastIndexNRows == HELPFULLUNDO && !alreadyInUse){
        saveHelpFullUndo();
    }*/
}

void print(int index1, int index2){
    int index;

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
                    redo(nTimes);
                }
                nUndo=0;
            }
            redo(nUndo);
        }
        nUndo=0;
    }
    for (int i = index1; i <= index2; ++i) {
        //if doesnt exists (>= max numero valido )
        if(i>used || i <= 0){
            fputs(".\n",stdout);
        } else{
            fputs(text[i],stdout);
        }
        //printf("HERE");
        //}
    }
   /* if(lastIndexNRows ==20)
         for (int j = 0; j < 20; ++j) {
            printf("\n%d ",usedRows[j]);
         }
    /*  if(debug){
        printf("FINE ,%d\n",lastIndexNRows);
   /* }*/
    /*for (int m = 0; m < lastMemory; ++m) {
        printf("%s\n",memory[m]);
    }
   */
}

void undo(int nTimes){
    size_t length,len_t;
    /*if(lastIndexNRows >= 10000)     //basta salvare 5k
        exit(1);
    if(nTimes> 20000){
        exit(1);
    }*/
    /*  if((nTimes >=lastIndexNRows || usedRows[lastIndexNRows-nTimes]==0/*|| lastIndexNRows-nTimes <= firstBlanck ) && fakeZeroStatus){
        fakeZeroUsed = used;
        fakeZeroLastIndexNRows = lastIndexNRows;
        lastIndexNRows = 0;
        used = 0;
        return;
    } else{
        fakeZeroStatus = 0;
    }*/
    /*if(lastIndexNRows-nTimes <= ausiliarLastIndexNRows && lastIndexNRows > (ausiliarLastIndexNRows+100)){
        if(!dontLostInf){
            savePrevStatus();  //first time salvo e sostituisco  //poi solo sostituisco
            nTimes -= ausiliarLastIndexNRows;
        }

    }*/

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
      //  ausiliarNRowsRedo = realloc(ausiliarNRowsRedo, sizeNRowsRedo * sizeof(int));
    }
   /* if(lastIndexNRows > fakeZeroLastIndexNRows){

        if(fakeZeroLastIndexNRows>0){
            free(ausiliarMemory[0]);
            for (int i = 1; i <= fakeZeroUsed ; ++i) {
                free(ausiliarText[i]);
                if(i < ausiliarLastMemory){
                    free(ausiliarMemory[i]);
                }
            }
            for (int j = fakeZeroUsed+1; j < ausiliarLastMemory; ++j) {
                free(ausiliarMemory[j]);
            }
        }
        //free di sta robba
        if(size > ausiliarTextSize){
            ausiliarTextSize += size;
            ausiliarText = realloc(ausiliarText,ausiliarTextSize * sizeof(char *));
        }
        if(memorySize > ausiliarMemorySize){
            ausiliarMemorySize += memorySize;
            ausiliarMemory = realloc(ausiliarMemory, ausiliarMemorySize * sizeof(char *));
        }
        for (int i = 1; i <= used; ++i) {
            length = strlen(text[i])+2;
            ausiliarText[i] = malloc(++length);
            strcpy(ausiliarText[i],text[i]);
        }
        fakeZeroUsed=used;
        for (int j = 0; j < lastMemory; ++j) {
            length = strlen(memory[j])+2;
            ausiliarMemory[j]= malloc(++length);
            strcpy(ausiliarMemory[j],memory[j]);
        }
        ausiliarLastMemory = lastMemory;
        fakeZeroLastIndexNRows = lastIndexNRows;

        //lastIndexlo salvo?
    }*/
    for (int i = 0; i < nTimes; ++i) {

        int redo = 0;
        /*leggo istruzione
        //decremento instr
        //leggo quanti cambiamenti son stati messi in mem
        //eseguo c o d al rovescio
        // c al rovescio
        //  alle righe index 1 fino a index1 + quanti al rovescio
        //  se righe occupate < used salvo quelle righe in undo list e redo x per quanti
        //  rimetto da mem a text quelle righe
        // posso usare memmove
        // used diminuisco se index2 è == used

        // d al rovescio traslo index1 in index1 + quanti
        // poi inserisco da index1
        // metto 0 in redo
        //used += quanti

        //decremento quanti
        // incremento quante redo
        incremento index x redo */
        if(lastUndo + used  >= undoSize) {
            undoSize += INITARRAY;
            undoList = realloc( undoList , undoSize * sizeof(char *));  //rethink
        }
        /*for (int m = 0; m < lastUndo; ++m) {
            printf("%s\n",undoList[m]);
        }
        printf("%d, %s\n",lastCommand,logger[2]);
        for (int l = 0; l < lastCommand; ++l) {
            printf(" com:   %s\n",logger[l]);
        }
        log *temp = log1;
         while (temp!= NULL){
            printf("%d %d %c\n",temp->index1,temp->index2,temp->op);
            temp = temp->next;
        }*/

        char instr = parsedMem();
        if(log1->prev != NULL){
            log1 = log1->prev;
        } else{
            if(lastIndexNRows > 1){
                nRowsUndo[lastIndexNRows - 2] = 0;
                usedRows[lastIndexNRows - 2] =0;
            }
        }
       /* if (debug)
            printf("%d , %d %c %d\n",intIndex[0],intIndex[1],instr,lastIndexNRows);
            */

        int usable = intIndex[1]-intIndex[0]+1-nRowsUndo[lastIndexNRows - 1];

        //printf("\nQUI\n");
        if(lastMemory == 0 &&  (usable-used) == 0){
            //if(fakeZeroStatus > 0) {
                for (int j = 1; j <= used; ++j) {
                    undoList[lastUndo] = text[j];
                   // length = strlen(text[j]) + 2;
                   // undoList[lastUndo] = malloc(++length);
                   // strcpy(undoList[lastUndo], text[j]);
                    lastUndo++;
                }
           // }
            redo = used;
            used = 0;
            /*
             lastCommand = lastIndexNRows-1;
             log *temp1 = log1;
             //guardi quanto manca e cicli di conseguenza tanto tutte le prossime istruzioni sono nulle
             //lastIndexNRows--;

             //lastIndexRedo++;
            // while(i< nTimes){
               //  if(temp1->prev != NULL){
                 //    temp1 = temp1->prev;
                 //}
                 lastIndexRedo++;
                 nRowsRedo[lastIndexRedo] = 0;
               //  lastIndexNRows--;
                 ++i;
             }

             log1 = temp1;*/
            /*++i;
            while (i< nTimes){
                nRowsRedo[lastIndexRedo] = 0;
                ++lastIndexRedo;
                --lastIndexNRows;
                i++;
            }*/
            instr = 'n';
        }

        if(instr == 'c'){
            lastMemory-= nRowsUndo[lastIndexNRows -1];
            for (int j = intIndex[0]; j <= intIndex[1]; ++j) {
                undoList[lastUndo] = text[j];
                //if(fakeZeroStatus >0){
                   // length = strlen(text[j])+2;
                   // undoList[lastUndo] = malloc(++length);
                   // strcpy(undoList[lastUndo],text[j]);
                    lastUndo++;
                //}

                redo++;
                //free(text[j]);

                if(j < nRowsUndo[lastIndexNRows -1] + intIndex[0] ){
                    // if(lastMemory > -1) {    ITS NOT THE PROBLEM
                    text[j] = memory[lastMemory];
                    //len_t = strlen(memory[lastMemory]) + 2;
                    //text[j] = malloc(++len_t);
                    //strcpy(text[j], memory[lastMemory]);
                    //free(memory[lastMemory]);
                    ++lastMemory;

                    //   }
                }

            }
            /*   for (int k = intIndex[1]+1; k <= used; ++k) {
                   length = strlen(text[k])+2;
                   text[k-1]= malloc(++length);
                   strcpy(text[k-1],text[k]);
               }*/
            /* if(lastMemory != 0){
             memcpy(temp,text+intIndex[1]+1,len);
             memcpy(text + intIndex[0],memory +lastMemory,len_t);
             memcpy(text+intIndex[1]+1,temp,len);


            }*/
            /*else{
               for (int j = 0; j < nRowsUndo[lastIndexNRows -1]; ++j) {
                   length = strlen(memory[j])+2;
                   //free(text[j+intIndex[0]]);
                   text[j+intIndex[0]]= malloc(++length);
                   strcpy(text[j+intIndex[0]],memory[j]);
               }
           }*/
            /*for (int l = 0; l < 1; l++) {
                printf("2 -- %d : %s\n",l,logger[l]);
            }

            printf("Mem : %s,%d\n ",memory[lastMemory],lastMemory);
            if(lastIndexNRows == 1 || lastIndexNRows == 2 ){
                for (int m = 1; m <= 2 ; ++m) {
                    fputs(text[m],stdout);
                }
            for (int n = 0; n < 2; ++n) {
                fputs(memory[n],stdout);
            }
            }
            printf("%s, %s %ld",text[intIndex[0]],memory[lastMemory],len_t);*/
            /*   if(lastIndexNRows == 1 || lastIndexNRows == 2 ){
                   for (int n = 0; n < 2; ++n) {
                       fputs(memory[n],stdout);
                   }
               }

               for (int l = 0; l < 1; l++) {
                   printf("2 -- %d : %s\n",l,logger[l]);
               }*/
            if(intIndex[1] == used){
                used -= usable;
            }
            lastMemory-= nRowsUndo[lastIndexNRows -1];

        } else if(instr == 'd'){
            lastMemory -= nRowsUndo[lastIndexNRows -1];

            for (int j = used; j>= intIndex[0]; --j) {
                text[j+nRowsUndo[lastIndexNRows-1]] = text[j];
               // length = strlen(text[j])+2;
                /*text[j+nRowsUndo[lastIndexNRows-1]] = malloc(++length);
                // temp[j-intIndex[0]] = malloc(++length);
                // strcpy(temp[j+nRowsUndo[lastIndexNRows-1]],text[j]);
                // strcpy(text[j+nRowsUndo[lastIndexNRows-1]],text[j]);
                //se used  = index1 no probl
                //else
                // text[used -j-int[0]] = used+....+offset
                // int offset = nRowsUndo[lastIndexNRows-1];*/
               // text[j+nRowsUndo[lastIndexNRows-1]] = malloc(++length);
               // strcpy(text[j+nRowsUndo[lastIndexNRows-1]],text[j]);
            }
            //    memmove(temp,text+intIndex[0],length);
            for (int k = 0; k < nRowsUndo[lastIndexNRows -1]; ++k) {
                text[intIndex[0]+k] = memory[lastMemory];
                //len_t = strlen(memory[lastMemory])+2;
                //text[intIndex[0]+k] = malloc(++len_t);
                //strcpy(text[intIndex[0]+k],memory[lastMemory]);
               // free(memory[lastMemory]);
                ++lastMemory;
            }
            /*for (int l = intIndex[0]; l <=used ; ++l) {
                length = strlen(temp[l-intIndex[0]])+2;
                text[l+nRowsUndo[lastIndexNRows-1]] = malloc(++length);
                strcpy(text[l+nRowsUndo[lastIndexNRows-1]],temp[l-intIndex[0]]);
               // free(temp[l-intIndex[0]]);
            }*/
            //   memmove(text+intIndex[0],memory+ lastMemory,len_t);

            //   memmove(text+intIndex[0]+nRowsUndo[lastIndexNRows -1],temp,length);
            used += nRowsUndo[lastIndexNRows -1];
            redo = nRowsUndo[lastIndexNRows -1];
            lastMemory-= nRowsUndo[lastIndexNRows -1];

        }/*else if(instr =='n'){
            //DO NOTHING
        }*/
        nRowsRedo[lastIndexRedo] = redo;
        ++lastIndexRedo;
        --lastIndexNRows;
    }

}

void redo(int nTimes) {
    size_t len, length;
    char command;
    /*for (int m = 0; m < lastMemory; ++m) {
        printf("%s\n",memory[m]);
    }
    for (int l = 0; l < lastCommand; ++l) {
        printf(" com:   %s\n",logger[l]);
    }*/
    // printf("SSS %d",lastIndexRedo);
    /*for (int l = 0; l < lastUndo; ++l) {
        printf("UN: %s",undoList[l]);
    }*/
   /* if(lastIndexNRows-nTimes >= HELPFULLUNDO){
        //scambio di nuovo
        int prevLastIndex = lastIndexNRows;
        changeToAusiliar();  //lì ho le info per fare undo/redo
        nTimes = prevLastIndex-nTimes-lastIndexNRows;
        nTimes *= -1;
        if(nTimes>0){
            undo(nTimes);
            return;
        }
    }*/
    for (int i = nTimes; i < 0; ++i) {
        /*
         * aumenting index last command and lastnindexrow
         *
         * if c
                leggo quanto da sostituire nrows [index]

                se gia qualcosa a quegli index metto nella mem

                leggo da undolist cosa da sostituire

                memmove
         */
        /* for (int l = 0; l < 20; ++l) {
                    printf("%s\n",logger[l]);
                }*/
        // printf("%d %d %d %d\n",lastIndexRedo,lastUndo,lastIndexNRows,lastCommand);
        // printf("%d ",nRowsRedo[lastIndexRedo-1]);
        /*  if((lastMemory+used) >= memorySize){          //posso farlo all inizio e non ogni volta
              memorySize += INITARRAY+used;
              memory = realloc(memory,memorySize * sizeof(char*));
          }
          if(lastIndexNRows >= sizeNRows){
              sizeNRows += INITARRAY;
              nRowsUndo = realloc(nRowsUndo, sizeNRows * sizeof(int ));
          }*/
        int undo = 0;
        if (lastIndexNRows == 0) {
            /* log *temp = log1;
             while (temp->prev != NULL){
                 temp = temp->prev;
             }
             log1 = temp;*/
            command = parsedMem();
        } else {
            if(log1->next != NULL )
                log1 = log1->next;
            else{
                nRowsRedo[lastIndexRedo - 1] = 0;
            }
            command = parsedMem();
        }
        /*if(intIndex[1] > size){     //posso farlo all inizio e non ogni volta
            size *= INITARRAY;
            text = realloc(text,size * sizeof(char*));
        }*/
    /*    if (debug)
            printf("\n%d %d %d %c\n",i,intIndex[0],intIndex[1],command);*/

        // char command = 'n';
        if (command == 'c') {
              lastUndo-= nRowsRedo[lastIndexRedo -1];
            for (int k = intIndex[0]; k <= intIndex[1]; ++k) {

                if (k <= used) {
                    memory[lastMemory] = text[k];

                    //lengthMemory = strlen(text[k]) + 2;
                    //memory[lastMemory] = malloc(++lengthMemory);   //save the exact row in memory
                    //strcpy(memory[lastMemory], text[k]);
                    ++lastMemory; //increase the Stack of the memory ( x UNDO)
                    ++undo;
                    //free(text[k]);
                }
                text[k] =  undoList[k-intIndex[0] + lastUndo];
                //length = strlen(undoList[k-intIndex[0] + lastUndo]) + 2;
                //text[k] = malloc(++length);
                //strcpy(text[k], undoList[k-intIndex[0] + lastUndo]);
                //free(undoList[k-intIndex[0] + lastUndo]);

                // free(text[k]);
             //   --lastUndo;
                //      len_t += strlen(undoList[lastUndo]) + 2;
            }
            //  printf("%d",lastUndo);
            /*  for (int k = intIndex[1] + 1; k <= used; ++k) {
                  len += strlen(text[k]) + 2;
              }*/

            /*if(lastUndo!=0){
                memcpy(temp1,text+intIndex[1]+1,len);
                memcpy(text + intIndex[0],undoList +lastUndo,len_t);
                memcpy(text+intIndex[1]+1,temp1,len);
            } else{*/    //UNIRE I CICLIIII!!!!
          /*  for (int j = 0; j < nRowsRedo[lastIndexRedo - 1]; ++j) {
                length = strlen(undoList[j + lastUndo]) + 2;
                //free(text[j+intIndex[0]]);
                text[j + intIndex[0]] = malloc(++length);
                strcpy(text[j + intIndex[0]], undoList[j + lastUndo]);
                free(undoList[j+lastUndo]);
                //  lastUndo++;
            }*/
            //}

            if (intIndex[1] > used) {
                used = intIndex[1];
            }
            //  lastUndo-= intIndex[1]-intIndex[0]+1;
            // undo = nRowsRedo[lastIndexRedo - 1];
        } else if (command == 'd') {
            int lastIndex = intIndex[1];
            if (intIndex[1] > used) {
                lastIndex = used;
            }

            for (int k = intIndex[0]; k <= lastIndex; ++k) {
                memory[lastMemory] = text[k];
               // lengthMemory = (strlen(text[k])) + 2;
               // memory[lastMemory] = malloc(++lengthMemory);
                //strcpy(memory[lastMemory], text[k]);
                ++lastMemory;
                //free(text[k]);
            }

            for (int j = lastIndex+1; j <= used; ++j) {
                int offset = intIndex[0]-lastIndex-1;
                text[j+offset]= text[j];
               //  len = strlen(text[j]) + 2;
                //text[j+ offset]= malloc(++len);
                //strcpy(text[j+ offset],text[j]);
            }
            // memcpy(text + intIndex[0], text + lastIndex + 1, len);

            undo = lastIndex - intIndex[0] + 1;
            used -= lastIndex - intIndex[0] + 1;
        }
        nRowsUndo[lastIndexNRows] = undo;
        lastIndexRedo--;
        ++lastIndexNRows;
    }
    fakeZeroStatus=0;
}

void savingInput(){
    /*if(0){
        if(lastCommand >= sizeLog){
            sizeLog += INITARRAY;
            logger = realloc(logger,sizeLog * sizeof(char*));
            printf("QUI!!!!");
        }

        /*if(lastCommand< maxCommand){
            for (int i = lastCommand; i < maxCommand; ++i) {
                free(logger[i]);
            }
            maxCommand=lastCommand;
        }
        lengthLog = inputLength+2;
        logger[lastCommand] = malloc (++lengthLog);
        strcpy(logger[lastCommand],input);
        ++lastCommand;
        ++maxCommand;
    } else{


    }*/

}

void incrementingUndo(int nTimes){
    int howMany = lastIndexNRows-nUndo;
    if (howMany >0){
        if(nTimes > howMany){
            nTimes = howMany;
        }
       // CHECK +=nTimes;
        nUndo += nTimes;
        nUndoDone += nTimes;
      //  printf(" HowManyUN %d\n",nTimes);
    }
    /*for (int i = 0; i < nTimes && lastIndexNRows-nUndo >0; ++i) {
        nUndo++;
        if(debug)
            printf("HowManyUN\n");
        nUndoDone++;
    }*/
}

void decrementingUndo(int nTimes){
    if(nUndoDone > 0){
        if (nUndoDone - nTimes < 0 ){
            nTimes = nUndoDone;
        }
       // CHECK +=nTimes;
        nUndo -= nTimes;
        nUndoDone -= nTimes;
        //printf(" HowManyRE %d\n",nTimes);
    }
    /*for (int i=0;nUndoDone > 0 && i< nTimes;i++){
        nUndo--;
        if (debug)
            printf(" HowManyRE\n");
        nUndoDone--;
    }*/
}

void saveInput(int index1,int index2,char o) {

    /*if (0) {*/
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
    /*} else{ */
   /* if(lastCommand >= logSize){
        logSize += 4*INITARRAY;
        logger = realloc(logger,logSize * sizeof(log));
    }
  //  logger[lastCommand] = malloc(sizeof(log));
    logger[lastCommand].index1 = index1;
    logger[lastCommand].index2 = index2;
    logger[lastCommand].op = o;
    ++lastCommand;
    ++maxCommand;*/

}

char parsedMem(){
    //logger[lastCommand]
    /*if(lastCommand > 0){
        int index1=0,index2= 0,i,offset = 48,maxNum =57;

        for (i = 0; logger[lastCommand-1][i] <= maxNum ; ++i) {
            if (logger[lastCommand - 1][i] == ',') {
                index1 = index2;
                index2 = 0;
            } else {
                index2 = index2 * 10 + (logger[lastCommand - 1][i] - offset);
            }
        }
        intIndex[0]= index1;
        intIndex[1]= index2;
        char c = logger[lastCommand - 1][i];
        return c;
    }
    return 'n';  //n->nop*/
   // if (log1!= NULL){
        intIndex[0]= log1->index1;
        intIndex[1]= log1->index2;
        return log1->op;
    //}
    //return 'n';


}

int parsedInput(){
    /*  int c = logger[--lastCommand];
      intIndex[1]= logger[--lastCommand];
      intIndex[0]= logger[--lastCommand];
      return c;*/
}

void saveHelpFullUndo(){

        ausiliarNRowsUndo = malloc(2*HELPFULLUNDO*sizeof(int));
     //   ausiliarNRowsRedo = malloc(sizeNRowsRedo*sizeof(int));
        if (size > ausiliarTextSize){
            ausiliarTextSize = size;
            ausiliarText = realloc(ausiliarText,ausiliarTextSize* sizeof(char *));
        }
        if (memorySize > ausiliarMemorySize ){
            ausiliarMemorySize = memorySize;
            ausiliarMemory = realloc(ausiliarMemory,ausiliarMemorySize* sizeof(char *));
        }
      /*  if (undoSize > ausiliarUndoSize){
            ausiliarUndoSize = undoSize;
            ausiliarUndoList = realloc(ausiliarUndoList, ausiliarUndoSize* sizeof(char *));
        }*/
        for (int i = 1; i <= used; ++i) {
            lengthText = strlen(text[i])+2;
            ausiliarText[i] = malloc(++lengthText);
            strcpy(ausiliarText[i],text[i]);
        }
        for (int j = 0; j < lastMemory; ++j) {
            lengthMemory = strlen(memory[j])+2;
            ausiliarMemory[j] = malloc(++lengthMemory);
            strcpy(ausiliarMemory[j],memory[j]);
        }
      /*  for (int k = 0; k < lastUndo; ++k) {
            lengthMemory = strlen(undoList[k])+2;
            ausiliarUndoList[k] = malloc(++lengthMemory);
            strcpy(ausiliarUndoList[k],undoList[k]);
        }*/
        for (int l = 0; l < lastIndexNRows; ++l) {
            ausiliarNRowsUndo[l] = nRowsUndo[l];
        }
        /*for (int i = 0; i < lastIndexRedo; ++i) {
            ausiliarNRowsRedo[i] = nRowsRedo[i];
        }*/
     //   ausiliarLastUndo = lastUndo;
        ausiliarLastMemory = lastMemory;
        ausiliarLog1 = log1;
        ausiliarUsed = used;
        alreadyInUse = 1;
}

void savePrevStatus(){
    char temp[INPUTSIZE];
    int temp1;
    log *temp2;
    if (size> ausiliarTextSize){
        ausiliarTextSize = size;
        ausiliarText = realloc(ausiliarText,ausiliarTextSize* sizeof(char *));
    }
    if (memorySize > ausiliarMemorySize ){
        ausiliarMemorySize = memorySize;
        ausiliarMemory = realloc(ausiliarMemory,ausiliarMemorySize* sizeof(char *));
    }
    if(lastIndexNRows > HELPFULLUNDO){
        ausiliarLastIndexNRows += lastIndexNRows;
        ausiliarNRowsUndo = realloc(ausiliarNRowsUndo,ausiliarLastIndexNRows* sizeof(int ));
    }
    for (int i = 1; i <= used; ++i) {   //se used =/= ausiliarused?

        lengthText = strlen(text[i])+2;
        if(i <= ausiliarUsed){
            lengthMemory = strlen(ausiliarText[i])+2;
            strcpy(temp,ausiliarText[i]);
            free(ausiliarText[i]);
        }
        ausiliarText[i] = malloc(++lengthText);
        strcpy(ausiliarText[i],text[i]);

        free(text[i]);
        if(i <= ausiliarUsed){
            text[i]= malloc(++lengthMemory);
            strcpy(text[i],temp);
        }
    }
    for (int l = used+1; l <= ausiliarUsed; ++l) {
        lengthMemory = strlen(ausiliarText[l])+2;
        text[l] = malloc(++lengthMemory);
        strcpy(text[l],ausiliarText[l]);
        free(ausiliarText[l]);
    }
    for (int j = 0; j < lastMemory; ++j) {   //se lastmem =/= ausiliarlastmem?
        lengthMemory = strlen(memory[j])+2;
        if(j< ausiliarLastMemory){
            lengthText = strlen(ausiliarMemory[j])+2;
            strcpy(temp, ausiliarMemory[j]);
            free(ausiliarMemory[j]);
        }
        ausiliarMemory[j] = malloc(++lengthMemory);
        strcpy(ausiliarMemory[j],memory[j]);

        free(memory[j]);
        if(j< ausiliarLastMemory){
            memory[j] = malloc(++lengthText);
            strcpy(memory[j],temp);
        }

    }
    for (int m = lastMemory; m < ausiliarLastMemory; ++m) {
        lengthMemory = strlen(ausiliarMemory[m])+2;
        memory[m] = malloc(++lengthMemory);
        strcpy(memory[m],ausiliarMemory[m]);
        free(ausiliarMemory[m]);
    }
    ausiliarUndoSize = 10* lastUndo;
    ausiliarUndoList = malloc(ausiliarUndoSize* sizeof(char*));
    for (int k = 0; k < lastUndo; ++k) {
        lengthText = strlen(undoList[k])+2;
        ausiliarUndoList[k] = malloc(++lengthText);
        strcpy(ausiliarUndoList[k],undoList[k]);
    }
    ausiliarLastUndo = lastUndo;
    lastUndo =0;
    ausiliarLastIndexRedo = lastIndexRedo;
    lastIndexRedo =0;

    temp1 = ausiliarLastMemory;
    ausiliarLastMemory = lastMemory;
    lastMemory = temp1;

    temp1 = ausiliarLastIndexNRows;
    ausiliarLastIndexNRows = lastIndexNRows;
    lastIndexNRows = temp1;

    temp2 = ausiliarLog1;
    ausiliarLog1 = log1;
    log1 = temp2;

    dontLostInf =1;
    //ora ho

}

void changeToMiddle(){
    for (int i = 1; i <= ausiliarUsed; ++i) {
        if(i<=used){
            free(text[i]);
        }
        lengthText = strlen(ausiliarText[i])+2;
        text[i]= malloc(++lengthText);
        strcpy(text[i],ausiliarText[i]);
        free(ausiliarText[i]);
    }
    for (int j = 0; j < ausiliarLastMemory; ++j) {
        if(j<lastMemory){
            free(memory[j]);
        }
        lengthText = strlen(ausiliarMemory[j])+2;
        memory[j]= malloc(++lengthText);
        strcpy(memory[j],ausiliarMemory[j]);
        free(ausiliarMemory[j]);
    }
    for (int k = ausiliarLastMemory; k < lastMemory; ++k) {
        free(memory[k]);
    }
    for (int l = 0; l < lastUndo; ++l) {
        free(undoList[l]);
    }
    lastUndo =0;
    lastIndexNRows = ausiliarLastIndexNRows;
    lastIndexRedo = ausiliarLastIndexRedo;
    lastMemory = ausiliarLastMemory;
    used = ausiliarUsed;
    log1 = ausiliarLog1;
}

void changeToAusiliar(){
    char temp[INPUTSIZE];
    int temp1;
    log *temp2;
    if (size> ausiliarTextSize){
        ausiliarTextSize = size;
        ausiliarText = realloc(ausiliarText,ausiliarTextSize* sizeof(char *));
    }
    if (memorySize > ausiliarMemorySize ){
        ausiliarMemorySize = memorySize;
        ausiliarMemory = realloc(ausiliarMemory,ausiliarMemorySize* sizeof(char *));
    }
    if(lastIndexNRows > HELPFULLUNDO){
        ausiliarLastIndexNRows += lastIndexNRows;
        ausiliarNRowsUndo = realloc(ausiliarNRowsUndo,ausiliarLastIndexNRows* sizeof(int ));
    }
    if(lastUndo > ausiliarUndoSize){
        ausiliarUndoSize += lastUndo;
        ausiliarUndoList = realloc(ausiliarUndoList, ausiliarUndoSize* sizeof(char *));
    }

    for (int i = 1; i <= used; ++i) {   //se used =/= ausiliarused?

        lengthText = strlen(text[i])+2;
        if(i <= ausiliarUsed){
            lengthMemory = strlen(ausiliarText[i])+2;
            strcpy(temp,ausiliarText[i]);
            free(ausiliarText[i]);
        }
        ausiliarText[i] = malloc(++lengthText);
        strcpy(ausiliarText[i],text[i]);

        free(text[i]);
        if(i <= ausiliarUsed){
            text[i]= malloc(++lengthMemory);
            strcpy(text[i],temp);
        }
    }
    for (int l = used+1; l <= ausiliarUsed; ++l) {
        lengthMemory = strlen(ausiliarText[l])+2;
        text[l] = malloc(++lengthMemory);
        strcpy(text[l],ausiliarText[l]);
        free(ausiliarText[l]);
    }
    for (int j = 0; j < lastMemory; ++j) {   //se lastmem =/= ausiliarlastmem?
        lengthMemory = strlen(memory[j])+2;
        if(j< ausiliarLastMemory){
            lengthText = strlen(ausiliarMemory[j])+2;
            strcpy(temp, ausiliarMemory[j]);
            free(ausiliarMemory[j]);
        }
        ausiliarMemory[j] = malloc(++lengthMemory);
        strcpy(ausiliarMemory[j],memory[j]);

        free(memory[j]);
        if(j< ausiliarLastMemory){
            memory[j] = malloc(++lengthText);
            strcpy(memory[j],temp);
        }

    }
    for (int m = lastMemory; m < ausiliarLastMemory; ++m) {
        lengthMemory = strlen(ausiliarMemory[m])+2;
        memory[m] = malloc(++lengthMemory);
        strcpy(memory[m],ausiliarMemory[m]);
        free(ausiliarMemory[m]);
    }
   // ausiliarUndoList = malloc(10*lastUndo * sizeof(char*));
    for (int k = 0; k < lastUndo; ++k) {
        lengthText = strlen(undoList[k])+2;
        if(k< ausiliarLastUndo){
            lengthMemory = strlen(ausiliarUndoList[k])+2;
            strcpy(temp,ausiliarUndoList[k]);
            free(ausiliarUndoList[k]);
        }
        ausiliarUndoList[k] = malloc(++lengthText);
        strcpy(ausiliarUndoList[k],undoList[k]);
        free(undoList[k]);
        if(k< ausiliarLastUndo){
            undoList[k] = malloc(++lengthMemory);
            strcpy(undoList[k],temp);
        }
    }
    for (int n = lastUndo; n < ausiliarLastUndo; ++n) {
        lengthMemory = strlen(ausiliarUndoList[n])+2;
        undoList[n] = malloc(++lengthMemory);
        strcpy(undoList[n],ausiliarUndoList[n]);
        free(ausiliarUndoList[n]);
    }

    // e il cambio?!

    temp1 = ausiliarLastUndo;
    ausiliarLastUndo = lastUndo;
    lastUndo = temp1;

    temp1 = ausiliarLastIndexRedo;
    ausiliarLastIndexRedo = lastIndexRedo;
    lastIndexRedo =temp1;

    temp1 = ausiliarLastMemory;
    ausiliarLastMemory = lastMemory;
    lastMemory = temp1;

    temp1 = ausiliarLastIndexNRows;
    ausiliarLastIndexNRows = lastIndexNRows;
    lastIndexNRows = temp1;

    temp2 = ausiliarLog1;
    ausiliarLog1 = log1;
    log1 = temp2;
}
/*
 * x ogni undo
 * salvo il testo completo su cui si faranno le undo + le info + importanti  (quindi free di quelle prima se c'erano)
 * se poi le redo consentono di tornare a nUndo done =0
 * -> sostituisco semplicemente il testo e indici e altri valori importanti
 * se invece devo fare per esempio 7r su 10 max vado a quella max e faccio da lì 3 undo ricordandosi di far scalare i comandi
 *
 *
 *
 *
 * salvo lo stato a 5k last indexnrows e faccio lo scambio se con l undo devo arrivare a 5k non potrò eseguire redo su di essa ma undo sì
 * quindi se da stato x passo a 5k lo faccio subito
 */


//ROLLING BACK salvare stati ogni 5k e andare solo indietro con essi quindi es da 20k a 100 passo a 5k poi 100  da 100 a 17k 20k - 3k di undo


/*
 * undo redo list
 * array dinamico con la testa dei puntatori
 * creo nodo per ogni roba da memorizzare
 *
 *
 *
 * se c
 * used > 0
 *      salvo primo ind nell array
 *      -> finche next!= null itero e sostituisco
 *      x undo
 *      se used > quanto ho in lista verifico xk ho la diff nel logger (intIndex[1] - intIndex[0])
 *       ---> used = quanto ho in lista e sostituisco
 *
 *
 *
 */