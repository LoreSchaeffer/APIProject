#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_SIZE 4
#define SIZE 10

//Transizioni
int* statoIn;
char* letto;
char* scritto;
char* movimento;
int* statoOut;
int trSize = 0;

//Parametri macchina
int* acc;
int accSize = 0;
long max;

//Variabili globali
struct Nastro nastro;
int statoOutTmp = 0;
size_t puntatoreGlobale = 0;

//Structs
typedef struct Nastro {
    size_t len;
    char* nastro;
    int puntatore;
};

typedef struct Passo {
    size_t len;
    char* nastro;
    int puntatore;
    int tr;
    int statoOutTmp;
};

char* substring(size_t inizio) { //TODO Si potrebbe rimuovere inizio nalla lunghezza in allocazione mem
    char* tmp = malloc(nastro.len);
    memcpy(tmp, nastro.nastro + inizio, nastro.len);
    tmp = (char *) realloc(tmp, SIZE);
    size_t len = strlen(tmp);
    if(len < SIZE) {
        for(size_t i = len; i < SIZE; i++) {
            tmp[i] = '_';
        }
    }
    //printf("Substring: %s", tmp);
    return tmp;
}

char* ottieniFrammento(size_t puntatore) {
    puntatore = puntatore + puntatoreGlobale;
    size_t j = 0;
    for(size_t i = 1; i < nastro.len; i++) {
        if(puntatore < i * SIZE) {
            return substring(j * SIZE);
        }
        j++;
    }
    return NULL;
}

char* patch(char* nastro, char* patch, size_t puntatore, size_t len) {
    size_t j = 0;
    for(size_t i = puntatore; i < len; i++) {
        nastro[i] = patch[j];
        j++;
    }
    return nastro;
}

int isAccettazione(int statoOut) {
    for(int i = 0; i < accSize; i++) {
        if(statoOut == acc[i]) return 1;
    }
    return 0;
}

int isInMezzo(int puntatore) {
    if(puntatore <= SIZE) return 0;
    if(puntatore > (nastro.len / SIZE) * SIZE) return 0;
    return 1;
}

char* sbianchina(char* nastro, size_t min) {
    for(size_t i = min; i <= min + SIZE - 1; i++) {
        //printf("Sbianchina: %s\n", nastro);
        nastro[i] = '_';
    }
    //printf("Sbianchina: %s\n", nastro);
    return nastro;
}

int* trovaPassi(struct Nastro subnastro) {
    int* passi = malloc(1024); //TODO Verificare grandezza in base ai test
    int i = 0;
    for(int tr = 0; tr < trSize; tr++) {
        if(statoIn[tr] == statoOutTmp && letto[tr] == subnastro.nastro[subnastro.puntatore]) {
            passi[i + 1] = tr;
            i++;
            passi[0] = i;
        }
    }

    return passi;
}

