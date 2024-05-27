#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct jucator
{
    char *nume;
    char *prenume;
    int punctaj;
    struct jucator *next;
} jucator;
typedef struct echipa
{
    jucator *jucatori;
    int nr_jucatori;
    char *nume;
    struct echipa *next;
    float punctaj;
} echipa;
//lista
void adaugareJucator(jucator **head, char *nume, char *prenume, int punctaj);
void adaugareEchipa(echipa **head, jucator *jucator_nou, int nr_jucatori, char *nume);
void citireDinFisier(FILE *f_date, echipa **heade, int *nr_echipe);
void scriereInFisier(FILE *f_out, echipa *heade);
float calcPunctajMinim(echipa *heade);
void eliberareMemorieListeEchipe(echipa *e);
void eliminareEchipe(echipa **heade, int nr_echipe);
