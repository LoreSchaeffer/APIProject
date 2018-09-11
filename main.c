#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT 4
#define SIZE 64
#define DEBUG 1

//Transitions
int* in;
char* read;
char* write;
char* move;
int* out;
int trSize = 0;

//Params TM
int* acc;
int accSize = 0;
long max;

//Structs
typedef struct Tape {
    char* tape;
    size_t size;
    size_t pointer;
} Tape;

//Global vars
Tape tape;
int outTmp = 0;
size_t offset = 0;


void printTr() {
    for(int i = 0; i < trSize; i++) {
        printf("Transition {in=%i, read=%c, write=%c, move=%c, out=%i}\n", in[i], read[i], write[i], move[i], out[i]);
    }
}

void printStatus() {
    printf("--------\n");
    printTr();
    printf("TrSize=%i\n", trSize);
    printf("\n\n");
    printf("Acc [");
    for(int i = 0; i < accSize; i++) {
        printf("%i", acc[i]);
        if(i != accSize - 1) printf(", ");
    }
    printf("]\n\n");
    printf("Max=%ld\n", max);
    printf("--------\n\n");
}

char* getChunk(size_t pointer) {
    pointer = pointer - offset;
    char* chunk = malloc(SIZE + 1);
    memcpy(chunk, tape.tape + pointer, SIZE);
    chunk[SIZE] = '\0';

    if(pointer + SIZE > tape.size) {
        for(size_t p = tape.size; p < pointer + SIZE; p++) {
            chunk[p] = '_';
        }
    }
    return chunk;
}

char* cleaner(char* tape, size_t start) {
    for(size_t p = start; p < start + SIZE; p++) {
        tape[p] = '_';
    }
    return tape;
}

/*int isInTheMiddle(size_t pointer) {
    pointer = pointer - offset;
    if(pointer >= (tape.size / SIZE) * SIZE) {
        return 0;
    }
    return 1;
}*/

int isLast(size_t pointer) {
    size_t gp = pointer - offset;
    if(gp < tape.size - 1) return 0;
    return 1;
}

int isAcc(int outTmp) {
    for(int i = 0; i < accSize; i++) {
        if(outTmp == acc[i]) return 1;
    }
    return 0;
}

int* findTransitions(char* tape, size_t pointer) {
    int* transitions = malloc(sizeof(int));
    int size = 0;

    for(int tr = 0; tr < trSize; tr++) {
        transitions = realloc(transitions, size == 0 ? (size + 2) * sizeof(int) : (size + 1) * sizeof(int));
        if(in[tr] == outTmp && read[tr] == tape[pointer]) {
            transitions[size + 1] = tr;
            size++;
        }
        transitions[0] = size;
    }

    return transitions;
}

