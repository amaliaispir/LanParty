#include "bibliotecamea.h"

//liste
void adaugareJucator(jucator **head, char *nume, char *prenume, int punctaj)
{
    jucator *jucator_nou = (jucator*)malloc(sizeof(jucator));
    jucator_nou->nume = (char*)malloc((strlen(nume) + 1) * sizeof(char));
    jucator_nou->prenume = (char*)malloc((strlen(prenume) + 1) * sizeof(char));
    strcpy(jucator_nou->nume, nume);
    strcpy(jucator_nou->prenume, prenume);
    jucator_nou->punctaj = punctaj;
    jucator_nou->next = *head;
    *head = jucator_nou;
}
void adaugareEchipa(echipa **head, jucator *jucator_nou, int nr_jucatori, char *nume)
{
    echipa *echipa_noua = (echipa*)malloc(sizeof(echipa));
    echipa_noua->jucatori = jucator_nou;
    echipa_noua->nr_jucatori = nr_jucatori;
    echipa_noua->nume = (char*)malloc((strlen(nume) + 1) * sizeof(char));
    strcpy(echipa_noua->nume, nume);
    echipa_noua->next = *head;
    *head = echipa_noua;
}
void citireDinFisier(FILE *f_date, echipa **heade, int *nr_echipe)
{
    fscanf(f_date, "%d", nr_echipe);
    for(int i = 0; i < (*nr_echipe); i++)
    {
        //citesc echipele si le adaug in lista de echipe
        jucator *headj = NULL;
        int nr_jucatori;
        int punctaj;
        char *nume_echipa;
        nume_echipa = (char*)malloc(100 * sizeof(char));
        fscanf(f_date, "%d ", &nr_jucatori);
        fgets(nume_echipa,100,f_date);
        nume_echipa[strlen(nume_echipa) - 2] = '\0'; //are caracterul '\n' la final si tb sters
        if(nume_echipa[strlen(nume_echipa) - 1] ==  ' ') //sterg spatii de la finalul numelor
            nume_echipa[strlen(nume_echipa) - 1]  = '\0';
        adaugareEchipa(heade, headj, nr_jucatori, nume_echipa);
        for(int j = 0; j < nr_jucatori; j++)
        {
            //citesc jucatorii din fisier si ii adaug in lista de jucatori
            char *nume, *prenume;
            nume = (char*)malloc(20 * sizeof(char));
            prenume = (char*)malloc(20 * sizeof(char));
            fscanf(f_date, "%s %s %d", nume, prenume, &punctaj);
            (*heade)->punctaj = (*heade)->punctaj + punctaj;
            adaugareJucator(&headj, nume, prenume, punctaj);
        }
        (*heade)->punctaj = (float)((*heade)->punctaj/(*heade)->nr_jucatori);
    }
}
void scriereInFisier(FILE *f_out, echipa *heade)
{
    echipa *curr = heade;
    while(curr != NULL)
    {
        fprintf(f_out, "%s\n", curr->nume);
        curr = curr->next;
    }
}
float calcPunctajMinim(echipa *heade)
{
    echipa *curre = heade;
    float mini = curre->punctaj;
    while(curre != NULL)
    {
        if(curre->punctaj < mini)
            mini = curre->punctaj;
        curre = curre->next;
    }
    return mini;
}
void eliberareMemorieListeEchipe(echipa *e)
{
    jucator *j = e->jucatori;
    while(j != NULL)
    {
        jucator *auxj = j;
        j = j->next;
        free(auxj);
    }
}
void eliminareEchipe(echipa **heade, int nr_echipe)
{
    int n = 1;
    while(n * 2 <= nr_echipe)
        n = n * 2;
    while(nr_echipe != n)
    {
        echipa *curre = *heade;
        float mini = calcPunctajMinim(curre);
        curre = *heade;
        int gasit = 1;
        //verific daca echipa care trebuie eliminata este prima si daca da, o elimin
        if(mini == curre->punctaj)
        {
            *heade = (*heade)->next;
            eliberareMemorieListeEchipe(curre);
        }
        else
        {   //daca nu e prima echipa, caut echipa cu punctajul minim si o elimin
            echipa *prev = curre;
            curre = curre->next;
            while(curre != NULL && gasit)
            {
                if(mini == curre->punctaj) //daca am gasit echipa ma opresc 
                {
                    prev->next = curre->next;
                    eliberareMemorieListeEchipe(curre);
                    gasit = 0;
                }
                else //caut in continuare echipa
                {
                    prev = curre;
                    curre = curre->next;
                }
            }
        }
        nr_echipe--;
    }
}

