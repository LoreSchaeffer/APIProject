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

//Structs
typedef struct Nastro {
    size_t len;
    char* nastro;
    int puntatore;
} Nastro;

/*typedef struct Passo {
    size_t len;
    char* nastro;
    int puntatore;
    int tr;
    int statoOutTmp;
};*/

//Variabili globali
Nastro nastro;
int statoOutTmp = 0;
size_t puntatoreGlobale = 0;

char* ottieniFrammento(size_t puntatore) {
    puntatore = puntatore - puntatoreGlobale;
    char* tmp = malloc(SIZE + 1);
    memcpy(tmp, nastro.nastro + puntatore, SIZE);
    tmp[SIZE] = '\0';
    if(puntatore + SIZE > nastro.len) {
        for(size_t i = nastro.len; i < puntatore + SIZE; i++) {
            tmp[i] = '_';
        }
    }
    printf("ottieniFrammento: %s, puntatore: %ld\n", tmp, puntatore);
    return tmp;
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
    for(size_t i = min; i < min + SIZE; i++) {
        //printf("Sbianchina: %s\n", nastro);
        nastro[i] = '_';
    }
    //printf("Sbianchina: %s\n", nastro);
    return nastro;
}

void printTr() {
    for(int i = 0; i < trSize; i++) {
        printf("in: %d, rd: %c, wr: %c, mv: %c, out: %d\n", statoIn[i], letto[i], scritto[i], movimento[i], statoOut[i]);
    }
}

int* trovaPassi(char* nastro, size_t puntatore) {
    int* passi = malloc(1024); //TODO Verificare grandezza in base ai test
    int i = 0;
    for(int tr = 0; tr < trSize; tr++) {
        if(statoIn[tr] == statoOutTmp && letto[tr] == nastro[puntatore]) {
            passi[i + 1] = tr;
            i++;
            passi[0] = i;
        }
    }

    return passi;
}

void gestore() {
    Nastro subnastro;
    char* tmp = ottieniFrammento(0);
    size_t len = strlen(tmp);
    subnastro.len = len;
    subnastro.nastro = malloc(len);
    strcpy(subnastro.nastro, tmp);
    subnastro.puntatore = 0;
    free(tmp);

    long p = 1;
    while(1) {
        printf("Nastro: %s\n", subnastro.nastro);
        printf("Passo: %ld, Puntatore: %i, Carattere: %c, Len: %ld\n", p, subnastro.puntatore, subnastro.nastro[subnastro.puntatore], subnastro.len);
        if(p >= max) {
            printf("U\n");
            return;
        }
        int* passi = trovaPassi(subnastro.nastro, subnastro.puntatore);
        if(passi[0] == 0) {
            printf("%i\n", 0);
            return;
        }
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
                    strcpy(nastroTmp, subnastro.nastro);
                    if(!isInMezzo(subnastro.puntatore)) {
                        nastroTmp = sbianchina(nastroTmp, subnastro.puntatore);
                    } else {
                        nastroTmp = patch(subnastro.nastro, ottieniFrammento(subnastro.puntatore + 1), subnastro.puntatore, subnastro.len + SIZE);
                    }
                    printf("SubTmp: %s\n", nastroTmp);
                    subnastro.len = subnastro.len + SIZE;
                    subnastro.nastro = realloc(subnastro.nastro, subnastro.len);
                    strcpy(subnastro.nastro, nastroTmp);
                    subnastro.puntatore = subnastro.puntatore + 1;
                } else {
                    subnastro.puntatore = subnastro.puntatore + 1;
                }
            }
            else if(movimento[tr] == 'L') {
                if(subnastro.puntatore == 0) {
                    char* nastroTmp = malloc(subnastro.len + SIZE);
                    memcpy(nastroTmp + SIZE, subnastro.nastro, subnastro.len);
                    nastroTmp = sbianchina(nastroTmp, 0);
                    subnastro.len = subnastro.len + SIZE;
                    subnastro.nastro = realloc(subnastro.nastro, subnastro.len);
                    strcpy(subnastro.nastro, nastroTmp);
                    subnastro.puntatore = subnastro.puntatore + SIZE - 1;
                    puntatoreGlobale += SIZE;
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
        //printf("________________________________________________________________________________________________________________________________\n");
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
            nastro.len = len;
            nastro.nastro = malloc(len);
            strcpy(nastro.nastro, tmp);
            free(tmp);
            nastro.puntatore = 0;
            printf("%s,\t%ld,\t%i\n",nastro.nastro, len, nastro.puntatore);
            //printf("\n"); //TODO Remove this
            gestore();
            statoOutTmp = 0;
            puntatoreGlobale = 0;
            printf("\n\n\n");
            return 0;
        }
        printf("out while\n");
    }
    return 0;
}