void handler() {
    Tape tapeCpy;
    /*char* chunk = getChunk(tape.pointer);
    size_t len = strlen(chunk);
    tapeCpy.size = len;
    tapeCpy.tape = malloc(len);
    strcpy(tapeCpy.tape, chunk);
    tapeCpy.pointer = tape.pointer;
    free(chunk);*/
    tapeCpy.size = tape.size;
    tapeCpy.tape = malloc(tapeCpy.size + 1);
    strcpy(tapeCpy.tape, tape.tape);
    tapeCpy.pointer = tape.pointer;

    long step = 1;
    while(1) {
        if(step > max) { //TODO Verify if max step must be run or not
            printf("U\n");
            return;
        }

        if(DEBUG) printf("Step: %ld\n", step);

        int* transitions = findTransitions(tapeCpy.tape, tapeCpy.pointer);
        if(transitions[0] == 0) {
            printf("%i\n", 0);
            return;
        }
        else if(transitions[0] == 1) {
            int tr = transitions[1];

            if(DEBUG) printf("Tape: %s\tPointer:%ld\tSize:%ld\n", tapeCpy.tape, tapeCpy.pointer, tapeCpy.size);
            if(DEBUG) printf("Transition {in=%i, read=%c, write=%c, move=%c, out=%i}\n", in[tr], read[tr], write[tr], move[tr], out[tr]);

            if(isAcc(out[tr])) {
                printf("%i\n", 1);
                return;
            }

            tapeCpy.tape[tapeCpy.pointer] = write[tr];
            outTmp = out[tr];

            if(move[tr] == 'R') {
                if(tapeCpy.pointer == tapeCpy.size - 1) {
                    tapeCpy.pointer = tapeCpy.pointer + 1;
                    char* newTape = malloc(tapeCpy.size + SIZE);
                    strcpy(newTape, tapeCpy.tape);
                    if(isLast(tapeCpy.pointer)) {
                        newTape = cleaner(newTape, tapeCpy.pointer);
                    } else {
                        strcat(newTape, getChunk(tapeCpy.pointer));
                    }
                    /*if (isInTheMiddle(tapeCpy.pointer) == 0) {
                        newTape = cleaner(newTape, tapeCpy.pointer);
                    } else {
                        //memcpy(newTape + tapeCpy.size, getChunk(tapeCpy.size), SIZE);
                        strcat(newTape, getChunk(tapeCpy.pointer));
                    }*/

                    tapeCpy.size = tapeCpy.size + SIZE;
                    tapeCpy.tape = realloc(tapeCpy.tape, tapeCpy.size);
                    strcpy(tapeCpy.tape, newTape);
                    free(newTape);
                } else {
                    tapeCpy.pointer = tapeCpy.pointer + 1;
                }

            }
            else if(move[tr] == 'L') {
                if(tapeCpy.pointer == 0) {
                    char* newTape = malloc(tapeCpy.size + SIZE);
                    memcpy(newTape + SIZE, tapeCpy.tape, tapeCpy.size);
                    newTape = cleaner(newTape, 0);
                    tapeCpy.size = tapeCpy.size + SIZE;
                    tapeCpy.tape = realloc(tapeCpy.tape, tapeCpy.size + 1);
                    strcpy(tapeCpy.tape, newTape);
                    tapeCpy.pointer = tapeCpy.pointer + SIZE - 1;
                    offset += SIZE;
                    free(newTape);
                } else {
                    tapeCpy.pointer = tapeCpy.pointer - 1;
                }
            }
        }
        if(DEBUG) printf("Tape: %s\tPointer:%ld\tSize:%ld\n", tapeCpy.tape, tapeCpy.pointer, tapeCpy.size);

        if(DEBUG) printf("________________________________________________________________________________________________________________________________\n");

        step++;
    }
}

int main() {
    in = malloc(INT);
    read = malloc(1);
    write = malloc(1);
    move = malloc(1);
    out = malloc(INT);
    acc = malloc(INT);

    char* input;
    scanf("%ms", &input);
    if(strcmp(input, "tr\n")) {
        free(input);

        int inTmp;
        char readTmp;
        char writeTmp;
        char moveTmp;
        int outTmp;
        int i = 0;

        while(scanf("%d %c %c %c %d", &inTmp, &readTmp, &writeTmp, &moveTmp, &outTmp)) {
            in[i] = inTmp;
            read[i] = readTmp;
            write[i] = writeTmp;
            move[i] = moveTmp;
            out[i] = outTmp;

            i++;

            in = realloc(in, (i + 2) * INT);
            read = realloc(read, (i + 2) * INT);
            write = realloc(write, (i + 2) * INT);
            move = realloc(move, (i + 2) * INT);
            out = realloc(out, (i + 2) * INT);
        }

        trSize = i;
    }

    scanf("%ms", &input);
    if(strcmp(input, "acc\n")) {
        free(input);

        int accTmp;
        int i = 0;

        while(scanf("%d", &accTmp)) {
            acc[i] = accTmp;
            i++;
            acc = realloc(acc, (i + 2) * INT);
        }

        accSize = i;
    }

    scanf("%ms", &input);
    if(strcmp(input, "max\n")) {
        free(input);
        scanf("%ld", &max);
    }

    //printf("\n");
    //printStatus();

    scanf("%ms", &input);
    if(strcmp(input, "run\n")) {
        free(input);
        while(scanf("%ms", &input) == 1) {
            size_t len = strlen(input);
            tape.size = len;
            tape.tape = malloc(len);
            strcpy(tape.tape, input);
            tape.pointer = 0;
            free(input);

            //printf("Tape {tape=%s, size=%ld, pointer=%ld}\n", tape.tape, tape.size, tape.pointer);
            //printf("________________________________________________________________________________________________________________________________\n");
            handler();
            //printf("\n________________________________________________________________________________________________________________________________\n\n");

            outTmp = 0;
            offset = 0;
            free(tape.tape);
            //return 0;
        }
    }

    return 0;
}
