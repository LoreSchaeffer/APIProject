#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 64

//Transitions
int* input;
char* read;
char* write;
char* move;
int* output;
int trSize = 0;

//Params TM
int* acc;
int accSize = 0;
long max;

//Structs
typedef struct Machine {
    int id;
    size_t size;
    size_t pointer;
    int out;
    size_t offset;
    long step;
} Machine;

//Global vars
char* tape;
size_t tapeSize = 0;
Machine* machines;
int machinesSize = 1;
int lastId = 0;
char** tapes;
int tapesSize = 1;


void removeLastMachine() {
    if(lastId == 0) {
        machines = realloc(machines, sizeof(struct Machine));
    } else {
        machines = realloc(machines, machinesSize * sizeof(struct Machine));
    }
    free(tapes[lastId]);
    tapes = realloc(tapes, tapesSize * sizeof(char*));
    machinesSize--;
    tapesSize--;
}

void printMachine(int id) {
    printf("Machine: {id=%i, tape=%s, size=%ld, pointer=%ld, out=%i, offset=%ld, step=%ld}\n", machines[id].id, tapes[id], machines[id].size, machines[id].pointer, machines[id].out, machines[id].offset, machines[id].step);
}

void printMachines() {
    printf("\n________\tInizio\t________\n");
    for(int i = 0; i <= lastId; i++) printMachine(i);
    printf("________\tFine\t________\n");
}

