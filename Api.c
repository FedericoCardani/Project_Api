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


/*
 * array of number of rows
 */



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



int main(){
    text = malloc(INITARRAY * sizeof(char*));
    logger = malloc(INITARRAY * sizeof(char*));
    memory = malloc(INITARRAY * sizeof(char*));
    undoList = malloc(INITARRAY * sizeof(char *));
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
       // index2 = index2 - (command[--i] -offset);
        inputLength = i;
        decision = command[i];
        operation(index1, index2, decision);
        //printf("%d,%d",inputLength,strlen(input));


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

    nUndoDone=0;
    //TODO:free the UNDO STACK

    savingInput();   //for the undo and threat as a brand new input

    for (int i = index1; i <= index2; ++i) {

        fgets(row,INPUTSIZE,stdin);
        if(i > size){     //posso farlo all inizio e non ogni volta
            size += INITARRAY;
            text = realloc(text,size * sizeof(char*));
        }

        if(lastMemory > memorySize){          //posso farlo all inizio e non ogni volta
            memorySize += INITARRAY;
            memory = realloc(memory,memorySize * sizeof(char*));
        }
        if(lastIndexNRows == sizeNRows){
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
            lengthMemory = strlen(text[i]);

            memory[lastMemory]= malloc(++lengthMemory);   //save the exact row in memory
            strcpy(memory[lastMemory],text[i]);
            ++sizeRows;
            ++lastMemory; //increase the Stack of the memory ( x UNDO)
        }

        // text = malloc(strlen(row) * sizeof(char));
        lengthText = strlen(row);   // lenght of the row saved
        text[i] = malloc (++lengthText);
        strcpy(text[i],row);

    }
    if(index2> used)
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


    nUndoDone = 0;
   //save something in case of delete unused
   //NOP how to write it smart usage!!!

    savingInput();

    //printf("%d",used);
    if(lastIndexNRows == sizeNRows){
        sizeNRows += INITARRAY;
        nRowsUndo = realloc(nRowsUndo, sizeNRows * sizeof(int ));
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

       if((lastMemory+used-index1+1) > memorySize){          //posso farlo all inizio e non ogni volta
           memorySize += INITARRAY+used-index1;
           memory = realloc(memory,memorySize* sizeof(char *));
       }

       //da index1 a lastindex salvo in mem
       for (int j = index1; j <= lastIndex; ++j) {
           lengthMemory = strlen(text[j]);
           memory[lastMemory] = malloc(++lengthMemory);
           strcpy(memory[lastMemory],text[j]);
           ++lastMemory;
       }


        for(int i= lastIndex; i<= used;++i){    //index2 -> used
            len += strlen(text[i])+2; //      MAYBE HERE OPTIMIZE
            //lengthMemory = (strlen(text[i]) * sizeof(char ));   USEFUL?
            //memory[lastMemory] = malloc(++lengthMemory); //Error
            //memcpy(memory[lengthMemory],text[i],lengthMemory); //NOPE
           // memory[lastMemory] = (char *) &text[i];

            // HERE ERRORS
           // strcpy(memory[lastMemory], text[i]);
           // printf("%s\n",memory[lastMemory]);
           // ++lastMemory;   //do something when index2 > used  REMEMBER!!!!!!!!!!!
        }

       //memcpy(memory[lastMemory], text+index1,len* sizeof(char )); NOOO



        //shiftare lo fa la memmove

        memmove(text+index1,text+lastIndex+1 ,len*sizeof(char));

        //copio la memoria su un nuovo array e poi riporto tanto so la lunghezza che avrà    NOT GOOD

        //char** a = realloc(text,(--size) * sizeof(char*));


        nRowsUndo[lastIndexNRows] = lastIndex - index1 + 1;
        //printf("%d",nRowsUndo[lastIndexNRows]);
        used -= (lastIndex-index1+1);

   }

   else{
       //savingInput(); same as but with n instead of d
       logger[lastCommand-1][inputLength] = 'n';
       //printf("%s\n",logger[lastCommand-1]);
       nRowsUndo[lastIndexNRows]= 0;
   }
    ++lastIndexNRows;
}