//coada
coada *creareCoada()
{
    coada *q;
    q = (coada*)malloc(sizeof(coada));
    q->front = q->rear = NULL;
    return q;
}
int isQueueEmpty(coada* q)
{
    return (q->front == NULL);
}
void adaugareUnMeciInCoada(coada *q, echipa *heade)
{
    echipa *echipa1 = heade;
    if(echipa1 != NULL && echipa1->next!= NULL)
    {
        echipa *echipa2 = echipa1->next;
        meci *meci_nou = (meci*)malloc(sizeof(meci));
        meci_nou->echipa1 = echipa1;
        meci_nou->echipa2 = echipa2;
        meci_nou->next = NULL;
        echipa *aux1 = echipa1;
        echipa *aux2 = echipa2;
        eliberareMemorieListeEchipe(aux1);
        eliberareMemorieListeEchipe(aux2);
        //daca nu exista niciun element in coada
        if(q->rear == NULL)
            q->rear = meci_nou;
        else
        {
            (q->rear)->next = meci_nou;
            (q->rear) = meci_nou;
        }
        //daca exista un singur element in coada
        if(q->front == NULL)
            q->front = q->rear;
    }
}
void afisareCuSpatii(FILE *f_out, echipa *e)
{
    int l = 32;
    int n = l - strlen(e->nume) + 1; // calculez nr de spatii care tb scrise ca sa fie totul aliniat
    while(n)
    {
        fprintf(f_out, " ");
        n--;
    }
}
void afisareMeciuri(FILE *f_out, coada *q)
{
    meci *aux = q->front;
    while(aux != NULL)
    {   //trebuie afisat frumos cu spatii ca in fiserul out
        fprintf(f_out, "%s", (aux->echipa1)->nume);
        afisareCuSpatii(f_out, aux->echipa1);
        fprintf(f_out, "-");
        afisareCuSpatii(f_out, aux->echipa2);
        fprintf(f_out, "%s\n", (aux->echipa2)->nume);
        aux = aux->next;
    }
    fprintf(f_out, "\n");
}
void deQueue(coada *q, echipa **d1, echipa **d2)
{
    if(isQueueEmpty(q)) 
    {
        (*d1) = NULL;
        (*d2) = NULL;
    }
    meci *aux = q->front;
    (*d1) = aux->echipa1;
    (*d2) = aux->echipa2;
    q->front = (q->front)->next;
    if(q->front == NULL)
        q->rear = NULL;
    eliberareMemorieListeEchipe(aux->echipa1);
    eliberareMemorieListeEchipe(aux->echipa2);
    free(aux);
}