char* getChunk(size_t pointer, size_t offset) {
    pointer = pointer - offset;
    char* chunk = malloc(SIZE + 1);
    memcpy(chunk, tape + pointer, SIZE);
    chunk[SIZE] = '\0';

    if(pointer + SIZE > tapeSize) {
        for(size_t p = tapeSize; p < pointer + SIZE; p++) {
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

int isLast(size_t pointer, size_t offset) {
    size_t gp = pointer - offset;
    if(gp == tapeSize - 1) return 1;
    return 0;
}

int isAcc(int out) {
    for(int i = 0; i < accSize; i++) {
        if(out == acc[i]) return 1;
    }
    return 0;
}

void moveRight(Machine machine) {
    machine.size += SIZE;
    machine.pointer++;
    char* tape = malloc(machine.size);
    strcpy(tape, tapes[machine.id]);

    if(isLast(machine.pointer, machine.offset)) {
        tape = cleaner(tape, machine.pointer);
    } else {
        char* tmp = getChunk(machine.pointer, machine.offset);
        strcat(tape, tmp);
        free(tmp);
    }

    tapes[machine.id] = realloc(tapes[machine.id], machine.size);
    strcpy(tapes[machine.id], tape);
    free(tape);

    machines[machine.id] = machine;
}

void moveLeft(Machine machine) {
    machine.size += SIZE;
    char* tape = malloc(machine.size);
    memcpy(tape + SIZE, tapes[machine.id], machine.size - SIZE);
    tape = cleaner(tape, 0);

    tapes[machine.id] = realloc(tapes[machine.id], machine.size);
    strcpy(tapes[machine.id], tape);
    free(tape);

    machine.pointer = SIZE - 1;
    machine.offset += SIZE;
    machines[machine.id] = machine;
}

int* findTransitions(int out, char c) {
    int* transitions = malloc(sizeof(int));
    int size = 0;

    for(int tr = 0; tr < trSize; tr++) {
        if(input[tr] == out && read[tr] == c) {
            if(size != 0) transitions = realloc(transitions, (size + 1) * sizeof(int));
            transitions[size + 1] = tr;
            size++;
        }
        transitions[0] = size;
    }

    return transitions;
}

int runnable() {
    while(lastId >= 0) {
        //printMachines();

        Machine current = machines[lastId];
        char* currentTape = tapes[current.id];

        if(current.step > max) {
            if(machinesSize > 1) {
                removeLastMachine();
                lastId--;
                continue;
            } else {
                return -1;
            }
        }

        int* transitions = findTransitions(current.out, tapes[current.id][current.pointer]);
        if(transitions[0] == 0) {
            removeLastMachine();
            lastId--;
        }
        else if(transitions[0] == 1) {
            int tr = transitions[1];
            //printf("Transition {in=%i, read=%c, write=%c, move=%c, out=%i}\n", input[tr], read[tr], write[tr], move[tr], output[tr]);
            tapes[current.id][current.pointer] = write[tr];
            current.out = output[tr];
            current.step++;

            if(isAcc(current.out)) {
                return 1;
            }

            if(move[tr] == 'R') {
                if(current.pointer == current.size - 1) {
                    moveRight(current);
                } else {
                    current.pointer++;
                    machines[current.id] = current;
                }
            }
            else if(move[tr] == 'L') {
                if(current.pointer == 0) {
                    moveLeft(current);
                } else {
                    current.pointer--;
                    machines[current.id] = current;
                }
            } else {
                machines[current.id] = current;
            }
        } else {
            lastId--;
            machinesSize--;
            tapesSize--;
            for(int i = 1; i <= transitions[0]; i++) {
                int tr = transitions[i];
                //printf("Transition {in=%i, read=%c, write=%c, move=%c, out=%i}\n", input[tr], read[tr], write[tr], move[tr], output[tr]);
                machinesSize++;
                tapesSize++;
                lastId++;

                tapes = realloc(tapes, (tapesSize + 1) * sizeof(char*));
                machines = realloc(machines, (machinesSize + 1) * sizeof(struct Machine));

                Machine child = current;
                child.id = lastId;
                tapes[child.id] = malloc(child.size);
                strcpy(tapes[child.id], currentTape);
                tapes[child.id][child.pointer] = write[tr];
                child.out = output[tr];
                child.step++;

                if(isAcc(child.out)) {
                    return 1;
                }

                if(move[tr] == 'R') {
                    if(child.pointer == child.size - 1) {
                        moveRight(child);
                    } else {
                        child.pointer++;
                        machines[child.id] = child;
                    }
                }
                else if(move[tr] == 'L') {
                    if(child.pointer == 0) {
                        moveLeft(child);
                    } else {
                        child.pointer--;
                        machines[child.id] = child;
                    }
                } else {
                    machines[child.id] = child;
                }
            }
        }
    }
    return 0;
}

void handler() {
    tapes = malloc(sizeof(char*));
    machines = malloc(sizeof(struct Machine));

    Machine original;
    original.id = lastId;

    tapes[original.id] = malloc(SIZE);
    char* tmp = getChunk(0, 0);
    strcpy(tapes[original.id], tmp);
    free(tmp);

    original.size = SIZE;
    original.pointer = 0;
    original.out = 0;
    original.offset = 0;
    original.step = 1;

    machines[lastId] = original;

    int response = runnable();

    if(response == 0) {
        printf("0\n");
    } else if(response == 1) {
        printf("1\n");
    } else if(response == -1) {
        printf("U\n");
    }

    for(int i = 0; i < tapesSize; i++) {
        free(tapes[i]);
    }

    free(machines);
    free(tapes);
    free(tape);
    tapeSize = 0;
    lastId = 0;
    tapesSize = 1;
    machinesSize = 1;
}

int main() {
    input = malloc(sizeof(int));
    read = malloc(1);
    write = malloc(1);
    move = malloc(1);
    output = malloc(sizeof(int));
    acc = malloc(sizeof(int));

    char* inputTmp;
    scanf("%ms", &inputTmp);
    if(strcmp(inputTmp, "tr\n")) {
        free(inputTmp);

        int inTmp;
        char readTmp;
        char writeTmp;
        char moveTmp;
        int outTmp;
        int i = 0;

        while(scanf("%d %c %c %c %d", &inTmp, &readTmp, &writeTmp, &moveTmp, &outTmp)) {
            input[i] = inTmp;
            read[i] = readTmp;
            write[i] = writeTmp;
            move[i] = moveTmp;
            output[i] = outTmp;

            i++;

            input = realloc(input, (size_t) ((i + 2) * sizeof(int)));
            read = realloc(read, (size_t) ((i + 2) * sizeof(int)));
            write = realloc(write, (size_t) ((i + 2) * sizeof(int)));
            move = realloc(move, (size_t) ((i + 2) * sizeof(int)));
            output = realloc(output, (size_t) ((i + 2) * sizeof(int)));
        }

        trSize = i;
    }

    scanf("%ms", &inputTmp);
    if(strcmp(inputTmp, "acc\n")) {
        free(inputTmp);

        int accTmp;
        int i = 0;

        while(scanf("%d", &accTmp)) {
            acc[i] = accTmp;
            i++;
            acc = realloc(acc, (size_t) ((i + 2) * sizeof(int)));
        }

        accSize = i;
    }

    scanf("%ms", &inputTmp);
    if(strcmp(inputTmp, "max\n")) {
        free(inputTmp);
        scanf("%ld", &max);
        //max = (long) (max * 0.8); //TODO remove this
    }

    scanf("%ms", &inputTmp);
    if(strcmp(inputTmp, "run\n")) {
        free(inputTmp);
        while(scanf("%ms", &inputTmp)) {
            tapeSize = strlen(inputTmp);
            tape = realloc(NULL, tapeSize);
            strcpy(tape, inputTmp);
            free(inputTmp);

            handler();
            //return 0;
        }
    }

    return 0;
}
