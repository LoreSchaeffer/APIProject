#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

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
    char* tape;
    size_t size;
    size_t pointer;
    int out;
    size_t offset;
    long step;
} Machine;

//Global vars
char* tape;
size_t tapeSize = 0;
int lastId = 0;
Machine* machines;

void addMachine(Machine machine) {
    machines = realloc(machines, sizeof(machines) + sizeof(machine));
    machines[lastId] = machine;
}

void removeLastMachine() {
    size_t size = sizeof(machines[lastId]);
    free(machines[lastId].tape);
    machines = realloc(machines, sizeof(machines) - size);
    lastId--;
}

void printMachine(int id) {
    printf("Machine: {id=%i, tape=%s, size=%ld, pointer=%ld, out=%i, offset=%ld, step=%ld}\n", machines[id].id, machines[id].tape, machines[id].size, machines[id].pointer, machines[id].out, machines[id].offset, machines[id].step);
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
    pointer = pointer - offset;
    if(pointer == tapeSize - 1) return 1;
    return 0;
}

int isAcc(int out) {
    for(int i = 0; i < accSize; i++) {
        if(out == acc[i]) return 1;
    }
    return 0;
}

int* findTransitions(int out, char c) {
    int* transitions = malloc(sizeof(int));
    int size = 0;

    for(int tr = 0; tr < trSize; tr++) {
        if(input[tr] == out && read[tr] == c) {
            if(size != 0) transitions = realloc(transitions, (size_t) ((size + 1) * sizeof(int)));
            transitions[size + 1] = tr;
            size++;
        }
        transitions[0] = size;
    }

    return transitions;
}

Machine getMachine(int id) {
    Machine machine;
    machine.id = lastId;
    machine.tape = malloc(machines[id].size);
    strcpy(machine.tape, machines[id].tape);
    machine.size = machines[id].size;
    machine.pointer = machines[id].pointer;
    machine.out = machines[id].out;
    machine.offset = machines[id].offset;
    machine.step = machines[id].step;

    return machine;
}

void freeMachine(int id) {
    Machine machine = machines[id];
    free(machine.tape);
}

void freeMachines() {
    for(int i = 0; i <= lastId; i++) {
        freeMachine(i);
        free(machines);
    }
}

int runnable() {
    while (lastId >= 0) {
        //printMachines();

        if(machines[lastId].step > max) return -1;

        int* transitions = findTransitions(machines[lastId].out, machines[lastId].tape[machines[lastId].pointer]);
        if(transitions[0] == 0) {
            removeLastMachine();
        } else {
            for(int i = 1; i <= transitions[0]; i++) {
                if(transitions[0] == 1) {
                    machines[lastId].tape[machines[lastId].pointer] = write[transitions[i]];
                    machines[lastId].out = output[transitions[i]];
                    machines[lastId].step++;

                    if(isAcc(machines[lastId].out)) return 1;

                    if(move[transitions[i]] == 'R') {
                        if(machines[lastId].pointer == machines[lastId].size - 1) {
                            machines[lastId].size += SIZE;
                            char* newTape = malloc(machines[lastId].size);
                            strcpy(newTape, machines[lastId].tape);
                            if(isLast(machines[lastId].pointer, machines[lastId].offset)) {
                                newTape = cleaner(newTape, machines[lastId].pointer + 1);
                            } else {
                                strcat(newTape, getChunk(machines[lastId].pointer + 1, machines[lastId].offset));
                            }
                            machines = realloc(machines, sizeof(machines) + SIZE);
                            machines[lastId].tape = realloc(machines[lastId].tape, machines[lastId].size);
                            strcpy(machines[lastId].tape, newTape);
                            free(newTape);
                        }
                        machines[lastId].pointer++;
                    }
                    else if(move[transitions[i]] == 'L') {
                        if(machines[lastId].pointer == 0) {
                            machines[lastId].size += SIZE;
                            char* newTape = malloc(machines[lastId].size);
                            memcpy(newTape + SIZE, machines[lastId].tape, machines[lastId].size - SIZE);
                            newTape = cleaner(newTape, 0);
                            machines = realloc(machines, sizeof(machines) + SIZE);
                            machines[lastId].tape = realloc(machines[lastId].tape, machines[lastId].size);
                            strcpy(machines[lastId].tape, newTape);
                            free(newTape);
                            machines[lastId].pointer = SIZE - 1;
                            machines[lastId].offset += SIZE;
                        } else {
                            machines[lastId].pointer--;
                        }
                    }
                    machines[lastId] = machines[lastId];

                } else {
                    Machine child = machines[machines[lastId].id];
                    lastId++;
                    child.id = lastId;
                    child.tape[child.pointer] = write[transitions[i]];
                    child.out = output[transitions[i]];
                    child.step++;

                    if(isAcc(child.out)) return 1;

                    if(move[transitions[i]] == 'R') {
                        if(child.pointer == child.size - 1) {
                            child.size += SIZE;
                            char* newTape = malloc(child.size);
                            strcpy(newTape, child.tape);
                            if(isLast(child.pointer, child.offset)) {
                                newTape = cleaner(newTape, child.pointer + 1);
                            } else {
                                strcat(newTape, getChunk(child.pointer + 1, child.offset));
                            }
                            child.tape = realloc(child.tape, child.size);
                            strcpy(child.tape, newTape);
                            free(newTape);
                        }
                        child.pointer++;
                    }
                    else if(move[transitions[i]] == 'L') {
                        if(child.pointer == 0) {
                            child.size += SIZE;
                            char* newTape = malloc(child.size);
                            memcpy(newTape + SIZE, child.tape, child.size - SIZE);
                            newTape = cleaner(newTape, 0);
                            child.tape = realloc(child.tape, child.size);
                            strcpy(child.tape, newTape);
                            free(newTape);
                            child.pointer = SIZE - 1;
                            child.offset += SIZE;
                        } else {
                            child.pointer--;
                        }
                    }

                    addMachine(child);
                }
            }
        }

    }
    return 0;
}

void handler() {
    Machine machine;
    machine.id = lastId;
    machine.tape = malloc(SIZE);
    strcpy(machine.tape, getChunk(0, 0));
    machine.size = tapeSize;
    machine.pointer = 0;
    machine.out = 0;
    machine.offset = 0;
    machine.step = 1;

    machines = malloc(sizeof(Machine));
    machines[lastId] = machine;

    int response = runnable();

    if(response == 0) {
        printf("0\n");
    }
    else if(response == 1) {
        printf("1\n");
    }
    else if(response == -1) {
        printf("U\n");
    }

    freeMachines();
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
    }

    scanf("%ms", &inputTmp);
    if(strcmp(inputTmp, "run\n")) {
        free(inputTmp);
        while(scanf("%ms", &inputTmp) == 1) {
            tapeSize = strlen(inputTmp);
            tape = realloc(NULL, tapeSize);
            strcpy(tape, inputTmp);
            free(inputTmp);

            //printf("\n");
            //printf("Tape {tape=%s, size=%ld}\n", tape, tapeSize);
            //printf("________________________________________________________________________________________________________________________________\n");
            handler();
            //printf("\n________________________________________________________________________________________________________________________________\n\n");

            free(tape);
            //return 0;
        }
    }

    return 0;
}
