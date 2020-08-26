#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INITARRAY 30
#define INPUTSIZE 1028


//Stack_node *firstUndo = NULL;

//contatore per le istruzioni nulle
//puntatore per le stringhe
//controllare 0,0d o 0,0c

// OKAY
//size_t arrsize = number;


//change the size -> realloc(text, arrSize * sizeof(int))

//initialized array or malloc(numberEl* sizeof()int);



// free(text)


char** text;
char** memory;
char** logger;
char** undoList;

char input[50];
int used= 0;           //used text
int size = INITARRAY;
int sizeLog = INITARRAY;        //sizelogger
int memorySize = INITARRAY;
int undoSize = INITARRAY;
int lastCommand = 0;   //logger
int maxCommand = 0;    //logger
int lastMemory = 0;    //memory
int lastUndo = 0;     //undo
int inputLength;   //length of input
int nUndoDone = 0;

int* nRowsUndo; //how many rows i have to change for undo
int lastIndexNRows = 0;
int sizeNRows = INITARRAY;
int* nRowsRedo;
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



int debug = 0;    // JUST FOR DEBUG

int nUndo = 0;
int intIndex [2];

size_t lengthText = 0;
size_t lengthMemory = 0;
size_t lengthLog = 0;

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



int main(){
    text = malloc(INITARRAY * sizeof(char*));
  //  logger = malloc(INITARRAY * sizeof(char*));
    memory = malloc(INITARRAY * sizeof(char*));
    undoList = malloc(INITARRAY * sizeof(char*));
    nRowsUndo = malloc(INITARRAY * sizeof(int));
    nRowsRedo = malloc(INITARRAY * sizeof(int));


    while (1) {
        fgets(input, 50,stdin);
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
        inputLength = i;
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
    if(nUndo != 0){
        if(nUndo > 0){
            undo(nUndo);
        } else{
            redo(nUndo);
        }
        nUndo=0;

    }
    /*if(nUndoDone!=0){
        for (int i = lastCommand; i <= maxCommand; ++i) {
            free(logger[i]);
        }
    }*/
    //TODO:free the UNDO STACK
    nUndoDone =0;

    if(lastCommand == 1 ){
        free(log1);
        log1 = malloc(sizeof(log));
        log1 = NULL;
        lastCommand = 0;
    }
   /* for (int k = 0; k < lastUndo; ++k) {
        free(undoList[k]);
    }*/
   // lastUndo = 0;
    lastIndexRedo=0;

    //savingInput();   //for the undo and threat as a brand new input
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
        if(lastIndexNRows+1 >= sizeNRows){
            sizeNRows += INITARRAY;
            nRowsUndo = realloc(nRowsUndo, sizeNRows * sizeof(int ));
        }

        if(i > used){
            //memory[lastMemory] = malloc(4 * sizeof(char));    //if not used before save only a point
            //strcpy(memory[lastMemory], ".\n");                //OPTIMIZABLE?
            //no mo' se uso un intero per dire che è il minimo possibile

            /*
             * segno quanti da cambiare a comando -> so quanto devo leggere a botta
             * leggo quel valore e a giro sostituisco quei valori
             */
        } else{                                 // HERE PROBLEMSSSS
            lengthMemory = strlen(text[i])+2;
            memory[lastMemory]= malloc(++lengthMemory);   //save the exact row in memory
            strcpy(memory[lastMemory],text[i]);
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
    fgets(point,3,stdin);
    //devo aumentare nrows index
    nRowsUndo[lastIndexNRows] = sizeRows;
    ++lastIndexNRows;
}

void delete(int index1, int index2) {
    size_t len = 0;
    int lastIndex = index2;     //still not get if useful or not
    //free the UNDO STACK cant write over because of the size

    if(nUndo != 0){
        if(nUndo > 0){
            undo(nUndo);
        } else{
            redo(nUndo);
        }
        nUndo=0;
    }
    /*if(nUndoDone!=0){
        for (int i = lastCommand; i <= maxCommand; ++i) {
            free(logger[i]);
        }
    }*/

    nUndoDone =0;
   //
    if(lastCommand == 1 ){
        free(log1);
        log1 = malloc(sizeof(log));
        log1 = NULL;
        lastCommand = 0;
    }
   /*  for (int k = 0; k < lastUndo; ++k) {
        free(undoList[k]);
     }*/
    // lastUndo=0;
     lastIndexRedo=0;

   //save something in case of delete unused
   //NOP how to write it smart usage!!!

    //savingInput();
    saveInput(index1,index2,'d');


    //printf("%d",used);
    if(lastIndexNRows+1 >= sizeNRows){
        sizeNRows += INITARRAY;
        nRowsUndo = realloc(nRowsUndo, sizeNRows * sizeof(int));
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
           lengthMemory = strlen(text[j])+2;
           memory[lastMemory] = malloc(++lengthMemory);
           strcpy(memory[lastMemory],text[j]);
           ++lastMemory;
           //free(text[j]);
       }


        for(int i= lastIndex; i<= used;++i){    //index2 -> used
            len += strlen(text[i])+2; //      MAYBE HERE OPTIMIZE
            //text[index1+i-lastIndex]= malloc(++len);
            //strcpy(text[index1+i-lastIndex],text[i]);
        }

        //shiftare lo fa la memmove


        memcpy(text+index1,text+lastIndex+1 ,len);//*sizeof(char));


        //char** a = realloc(text,(--size) * sizeof(char*));


        nRowsUndo[lastIndexNRows] = lastIndex - index1 + 1;
        //printf("%d",nRowsUndo[lastIndexNRows]);
        used -= (lastIndex-index1+1);
   }else{
       //savingInput(); same as but with n instead of d
    //   logger[lastCommand-1][inputLength] = 'n';
       //printf("%s\n",logger[lastCommand-1]);
       log1->op = 'n';
       nRowsUndo[lastIndexNRows]= 0;
   }
    ++lastIndexNRows;
}

void print(int index1, int index2){

    if(nUndo != 0){
       // printf("%d\n",lastIndexNRows);
        if(nUndo > 0){
            //--lastCommand;
            undo(nUndo);

        } else{
            redo(nUndo);
            //++lastCommand;
           // printf("QUIIII");
        }
        //printf("%d\n",lastIndexNRows);
       // printf("INIZ ");
        nUndo=0;
        //lastCommand = 0;

    }
   //if(index1>=0) {
   //printf("%d\n",used);
       for (int i = index1; i <= index2; ++i) {
           //if doesnt exists (>= max numero valido )
           if(i>used || i <= 0){
               fputs(".\n",stdout);
           } else{
               fputs(text[i],stdout);
           }

       //}
   }
   // printf("FINE ,%d\n",lastIndexNRows);
    /*for (int m = 0; m < lastMemory; ++m) {
        printf("%s\n",memory[m]);
    }
    for (int l = 0; l < lastCommand; ++l) {
        printf(" com:   %s\n",logger[l]);
    }*/


}

void undo(int nTimes){
    size_t length,len,len_t;
    char *temp [size];


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
            undoSize += (INITARRAY + used) ;
            undoList = realloc( undoList , undoSize * sizeof(char*));  //rethink
        }
        len = 0;
        length = 0;
        len_t = 0;
        if(lastIndexRedo  >= sizeNRowsRedo){
            sizeNRowsRedo += INITARRAY ;
            nRowsRedo = realloc(nRowsRedo, sizeNRowsRedo * sizeof(int));
        }
        /*for (int m = 0; m < lastUndo; ++m) {
            printf("%s\n",undoList[m]);
        }*/
        //printf("%d, %s\n",lastCommand,logger[2]);
        /*for (int l = 0; l < lastCommand; ++l) {
            printf(" com:   %s\n",logger[l]);
        }*/
        //log *temp = log1;
        /* while (temp!= NULL){
            printf("%d %d %c\n",temp->index1,temp->index2,temp->op);
            temp = temp->next;
        }*/

        char instr = parsedMem();
        if(log1->prev != NULL){
            log1 = log1->prev;
        } /*else{
            printf("00000000");
        }*/

    //    printf("%d , %d %c %d\n",intIndex[0],intIndex[1],instr,lastIndexNRows);

        //printf("\nQUI\n");
        if(lastMemory == 0){
            for (int j = 1; j <=used ; ++j) {
                length = strlen(text[j])+2;
                undoList[lastUndo] = malloc(++length);
                strcpy(undoList[lastUndo],text[j]);
                lastUndo++;
            }
            nRowsRedo[lastIndexRedo] = used;
            used=0;
            log *temp1 = log1;
            //guardi quanto manca e cicli di conseguenza tanto tutte le prossime istruzioni sono nulle
            ++i;
            lastIndexNRows--;
            lastIndexRedo++;
            while(i< nTimes){
                if(temp1->prev != NULL){
                    temp1 = temp1->prev;
                }
                lastIndexRedo++;
                nRowsRedo[lastIndexRedo] = 0;
                lastIndexNRows--;
                ++i;
            }
            log1 = temp1;
            lastCommand = 1;
            instr = 'n';
        }

        if(instr == 'c'){

            for (int j = intIndex[0]; j <= intIndex[1]; ++j) {

                length = strlen(text[j])+2;
                undoList[lastUndo] = malloc(++length);

                strcpy(undoList[lastUndo],text[j]);
                lastUndo++;
                redo++;
                //free(text[j]);

                if(j< nRowsUndo[lastIndexNRows -1]+intIndex[0] ){
                    lastMemory--;
                    len_t += strlen(memory[lastMemory])+2;
                }

            }
            for (int k = intIndex[1]+1; k <= used; ++k) {
                len += strlen(text[k])+2;
            }

           // if(lastMemory != 0){
                memcpy(temp,text+intIndex[1]+1,len);
                memcpy(text + intIndex[0],memory +lastMemory,len_t);
                memcpy(text+intIndex[1]+1,temp,len);


            //}
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
                int usable = intIndex[1]-intIndex[0]+1-nRowsUndo[lastIndexNRows - 1];
                used -= usable;
            }


        } else if(instr == 'd'){

            for (int j = intIndex[0]; j<= used; ++j) {
                length += strlen(text[j])+2;
            }
            memcpy(temp,text+intIndex[0],length);
            for (int k = 0; k < nRowsUndo[lastIndexNRows -1]; ++k) {
                --lastMemory;
                len_t += strlen(memory[lastMemory])+2;
               // text[intIndex[0]+k] = malloc(++len_t);
             //   strcpy(text[intIndex[0]+k],memory[lastMemory]);
            }
            memcpy(text+intIndex[0],memory+ lastMemory,len_t);
           // printf("%s    aaa",text[intIndex[0]]);
            memcpy(text+intIndex[0]+nRowsUndo[lastIndexNRows -1],temp,length);
            used += nRowsUndo[lastIndexNRows -1];
            redo = nRowsUndo[lastIndexNRows -1];

        }else if(instr =='n'){
            //DO NOTHING
        }
      /*  if(lastIndexNRows==1){
            used =0;
        }*/

        //printf("%s\n",text[used+2]);

        if(lastCommand != 1){
            nRowsRedo[lastIndexRedo] = redo;
            ++lastIndexRedo;
            --lastIndexNRows;
        }


        /*if(1){
            printf("quiiii!\n");
            for (int l = 0; l < 20; ++l) {
                printf("%s\n",logger[l]);
            }
        }*/



        //printf("%d, %d",lastIndexNRows,nRowsUndo[lastIndexNRows]);
       // printf(" ,%d , %d %d \n",lastMemory,lastCommand, used);



    }
    //++lastMemory;

}