void print(int index1, int index2){

    if(nUndo != 0){
        if(nUndo > 0){
            undo(nUndo);
        } else{
            redo(nUndo);
           // printf("QUIIII");
        }
        nUndo=0;
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
       printf("FINE\n");

}

void undo(int nTimes){
    int lastIndex,l;
    size_t length,len;

    //CHECK THE USED!!!!    improve!!!!
    if(sizeNRowsRedo == lastIndexRedo){
        sizeNRowsRedo+= INITARRAY;
        nRowsRedo =  realloc(nRowsRedo, sizeNRowsRedo * sizeof(int));
    }

    for(int i=0;i< nTimes;++i) {
        char decision = parsedMem(); // non devo risalvare l input quindi o var glob per farlo o nuova azione
        len=0;
        length=0;
        if (lastUndo-intIndex[0]+intIndex[1] > undoSize) {
            undoSize += INITARRAY;
            undoList = realloc(undoList, undoSize * sizeof(char *));  //rethink
            //devo salvare i comandi eseguiti
        }
        if (decision == 'c') {
            //change like

            //salvare le cose che c erano prima nella undo list se non c'era nulla prima salvo i punti
            //salvo tot elementi come nella delete e poi memmove
            if(nRowsUndo[lastIndexNRows - 1] == 0){
                for(int j=1;j<= used;++j){
                    len += strlen(text[j]) * sizeof(char);

                   // ++lastUndo;
                    //printf("QUIII");

                }
                memmove(undoList + lastUndo, text + intIndex[0], len);
                lastUndo += used;
                nRowsRedo[lastIndexRedo] = used;  //PROBL?
                used = 0;
            }else {
                for (int j = intIndex[0]; j < intIndex[0] + nRowsUndo[lastIndexNRows - 1]; ++j) {
                    lengthMemory = strlen(text[j]) * sizeof(char);
                    //undoList[lastUndo] = malloc(++lengthMemory);
                    memmove(undoList + lastUndo, text + j, lengthMemory);  //possible improve
                    //   strcpy(undoList[lastUndo],text[j]);
                    // free(text[j]);
                    ++lastUndo;
                    --lastMemory;
                    length += strlen(memory[lastMemory]);

                    //int index = lastMemory + intIndex[1] - intIndex[0] - 1;
                    //if(strcmp(memory[index],".\n")==0){
                    //  lastIndex = j;
                    //}
                   // length += strlen(memory[index]) * sizeof(char);  //lastmemory-1 -index2+index1
                    //text[j] = malloc(++lengthText); // e se facessi memmove?!
                    //strcpy(text[j],memory[index]);
                    //--lastMemory;
                }
                if (intIndex[1] < used) {
                    used = intIndex[1];
                }
                nRowsRedo[lastIndexRedo]= nRowsUndo[lastIndexNRows -1];
            }
                memmove(text + intIndex[0], memory + lastMemory , length);
                ++lastMemory;


        } else if(decision == 'd') {    //check lastindex and sooo

            //problems with 1,1d

            //salvo il comando che avrei fatto x es 1,3d
            for(int k=0; k < nRowsUndo[lastIndexNRows - 1]; ++k){
                //int index= lastMemory-1; -(k+1)
                //printf("%d\n",nRowsUndo[lastIndexNRows-1]);
                length += strlen(memory[lastMemory-1]);
                --lastMemory;   // devo fare free?
            }      //quanti indirizzi devo leggere tra intIndex 0 e 1
            //calcolo da index2 a fine la size e poi sposto


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
    }



}

void redo(int nTimes){
    int lastIndex = intIndex[1];
    size_t len,length;
    //++lastIndexNRows;
    //++lastCommand;

    for (int i = nTimes; i != 0; ++i) {

        /*
         * aumenting index last command and lastnindexrow
         *
         * if c
                leggo quanto da sostituire nrows [index]

                se gia qualcosa a quegli index metto nella mem

                leggo da undolist cosa da sostituire

                memmove
         */

        ++lastCommand;
        lastIndexNRows++;
        //lastMemory += nRowsUndo[lastIndexNRows - 1];

        len = 0;
        length = 0;
        char command = parsedMem();
        if(command == 'c'){
            for (int j = 0; j < nRowsUndo[lastIndexNRows - 1]; ++j) {
                len += strlen(undoList[lastUndo]);
                lastUndo--;
                length += strlen(text[j+intIndex[0]]);
            }
            memmove(memory+ lastMemory, text+ intIndex[0],length);
            lastMemory += nRowsUndo[lastIndexNRows - 1];
            memmove(text + intIndex[0],undoList + lastUndo,len);

            if(intIndex[1]> used) {
                used = intIndex[1];
            }
        } else if (command == 'd'){
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

            nRowsUndo[lastIndexNRows - 1] = intIndex[1] - intIndex[0] + 1;
            //printf("%d",nRowsUndo[lastIndexNRows]);
            used -= nRowsUndo[lastIndexNRows - 1];
        }

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
    if(lastCommand == sizeLog ){
        sizeLog += INITARRAY;
        logger = realloc(logger,sizeLog * sizeof(char*));
    }

    lengthLog = inputLength+2;
    logger[lastCommand] = malloc (++lengthLog);
    strcpy(logger[lastCommand],input);
    ++lastCommand;
}

void incrementingUndo(int nTimes){
    for (int i = 0; i < nTimes && lastIndexNRows-i >0; ++i) {
        nUndo++;
        nUndoDone++;
    }
}

void decrementingUndo(int nTimes){
    for (int i=0;nUndoDone > 0 && i< nTimes;i++){
        nUndo--;
        printf(" HowMany\n");
        nUndoDone--;
    }
   // printf("%d\n",nUndo);
}


char parsedMem(){
    //logger[lastCommand]
    if(lastCommand > 0){
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
        return logger[lastCommand - 1][i];
    }
    return 'n';  //n->nop

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



