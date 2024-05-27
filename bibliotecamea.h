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
typedef struct meci
{
    echipa *echipa1;
    echipa *echipa2;
    struct meci *next;
}meci;
typedef struct coada
{
    meci *front;
    meci *rear;
}coada;
typedef struct stiva
{
    echipa *echipe;
    struct stiva *next;
}stiva;
typedef struct top8
{
    echipa *castigatoare;
    struct top8 *next;
}top8;
typedef struct clasament
{
    echipa *echipe;
    struct clasament *left, *right;
}clasament;
typedef struct arbore
{
    int height;
    echipa *echipe;
    struct arbore *left, *right;
}arbore;
//lista
void adaugareJucator(jucator **head, char *nume, char *prenume, int punctaj);
void adaugareEchipa(echipa **head, jucator *jucator_nou, int nr_jucatori, char *nume);
void citireDinFisier(FILE *f_date, echipa **heade, int *nr_echipe);
void scriereInFisier(FILE *f_out, echipa *heade);
float calcPunctajMinim(echipa *heade);
void eliberareMemorieListeEchipe(echipa *e);
void eliminareEchipe(echipa **heade, int nr_echipe);
//coada
coada *creareCoada();
int isQueueEmpty(coada* q);
void adaugareUnMeciInCoada(coada *q, echipa *heade);
void afisareCuSpatii(FILE *f_out, echipa *e);
void afisareMeciuri(FILE *f_out, coada *q);
void deQueue(coada *q, echipa **d1, echipa **d2);
//stiva
int isStackEmpty(stiva *s);
void push(stiva **top_castigatori, echipa *head_echipa);
echipa *pop(stiva **s);
void aflareCastigatoriSiNecastigatori(stiva **top_castigatori, stiva **top_necastigatori, coada *q);
int nrEchipe(echipa *heade);
void initializareCoada(coada **q, echipa *heade);
void adaugareInLista8(top8 **head, echipa *castigatoare);
void afisareEchipaCastigatoare(FILE *f_out, stiva **top_castigatori, stiva **top_necastigatori, coada *q, echipa *heade, top8 **lista8);
//bst
clasament *nodNou(echipa *echipa_noua);
int isTreeEmpty(clasament *root);
clasament *insert(clasament *nod, echipa *key);
void inorder(FILE *f_out, clasament *root);
void afisareBST(FILE *f_out, top8 *lista8, clasament **root);
//avl
int nodeHeight(arbore *root);
arbore *leftRotation(arbore *z);
arbore *rightRotation(arbore *z);
arbore *LRRotation(arbore *Z);
arbore *RLRotation(arbore *Z);
arbore *insertArbore(arbore *nod, echipa *nou);
int max(int a, int b);
void adaugareDinBSTinLista(clasament *root, echipa **head);
void printLevel(FILE *f_out, arbore *root, int level);
void afisareAVL(FILE *f_out, arbore *avl, echipa *head, clasament *root);