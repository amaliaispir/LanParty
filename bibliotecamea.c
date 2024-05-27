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