//stiva
int isStackEmpty(stiva *s)
{
    return (s == NULL);
}
void push(stiva **top_castigatori, echipa *head_echipa)
{
    stiva *nou = (stiva*)malloc(sizeof(stiva));
    nou->echipe = head_echipa;
    nou->next = *top_castigatori;
    *top_castigatori = nou;
}
echipa *pop(stiva **s)
{
    if(isStackEmpty(*s))
        return NULL;
    stiva *temp = (*s);
    echipa *aux = (*s)->echipe;
    *s = (*s)->next;
    eliberareMemorieListeEchipe(temp->echipe);
    free(temp);
    return aux;
}
void aflareCastigatoriSiNecastigatori(stiva **top_castigatori, stiva **top_necastigatori, coada *q)
{
    while(!isQueueEmpty(q))
    {
        echipa *echipa1 = NULL, *echipa2 = NULL;
        deQueue(q, &echipa1, &echipa2);
        if(echipa1->punctaj > echipa2->punctaj)
        {
            push(top_castigatori, echipa1);
            push(top_necastigatori, echipa2);
        }
        else
        {
            push(top_castigatori, echipa2);
            push(top_necastigatori, echipa1);
        }
        (*top_castigatori)->echipe->punctaj++;
    }
}
int nrEchipe(echipa *heade)
{
    int nr_echipe = 0;
    echipa *copie = heade;
    while(copie != NULL)
    {
        nr_echipe++;
        copie = copie->next;
    }
    return nr_echipe;
}
void initializareCoada(coada **q, echipa *heade)
{
    (*q) = creareCoada();
    echipa *copie=heade;
    while(copie != NULL)
    {
        adaugareUnMeciInCoada((*q), copie);
        copie = copie->next->next;
    }
}
void adaugareInLista8(top8 **head, echipa *castigatoare)
{
    top8 *echipa_noua = (top8*)malloc(sizeof(top8));
    echipa_noua->castigatoare = (echipa*)malloc(sizeof(echipa));
    echipa_noua->castigatoare->punctaj = castigatoare->punctaj;
    echipa_noua->castigatoare->nume = (char*)malloc(sizeof(char) * (strlen(castigatoare->nume)));
    strcpy(echipa_noua->castigatoare->nume, castigatoare->nume);
    echipa_noua->castigatoare->nr_jucatori = castigatoare->nr_jucatori;
    echipa_noua->castigatoare->next = castigatoare->next;
    echipa_noua->castigatoare->jucatori = castigatoare->jucatori;
    echipa_noua->next = *head;
    *head = echipa_noua;
}
void afisareEchipaCastigatoare(FILE *f_out, stiva **top_castigatori, stiva **top_necastigatori, coada *q, echipa *heade, top8 **lista8)
{
    int nr_echipe = nrEchipe(heade);
    initializareCoada(&q, heade);
    int n = 1;
    while(nr_echipe > 1)
    {
        fprintf(f_out,"--- ROUND NO:%d\n", n);
        afisareMeciuri(f_out,q);
        aflareCastigatoriSiNecastigatori(top_castigatori, top_necastigatori, q);
        while(!isStackEmpty(*top_necastigatori))
            pop(top_necastigatori);             //golim stiva necastigatori
        fprintf(f_out,"WINNERS OF ROUND NO:%d\n", n++);
        while(!isStackEmpty(*top_castigatori))
        {
            echipa *echipe = pop(top_castigatori);
            echipa *echipe1 = pop(top_castigatori);
            if(nr_echipe / 2 == 8)                             
            {
                adaugareInLista8(lista8, echipe1);
                adaugareInLista8(lista8, echipe);
            }
            echipe->next = echipe1; // ca sa pot sa adaug iar castigatorii in coada 2 cate 2
            fprintf(f_out,"%s",echipe->nume); //de aici afisam frumos cu spatii castigatorii 2 cate 2
            afisareCuSpatii(f_out, echipe);
            fprintf(f_out," -  %.2f",echipe->punctaj);
            if(echipe->next != NULL) //daca nu am ajuns la un sg castigator, afisez "perechea" echipei pe care am afisat-o mai devreme
            {
                fprintf(f_out,"\n%s",echipe1->nume);
                afisareCuSpatii(f_out, echipe1);
                fprintf(f_out," -  %.2f\n",echipe1->punctaj);
            }
            adaugareUnMeciInCoada(q,echipe);
        }
        fprintf(f_out,"\n"); //afisez o linie libera intre runde
        nr_echipe = nr_echipe / 2;
    }
}