#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

//TODO Liberare memoria degli array

int* statoIn;
char* letto;
char* scritto;
char* movimento;
int* statoOut;
int* acc;
long max;

void print() {
    printf("\ntr\n");
    for(int i = 0; i < letto[i] != '\0'; i++) {
        printf("\t- %i %c %c %c %i\n", statoIn[i], letto[i], scritto[i], movimento[i], statoOut[i]);
    }
    printf("acc\n");
    for(int i = 0; acc[i] != '\0'; i++) {
        printf("\t- %i\n", acc[i]);
    }
    printf("max\n%ld", max);
}

void todo(char* run) {
    
}

int main() {
    char* input;

    /*statoIn = malloc(sizeof(int));
    letto = malloc(sizeof(char));
    scritto = malloc(sizeof(char));
    movimento = malloc(sizeof(char));
    statoOut = malloc(sizeof(int));*/
    acc = malloc(sizeof(int));
    statoIn = malloc(50 * sizeof(int));
    letto = malloc(50 * sizeof(char));
    scritto = malloc(50 * sizeof(char));
    movimento = malloc(50 * sizeof(char));
    statoOut = malloc(50 * sizeof(int));

    scanf("%ms", &input);
    if(strcmp(input, "tr\n")) {
        free(input);
        int p1;
        char p2;
        char p3;
        char p4;
        int p5;
        int i = 0;
        while (scanf("%d %c %c %c %d", &p1, &p2, &p3, &p4, &p5)) {
            //printf("tr: %i, %c, %c, %c, %i\n", p1, p2, p3, p4, p5);
            statoIn[i] = p1;
            letto[i] = p2;
            scritto[i] = p3;
            movimento[i] = p4;
            statoOut[i] = p5;
            i++;
            statoIn = (int *) realloc(statoIn, (i + 2) * sizeof(int));
            letto = (char *) realloc(letto, (i + 2) * sizeof(char));
            scritto = (char *) realloc(scritto, (i + 2) * sizeof(char));
            movimento = (char *) realloc(movimento, (i + 2) * sizeof(char));
            statoOut = (int *) realloc(statoOut, (i + 2) * sizeof(int));
        }
    }

    scanf("%ms", &input);
    if(strcmp(input, "acc\n")) {
        free(input);
        int p1;
        int i = 0;
        while (scanf("%d", &p1)) {
            acc[i] = p1;
            i++;
            acc = (int *) realloc(acc, (i + 2) * sizeof(int));
        }
    }

    scanf("%ms", &input);
    if(strcmp(input, "max\n")) {
        free(input);
        scanf("%ld", &max);
    }
    
    //print();
    
    scanf("%ms", &input);
    if(strcmp(input, "run\n")) {
        free(input);
        char* run;
        while (scanf("%ms", &run)) {
            todo(run);
        }
    }
    return 0;
}