void gestore() {
    char* tmp = ottieniFrammento(0);
    struct Nastro subnastro = {strlen(tmp), tmp, 0};
    free(tmp);

    long p = 1;
    while(1) {
        printf("Nastro: %s\n", subnastro.nastro);
        printf("Passo: %ld, Puntatore: %i, Carattere: %c, Len: %ld\n", p, subnastro.puntatore, subnastro.nastro[subnastro.puntatore], subnastro.len);
        if(p >= max) {
            printf("U\n");
            return;
        }
        int* passi = trovaPassi(subnastro);
        if(passi[0] == 1) {
            int tr = passi[1];
            printf("Transizione {Riga: %i, Scritto: %c Movimento: %c, StatoOut: %i}\n", tr + 2, scritto[tr], movimento[tr], statoOut[tr]);

            if(isAccettazione(statoOut[tr])) {
                printf("%i\n", 1);
                return;
            }

            subnastro.nastro[subnastro.puntatore] = scritto[tr];
            statoOutTmp = statoOut[tr];

            if(movimento[tr] == 'R') {
                if(subnastro.puntatore == subnastro.len - 1) {
                    char* nastroTmp = malloc(subnastro.len + SIZE);
                    nastroTmp = subnastro.nastro;
                    if(!isInMezzo(subnastro.puntatore)) {
                        nastroTmp = sbianchina(nastroTmp, subnastro.puntatore);
                    } else {
                        nastroTmp = patch(subnastro.nastro, ottieniFrammento(subnastro.puntatore), subnastro.puntatore, subnastro.len + SIZE);
                    }
                    struct Nastro subnastroTmp = {subnastro.len + SIZE, nastroTmp, subnastro.puntatore + 1};
                    subnastro = subnastroTmp;
                    /*subnastro.nastro = (char *) realloc(subnastro.nastro, subnastro.len + SIZE);
                    subnastro.len = subnastro.len + SIZE;
                    subnastro.puntatore = subnastro.puntatore + 1;
                    if(!isInMezzo(subnastro.puntatore)) {
                        subnastro.nastro = sbianchina(subnastro.nastro, subnastro.puntatore);
                    } else {
                        subnastro.nastro = patch(subnastro.nastro, ottieniFrammento(subnastro.puntatore), subnastro.puntatore, subnastro.len);
                    }*/
                } else {
                    subnastro.puntatore = subnastro.puntatore + 1;
                }
            }
            else if(movimento[tr] == 'L') {
                if(subnastro.puntatore == 0) {
                    char* nastroTmp = malloc(subnastro.len + SIZE);
                    memcpy(nastroTmp + SIZE, subnastro.nastro, subnastro.len);
                    nastroTmp = sbianchina(nastroTmp, 0);
                    struct Nastro subnastroTmp = {subnastro.len + SIZE, nastroTmp, subnastro.puntatore + SIZE - 1};
                    subnastro = subnastroTmp;
                    puntatoreGlobale += SIZE;
                    /*subnastro.nastro = (char *) realloc(subnastro.nastro, subnastro.len + SIZE);
                    subnastro.len = subnastro.len + SIZE;
                    memmove(subnastro.nastro + SIZE + 1, subnastro.nastro, subnastro.len - SIZE);
                    subnastro.nastro = sbianchina(subnastro.nastro, 0);
                    subnastro.puntatore = subnastro.puntatore + SIZE;
                    puntatoreGlobale += SIZE;*/
                } else {
                    subnastro.puntatore = subnastro.puntatore - 1;
                }
            }
        }
        /*for(int i = 1; i <= passi[0]; i++) {
            int tr = passi[i];
            if(isAccettazione(tr) == 1) {
                printf("%i\n", 1);
                return;
            }
            struct Passo passo = {subnastro.len, subnastro.nastro, subnastro.puntatore, tr, statoOutTmp};
        }*/
        p++;
        printf("________________________________________________________________________________________________________________________________\n");
    }
}

int main() {
    statoIn = malloc(INT_SIZE);
    letto = malloc(1);
    scritto = malloc(1);
    movimento = malloc(1);
    statoOut = malloc(INT_SIZE);
    acc = malloc(INT_SIZE);

    char* input;

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
            trSize++;
            statoIn[i] = p1;
            letto[i] = p2;
            scritto[i] = p3;
            movimento[i] = p4;
            statoOut[i] = p5;
            i++;
            trSize++;

            statoIn = (int *) realloc(statoIn, (i + 2) * INT_SIZE);
            letto = (char *) realloc(letto, (i + 2));
            scritto = (char *) realloc(scritto, (i + 2));
            movimento = (char *) realloc(movimento, (i + 2));
            statoOut = (int *) realloc(statoOut, (i + 2) * INT_SIZE);
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
            accSize++;
            acc = (int *) realloc(acc, (i + 2) * INT_SIZE);
        }
    }

    scanf("%ms", &input);
    if(strcmp(input, "max\n")) {
        free(input);
        scanf("%ld", &max);
    }

    scanf("%ms", &input);
    if(strcmp(input, "run\n")) {
        free(input);

        char * tmp;
        while (scanf("%ms", &tmp)) {
            size_t len = strlen(tmp);
            tmp = (char*) realloc(tmp, len + 1);
            tmp[len] = '\0';
            struct Nastro n = {len, tmp, 0};
            nastro = n;
            statoOutTmp = 0;
            //printf("%s,\t%ld,\t%i\n",nastro.nastro, len, nastro.puntatore);
            printf("\n"); //TODO Remove this
            gestore();
            printf("\n\n\n");
            //return 0;
        }
    }
    return 0;
}
