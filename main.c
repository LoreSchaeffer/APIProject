#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define INT_SIZE 4
#define SIZE 128

//Transizioni
int* statoIn;
char* letto;
char* scritto;
char* movimento;
int* statoOut;

//Parametri macchina
int* acc;
long max;

//Variabili di nastroGlobale
char* nastroGlobale;
size_t lunghezza;
int passi;
int statoOutTmp = 0;
int puntatoreNastro = 0;
int* trPossibili;
int numeroPossibili = 0;
struct Passo* passaggi;

//Structs
typedef struct Passo {
    int passi;
    char* nastro;
    int puntatoreNastro;
    int numPossibili;
    int trCorrente;
    int statoOutTmp;
};

char* frammenta(char* nastro, int sezione) {
    int inizio;
    if(sezione == 0) {
        inizio = 0;
    } else {
        inizio = SIZE * sezione + 1;
    }
    int fine = inizio + SIZE;

    char* output = malloc(SIZE);

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
        if(puntatore < i * SIZE) {
            return frammenta(nastro, j);
        }
        j++;
    }
    return NULL;
}

char* sbianchina(char* nastro, int min, int max) {
    for(int i = min; i < max; i++) {
        nastro[i] = '_';
    }
    return nastro;
}

void aggiornaNastro(char* nastro) {
    free(nastroGlobale);
    nastroGlobale = malloc(sizeof(nastro));
    strcpy(nastroGlobale, nastro);
}

void transizioneDet(char* nastro, int puntatoreNastro, int det) {
    int tr = trPossibili[0];
    int len = (int) strlen(nastro);
    nastro[puntatoreNastro] = scritto[tr];

    if(movimento[tr] == 'R') {
        if(puntatoreNastro == len - 1) { //TODO Check if this character is '\0'
            if(det == 1) {
                nastro = (char*) realloc(nastro, lunghezza + SIZE);
                lunghezza += SIZE;
                nastro = sbianchina(nastro, puntatoreNastro, lunghezza);
                puntatoreNastro++;
                aggiornaNastro(nastro);
            } else {
                if(puntatoreNastro == lunghezza - 1) {
                    nastro = (char*) realloc(nastro, lunghezza + SIZE);
                    lunghezza += SIZE;
                    nastro = sbianchina(nastro, puntatoreNastro, lunghezza);
                    puntatoreNastro++;
                } else {
                    nastro = (char*) realloc(nastro, (size_t) (len + SIZE));
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
                nastro = (char*) realloc(nastro, lunghezza + SIZE);
                lunghezza += SIZE;
                memmove(nastro, nastro, lunghezza - 1);
                sbianchina(nastro, puntatoreNastro, SIZE);
                aggiornaNastro(nastro);
            } else {

            }
        }
    }

    if(movimento[tr] == 'L' && puntatoreNastro == 0) {
        if(det == 1) {
            nastro = (char*) realloc(nastro, (lunghezza + 1));
            lunghezza++;
            memmove(nastro, nastro, (size_t) (lunghezza - 1));
            aggiornaNastro(nastro);
        } else {
            if(puntatoreNastro == 0) {

            }
        }
    }
}

void transizioneNonDet(char* nastro, int det) {

}

void trovaPassi(char* nastro, int det) {
    int trovato = 0;

    for(int i = 0; i < sizeof(statoIn); i++) {
        if(statoIn[i] == statoOutTmp) {
            if(letto[i] == nastro[puntatoreNastro]) {
                trovato = 1;
                passi++;
                trPossibili[numeroPossibili] = i;
                numeroPossibili++;
                /*struct Passo passo = {
                        passi,
                        nastroGlobale,
                        puntatoreNastro,
                        0,
                        i,
                        statoOutTmp
                };
                passaggi = (struct Passo*) realloc(passaggi, sizeof(passaggi) + sizeof(passaggi));
                passaggi*/
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
    /*if(passi > max) {
        printf("U\n");
        return;
    }*/
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

        while (scanf("%ms", &nastroGlobale)) {
            lunghezza = strlen(nastroGlobale);
            passi = 0;
            free(trPossibili);
            trPossibili = (int*) malloc(INT_SIZE);
            trovaPassi(nastroGlobale, 1);
            free(nastroGlobale);
        }
    }
    return 0;
}