void redo(int nTimes){
    size_t len,length,len_t;
    char command = 'n';
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



       // printf("%d",lastCommand);

        //++lastCommand;
       /* for (int l = 0; l < 20; ++l) {
                   printf("%s\n",logger[l]);
               }*/
       // printf("%d %d %d %d\n",lastIndexRedo,lastUndo,lastIndexNRows,lastCommand);
       // printf("%d ",nRowsRedo[lastIndexRedo-1]);


    //    printf("\nquaaaaaack\n");
        int undo=0;
        len = 0;
        len_t =0;
        if(lastIndexNRows == 0) {
            /* log *temp = log1;
             while (temp->prev != NULL){
                 temp = temp->prev;
             }
             log1 = temp;*/
            command = parsedMem();
            lastCommand = 0;
        }else{
             //if(log1->next != NULL )
                 log1 = log1->next;

             command = parsedMem();
        }

    //    printf("\n%d %d %d %c ",i,intIndex[0],intIndex[1],command);

      // char command = 'n';
        if(command == 'c'){
            for (int k = intIndex[0]; k <= intIndex[1] ; ++k) {

                if(k<= used){
                    lengthMemory = strlen(text[k])+2;
                    memory[lastMemory]= malloc(++lengthMemory);   //save the exact row in memory
                    strcpy(memory[lastMemory],text[k]);
                    ++lastMemory; //increase the Stack of the memory ( x UNDO)
                    ++undo;
                }

                // free(text[k]);
                --lastUndo;
                len_t += strlen(undoList[lastUndo])+2;
            }
          //  printf("%d",lastUndo);
            for (int k = intIndex[1]+1; k <= used; ++k) {
                len += strlen(text[k])+2;
            }

            /*if(lastUndo!=0){
                memcpy(temp1,text+intIndex[1]+1,len);
                memcpy(text + intIndex[0],undoList +lastUndo,len_t);
                memcpy(text+intIndex[1]+1,temp1,len);
            } else{*/
                for (int j = 0; j < nRowsRedo[lastIndexRedo -1]; ++j) {
                    length = strlen(undoList[j+lastUndo])+2;
                    //free(text[j+intIndex[0]]);
                    text[j+intIndex[0]]= malloc(++length);
                    strcpy(text[j+intIndex[0]],undoList[j+lastUndo]);
                }
          //}


            if(intIndex[1] > used) {
                used = intIndex[1];
            }
           // undo = nRowsRedo[lastIndexRedo - 1];
        } else if (command == 'd'){
            int lastIndex = intIndex[1];
            if(intIndex[1] > used){
                lastIndex = used;
            }

            for (int k = intIndex[0]; k <= lastIndex ; ++k) {
                lengthMemory = (strlen(text[k]))+2;
                memory[lastMemory] = malloc(++lengthMemory);
                strcpy(memory[lastMemory],text[k]);
                ++lastMemory;
                //free(text[k]);
            }

            for (int j = lastIndex; j <= used; ++j) {
                len+=strlen(text[j])+2;
               // text[intIndex[0]+j-lastIndex]= malloc(++len);
               // strcpy(text[intIndex[0]+j-lastIndex],text[j]);
            }
            memcpy(text+intIndex[0],text+lastIndex+1,len);

            undo = lastIndex-intIndex[0]+1;
            //printf("%d",nRowsUndo[lastIndexNRows]);
            used -= lastIndex-intIndex[0]+1;
        } else if (command == 'n'){
            //do nothing
        }

        nRowsUndo[lastIndexNRows] = undo;
       // printf("\n QUI\n");
        lastIndexRedo--;
        ++lastIndexNRows;

    }





        //printf("%c, %d , %d ,%d \n",command,intIndex[0],intIndex[1],nRowsUndo[lastIndexNRows-1]);
      /*  if(command == 'c'){
           // if(nRowsUndo[lastIndexNRows-1]==0){
              /*  for(int j=0;j< used;++j){     //MORE EFFICIENT WAY DOING THE MEMMOVE AFTER
                    lengthMemory = strlen(text[j]) * sizeof(char);
                    memmove(memory + lastMemory, text + j, lengthMemory);
                    ++lastMemory;
                }
                //used = 0;
            //} else{
           // printf("%d\n",nRowsUndo[lastIndexNRows - 1]);
              //  for (int j = intIndex[0]; j < intIndex[0] + nRowsUndo[lastIndexNRows - 1]; ++j) {
                //    if(used > 0){
                      //  lengthMemory = strlen(text[j]) * sizeof(char);
                        //undoList[lastUndo] = malloc(++lengthMemory);
                        //memmove(memory + lastMemory, text + j, lengthMemory);  //qua riscrivo la mem ma non serve
                        //   strcpy(undoList[lastUndo],text[j]);
                        // free(text[j]);
                       // --lastMemory;
                    }

                    length += strlen(undoList[lastUndo-1]);
                    --lastUndo;
                    //int index = lastMemory + intIndex[1] - intIndex[0] - 1;
                    //if(strcmp(memory[index],".\n")==0){
                    //  lastIndex = j;
                    //}
                    // length += strlen(memory[index]) * sizeof(char);  //lastmemory-1 -index2+index1
                    //text[j] = malloc(++lengthText); // e se facessi memmove?!
                    //strcpy(text[j],memory[index]);
                    //--lastMemory;
                    len+= strlen(text[j]);
                    ++lastMemory;

                }
                if (intIndex[1] > used) {
                    used = intIndex[1];
                }
                if(nRowsUndo[lastIndexNRows - 1]!=0){
                    memmove(text + intIndex[0], memory + lastMemory , len);
                    memmove(text + intIndex[0], undoList + lastUndo-1 , length);
                }

            //free(undoList + lastUndo); // basta o di piu?!

        } else if( command == 'd'){
            if(intIndex[1] > used){
                intIndex[1] = used;
            }
            for (int j = intIndex[0]; j < used; ++j) {
                len+=strlen(text[j])+2;
                lengthMemory = (strlen(text[j]));
                memory[lastMemory] = malloc(++lengthMemory);
                strcpy(memory[lastMemory],text[j]);
                ++lastMemory;
            }
            memmove(text+intIndex[0],text+intIndex[1]+1,len);

            nRowsUndo[lastIndexNRows-1] = intIndex[1]-intIndex[0] +1;
            //printf("%d",nRowsUndo[lastIndexNRows]);
            used -= nRowsUndo[lastIndexNRows - 1];
        }

        //nUndoDone--;
        //lastCommand++;
    }*/

}

