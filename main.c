#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_DIMENSIONE 4
#define DIMENSIONE 128

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

char* frammenta(char* nastro, int sezione) {
    int inizio;
    if(sezione == 0) {
        inizio = 0;
    } else {
        inizio = DIMENSIONE * sezione + 1;
    }
    int fine = inizio + DIMENSIONE;

    char* output = malloc(DIMENSIONE);

    int j = 0;
    for(int i = inizio; i < fine; i++) {
        output[j] = nastro[i];
        j++;
    }
    return output;
}

char* ottieniFrammento(char* nastro, int puntatore) {
    int j = 0;
    for(int i = 1; i < strlen(nastro); i++) {
        if(puntatore < i * DIMENSIONE) {
            return frammenta(nastro, j);
        }
        j++;
    }
    return NULL;
}

int isAccettazione(int tr) {
    for(int i = 0; i < accSize; i++) {
        if(tr == acc[i]) return 1;
    }
    return 0;
}

/*char* sbianchina(char* nastro, int min, int max) {
    for(int i = min; i < max; i++) {
        nastro[i] = '_';
    }
    return nastro;
}

void aggiornaNastro(char* nastro) {
    free(nastroGlobale);
    nastroGlobale = malloc(sizeof(nastro));
    strcpy(nastroGlobale, nastro);
}*/
/*void transizioneDet(char* nastro, int puntatoreNastro, int det) {
    int tr = trPossibili[0];
    int len = (int) strlen(nastro);
    nastro[puntatoreNastro] = scritto[tr];

    if(movimento[tr] == 'R') {
        if(puntatoreNastro == len - 1) { //TODO Check if this character is '\0'
            if(det == 1) {
                nastro = (char*) realloc(nastro, lunghezza + DIMENSIONE);
                lunghezza += DIMENSIONE;
                nastro = sbianchina(nastro, puntatoreNastro, lunghezza);
                puntatoreNastro++;
                aggiornaNastro(nastro);
            } else {
                if(puntatoreNastro == lunghezza - 1) {
                    nastro = (char*) realloc(nastro, lunghezza + DIMENSIONE);
                    lunghezza += DIMENSIONE;
                    nastro = sbianchina(nastro, puntatoreNastro, lunghezza);
                    puntatoreNastro++;
                } else {
                    nastro = (char*) realloc(nastro, (DIMENSIONE_t) (len + DIMENSIONE));
                    puntatoreNastro++;
                    strcat(nastro, ottieniFrammento(nastroGlobale, puntatoreNastro));
                }
            }
        } else {
            puntatoreNastro++;
        }
    }
    else if(movimento[tr] == 'L') {
        if(puntatoreNastro == 0) {
            if(det == 1) {
                nastro = (char*) realloc(nastro, lunghezza + DIMENSIONE);
                lunghezza += DIMENSIONE;
                memmove(nastro, nastro, lunghezza - 1);
                sbianchina(nastro, puntatoreNastro, DIMENSIONE);
                aggiornaNastro(nastro);
            } else {

            }
        }
    }

    if(movimento[tr] == 'L' && puntatoreNastro == 0) {
        if(det == 1) {
            nastro = (char*) realloc(nastro, (lunghezza + 1));
            lunghezza++;
            memmove(nastro, nastro, (DIMENSIONE_t) (lunghezza - 1));
            aggiornaNastro(nastro);
        } else {
            if(puntatoreNastro == 0) {

            }
        }
    }
}*/
/*void trovaPassi(char* nastro, int det) {
    int trovato = 0;

    for(int i = 0; i < sizeof(statoIn); i++) {
        if(statoIn[i] == statoOutTmp) {
            if(letto[i] == nastro[puntatoreNastro]) {
                trovato = 1;
                passi++;
                trPossibili[numeroPossibili] = i;
                numeroPossibili++;
                struct Passo passo = {
                        passi,
                        nastroGlobale,
                        puntatoreNastro,
                        0,
                        i,
                        statoOutTmp
                };
                passaggi = (struct Passo*) realloc(passaggi, sizeof(passaggi) + sizeof(passaggi));
                passaggi
            }
        }
    }

    if(trovato == 0) {
        printf("%i\n", 0);
    }

    if(numeroPossibili == 1) {
        //transizioneDet(nastro, det);
    } else {
        transizioneNonDet(nastro, det);
    }

    //TODO Move this to struct
    if(passi > max) {
        printf("U\n");
        return;
    }
}*/

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
    char* tmp = ottieniFrammento(nastro.nastro, 0);
    struct Nastro subnastro = {strlen(tmp), tmp, 0};
    free(tmp);

    long p = 0;
    while(1) {
        if(p >= max) {
            printf("U\n");
            return;
        }
        int* passi = trovaPassi(subnastro);
        for(int i = 1; i <= passi[0]; i++) {
            int tr = passi[i];
            if(isAccettazione(tr) == 1) {
                printf("%i\n", 1);
                return;
            }
            struct Passo passo = {subnastro.len, subnastro.nastro, subnastro.puntatore, tr, statoOutTmp};
        }
        p++;
    }
}

int main() {
    statoIn = malloc(INT_DIMENSIONE);
    letto = malloc(1);
    scritto = malloc(1);
    movimento = malloc(1);
    statoOut = malloc(INT_DIMENSIONE);
    acc = malloc(INT_DIMENSIONE);

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

            statoIn = (int *) realloc(statoIn, (i + 2) * INT_DIMENSIONE);
            letto = (char *) realloc(letto, (i + 2));
            scritto = (char *) realloc(scritto, (i + 2));
            movimento = (char *) realloc(movimento, (i + 2));
            statoOut = (int *) realloc(statoOut, (i + 2) * INT_DIMENSIONE);
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
            trSize++;
            acc = (int *) realloc(acc, (i + 2) * INT_DIMENSIONE);
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
            gestore();
        }
    }
    return 0;
}