void savingInput(){
    if(0){
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
        }*/
        lengthLog = inputLength+2;
        logger[lastCommand] = malloc (++lengthLog);
        strcpy(logger[lastCommand],input);
        ++lastCommand;
        ++maxCommand;
    } else{


    }

}

void incrementingUndo(int nTimes){
    for (int i = 0; i < nTimes && lastIndexNRows-nUndo >0; ++i) {
        nUndo++;
        //printf("HowManyUN\n");
        nUndoDone++;
    }
}

void decrementingUndo(int nTimes){
    for (int i=0;nUndoDone > 0 && i< nTimes;i++){
        nUndo--;
        //printf(" HowManyRE\n");
        //print(1,3);
        nUndoDone--;
    }
   // printf("%d\n",nUndo);
}

void saveInput(int index1,int index2,char o){

   log* newLog = (log*) malloc(sizeof(log));
   newLog->index1 = index1;
   newLog->index2 = index2;
   newLog->op = o;
   newLog->next = NULL;
   newLog->prev = log1;
   if(log1 != NULL){
       log1->next = newLog;
   }
   log1=newLog;

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
    if (log1!= NULL){
        intIndex[0]= log1->index1;
        intIndex[1]= log1->index2;
        return log1->op;
    }
    return 'n';


}

int parsedInput(){
  /*  int c = logger[--lastCommand];
    intIndex[1]= logger[--lastCommand];
    intIndex[0]= logger[--lastCommand];
    return c;*/
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
 *                      initialized 1025 for every rows and then after some realloc     NOPE
 *                      undo and redo united to improve!
 *
 *
 *
 *
 *
 *                 BUG Memory allocated could create problems when delete if 1,2d and for example the 2 line is shorter than the fourth
 *                                      this could create problems!
 *                     1,1d the 2 line remain it won't shift
 *
 *
 *
 *
 *
 *
 * ogni volta che viene chiamata l undo/redo somma/sottrai su una var globale
 *
 * prima di una change o d se ci sono undo da fare eseguile
 *
 *
 *
 *
 *
 *
 * 1 u leggo ultimo input if n -> do nothing
 * if c ripristina quelle c fino a strcmp != ".\n"
 * if d ripristina fai una memmove es 1,3 2,2d memmove ma se ho 1,5d ripristina quello che hai salvato con una memmove
 *
 *
 * redo leggo input -> esegui normalmente
 *
 *
 * esempio faccio 1 u sull input e sposto il puntatore degli input di -x mosse
 * per fare redo rieseguo quell input ma nel caso del c ho già in memoria le stringhe
 *
 *
 *
 *
 *
 *
 * nRowsUndo mi dice quanti indici tra index1 e index2 sono effettivamente stati utilizzati
 */



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
 *
 *                  REDO
 *
 *                        ++lastCommand;

        len = 0;
        length = 0;
        char command = parsedMem();
        nRowsUndo[lastIndexNRows]= 0;
        if(command == 'c'){
            for (int j = 0; j < nRowsRedo[lastIndexRedo - 1]; ++j) {
                lastUndo--;
                len += strlen(undoList[lastUndo]);
                length += strlen(text[j+intIndex[0]]);
            }

            memmove(memory+ lastMemory, text+ intIndex[0],length);
            lastMemory += nRowsRedo[lastIndexRedo - 1];
            memmove(text + intIndex[0],undoList + lastUndo,len);


            if(intIndex[1] > used) {
                used = intIndex[1];
            }
            nRowsUndo[lastIndexNRows] = nRowsRedo[lastIndexRedo - 1];
        } else if (command == 'd'){
            if(intIndex[1] > used){
                intIndex[1] = used;
            }
            for (int k = intIndex[0]; k <= intIndex[1] ; ++k) {
                lengthMemory = (strlen(text[k]));
                memory[lastMemory] = malloc(++lengthMemory);
                strcpy(memory[lastMemory],text[k]);
                ++lastMemory;
            }

            for (int j = intIndex[1]; j <= used; ++j) {
                len+=strlen(text[j]);
            }
            memmove(text+intIndex[0],text+intIndex[1]+1,len);

            nRowsUndo[lastIndexNRows] = nRowsRedo[lastIndexRedo - 1];
            //printf("%d",nRowsUndo[lastIndexNRows]);
            used -= nRowsRedo[lastIndexRedo - 1];
        }
        lastIndexRedo--;
        ++lastIndexNRows;

 *
 *              UNDO
 *
 *
 *                for(int i=0;i< nTimes;++i) {
       // --lastCommand;
        char decision = parsedMem();
        len_tot = 0;
        len=0;
        length=0;
        nRowsRedo[lastIndexRedo] = 0;
        if (lastUndo + used  >= undoSize) {
            undoSize += INITARRAY+ used ;
            undoList = realloc( undoList , undoSize * sizeof(char *));  //rethink
            //devo salvare i comandi eseguiti
        }
        // non devo risalvare l input quindi o var glob per farlo o nuova azione
        if (decision == 'c') {
            //change like

            //salvare le cose che c erano prima nella undo list se non c'era nulla prima salvo i punti
            //salvo tot elementi come nella delete e poi memmove
            if(nRowsUndo[lastIndexNRows - 1] == 0){
                for(int j=1;j<= used;++j){
                    len += strlen(text[j]) * sizeof(char);
                }
                memmove(undoList + lastUndo, text + intIndex[0], len);
                 //PROBL?
                if(intIndex[0] == 1){
                    nRowsRedo[lastIndexRedo] = used;
                    lastUndo += used+1;
                    used = 0;
                } else{
                    int howMany = intIndex[1] - intIndex[0]+1;
                    nRowsRedo[lastIndexRedo] = howMany;
                    lastUndo += howMany+1;
                    used -= howMany;
                }

            }else {
                for (int j = intIndex[0]; j <= intIndex[1]; ++j) {

                        //ne metto intIndex[1]-intIndex[0] +1 in mem
                        //ne sposto solo nRows...
                        //cambio used


                   if(j < intIndex[0] + nRowsUndo[lastIndexNRows - 1]){
                       --lastMemory;
                       len += strlen(memory[lastMemory]);//+2;
                   }


                    length += strlen(text[j]);//+2;// * sizeof(char);

                      //possible improve
                    //   strcpy(undoList[lastUndo],text[j]);
                    // free(text[j]);


                    //int index = lastMemory + intIndex[1] - intIndex[0] - 1;
                    //if(strcmp(memory[index],".\n")==0){
                    //  lastIndex = j;
                    //}
                   // length += strlen(memory[index]) * sizeof(char);  //lastmemory-1 -index2+index1
                    //text[j] = malloc(++lengthText); // e se facessi memmove?!
                    //strcpy(text[j],memory[index]);
                    //--lastMemory;
                }

               //
                memmove(text + intIndex[0], memory + lastMemory , len);
                memmove(undoList + lastUndo, text + (intIndex[0]+nRowsUndo[lastIndexNRows -1]), length);
                lastUndo += intIndex[1]-intIndex[0]+1;
                //lastUndo++;
                if(intIndex[1] >= used){
                    int usable = intIndex[0] + nRowsUndo[lastIndexNRows -1]-1;
                    if (usable < used) {
                        used = usable;
                    }
                }
                nRowsRedo[lastIndexRedo] = intIndex[1]-intIndex[0]+1;


                //++lastMemory;
            }
        } else if(decision == 'd') {    //check lastindex and sooo

            //problems with 1,1d

            //salvo il comando che avrei fatto x es 1,3d
            for(int k=0; k < nRowsUndo[lastIndexNRows - 1]; ++k){
                //int index= lastMemory-1; -(k+1)
                //printf("%d\n",nRowsUndo[lastIndexNRows-1]);
                --lastMemory;
                length += strlen(memory[lastMemory]);
                  // devo fare free?
            }      //quanti indirizzi devo leggere tra intIndex 0 e 1
            //calcolo da index2 a fine la size e poi sposto

                      //IDEA: UNISCO I CICLI!

            for(l=intIndex[0];l<=used;++l){ //spostare i valori già utilizzati
                len+= strlen(text[l]);
                //printf("%d\n",lastIndex);
               // printf("%d\n",lastIndexNRows);

            }
            if(len!=0){   // se già c era qualcosa a quell index
                int index = intIndex[0] + nRowsUndo[lastIndexNRows - 1];
               // printf("%d\n",index);
                memmove(text+index ,text+intIndex[0],len*sizeof(char)); //sposto chi è negli index incriminati dopo
               // printf("HERE?\n");
               // printf("%d,%d\n",l,intIndex[0]);
               // printf("%s\n",text[index]);

            }

            //printf("%s",memory[lastMemory-1]);
            //printf("%d", lastMemory);  //errore
            //printf("%s\n",memory[index]);
            memmove(text+intIndex[0],memory+lastMemory,length*sizeof(char));
            //printf("%s\n",text[intIndex[0]]);

            //++lastMemory;

            //salvo gli elementi in quelle posizioni sempre nell undo list
            //memmove like a inverse delete from indice attuale to index2+1(last index che potrebbe non essere index2)
            //memmove dalla memoria a quegli indici

            nRowsRedo[lastIndexRedo] = nRowsUndo[lastIndexNRows -1];

            used += nRowsUndo[lastIndexNRows - 1];  //check
            //printf("%d\n",nRowsUndo[lastIndexNRows-1]);
        }
        //decremento l indice degli input
        //--lastIndexNRows;
        --lastCommand;
        //++lastUndo;
       // nUndoDone++;
       --lastIndexNRows;
        ++lastIndexRedo;
        printf("%d",lastIndexNRows);
        printf(" ,%d , %d %d \n",lastMemory,lastCommand, used);
    }
 *
 *
 *
 *
 *undo con liste carico in una lista i valori da cambiare
 *
 *
 *
 * void undo(int nTimes){
    int l;
    size_t length,len;

    //CHECK THE USED!!!!    improve!!!!
    if(lastIndexRedo+nTimes+1 >=  sizeNRowsRedo){
        sizeNRowsRedo+= INITARRAY+nTimes;
        nRowsRedo =  realloc(nRowsRedo, sizeNRowsRedo * sizeof(int));
    }

for(int i=0;i< nTimes;++i) {
char decision = parsedMem();
len=0;
length=0;
nRowsRedo[lastIndexRedo] = 0;
if (lastUndo + used  >= undoSize) {
undoSize += INITARRAY+ used ;
undoList = realloc( undoList , undoSize * sizeof(char *));  //rethink
//devo salvare i comandi eseguiti
}
// non devo risalvare l input quindi o var glob per farlo o nuova azione
if (decision == 'c') {
//change like

//salvare le cose che c erano prima nella undo list se non c'era nulla prima salvo i punti
//salvo tot elementi come nella delete e poi memmove
if(nRowsUndo[lastIndexNRows - 1] == 0){
for(int j=intIndex[0];j<= used;++j){
len = strlen(text[j])+2; //* sizeof(char);
undoList[lastUndo] = malloc(++len);
strcpy(undoList[lastUndo],text[j]);
++lastUndo;
}
//PROBL?
if(intIndex[0] == 1){
nRowsRedo[lastIndexRedo] = used;
used = 0;
} else{
int howMany = intIndex[1] - intIndex[0]+1;
nRowsRedo[lastIndexRedo] = howMany;
used -= howMany;
}

}else {
for (int j = intIndex[0]; j <= intIndex[1]; ++j) {

//ne metto intIndex[1]-intIndex[0] +1 in mem
//ne sposto solo nRows...
//cambio used

if(j < intIndex[0] + nRowsUndo[lastIndexNRows - 1]){
--lastMemory;
len += strlen(memory[lastMemory])+2;//+2;
}


length = strlen(text[j])+2;//+2;// * sizeof(char);
undoList[lastUndo] = malloc(++length);
strcpy(undoList[lastUndo],text[j]);
++lastUndo;

//possible improve
//   strcpy(undoList[lastUndo],text[j]);
// free(text[j]);


//int index = lastMemory + intIndex[1] - intIndex[0] - 1;
//if(strcmp(memory[index],".\n")==0){
//  lastIndex = j;
//}
// length += strlen(memory[index]) * sizeof(char);  //lastmemory-1 -index2+index1
//text[j] = malloc(++lengthText); // e se facessi memmove?!
//strcpy(text[j],memory[index]);
//--lastMemory;
}

memmove(text + intIndex[0], memory + lastMemory , len);

if(intIndex[1] >= used){   //rethink
int usable = intIndex[0] + nRowsUndo[lastIndexNRows -1]-1;
if (usable < used) {
used = usable;
}
}
nRowsRedo[lastIndexRedo] = intIndex[1]-intIndex[0]+1;


//++lastMemory;
}
} else if(decision == 'd') {    //check lastindex and sooo

//problems with 1,1d

//salvo il comando che avrei fatto x es 1,3d
for(int k=0; k < nRowsUndo[lastIndexNRows - 1]; ++k){
//int index= lastMemory-1; -(k+1)
//printf("%d\n",nRowsUndo[lastIndexNRows-1]);
--lastMemory;
length += strlen(memory[lastMemory])+2;
// devo fare free?
}      //quanti indirizzi devo leggere tra intIndex 0 e 1
//calcolo da index2 a fine la size e poi sposto

//IDEA: UNISCO I CICLI!

for(l=intIndex[0];l<=used;++l){ //spostare i valori già utilizzati
len+= strlen(text[l])+2;
//printf("%d\n",lastIndex);
// printf("%d\n",lastIndexNRows);

}
if(len!=0){   // se già c era qualcosa a quell index
int index = intIndex[0] + nRowsUndo[lastIndexNRows - 1];
memmove(text+index ,text+intIndex[0],len*sizeof(char)); //sposto chi è negli index incriminati dopo
}

memmove(text+intIndex[0],memory+lastMemory,length*sizeof(char));
//printf("%s\n",text[intIndex[0]]);

//++lastMemory;

//salvo gli elementi in quelle posizioni sempre nell undo list
//memmove like a inverse delete from indice attuale to index2+1(last index che potrebbe non essere index2)
//memmove dalla memoria a quegli indici

nRowsRedo[lastIndexRedo] = nRowsUndo[lastIndexNRows -1];

used += nRowsUndo[lastIndexNRows - 1];  //check
//printf("%d\n",nRowsUndo[lastIndexNRows-1]);
}
//decremento l indice degli input
//--lastIndexNRows;
--lastCommand;
//++lastUndo;
// nUndoDone++;
--lastIndexNRows;
++lastIndexRedo;
printf("%d",lastIndexNRows);
printf(" ,%d , %d %d %d\n",lastMemory,lastCommand, used,lastUndo);
}



}
 *
 *
 *
 */