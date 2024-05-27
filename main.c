#include "bibliotecamea.h"
int main(int argc, char *argv[])
{
    FILE *f_date, *f_cerinte, *f_out;
    if((f_cerinte = fopen(*(argv+1), "r"))== NULL)
    {
        printf("fisierul nu s-a putut deschide");
        exit(1);
    }
    if((f_date = fopen(*(argv+2), "r"))== NULL)
    {
        printf("fisierul nu s-a putut deschide");
        exit(1);
    }
    if((f_out = fopen(*(argv+3), "w"))== NULL)
    {
        printf("fisierul nu s-a putut deschide");
        exit(1);
    }
    int *cerinta;
    cerinta = (int*)malloc(5*sizeof(int));
    for(int i = 0; i < 5; i++)
        fscanf(f_cerinte, "%d", cerinta + i);
    fclose(f_cerinte);
    
    //cerinta 1
    int nr_echipe;
    echipa *heade = NULL;
    if(*(cerinta) == 1)
    {
        citireDinFisier(f_date, &heade, &nr_echipe);
    }
    fclose(f_date);
    
    //cerinta 2
    if(*(cerinta+1) == 1)
    {
        eliminareEchipe(&heade, nr_echipe);
    }
    scriereInFisier(f_out, heade);
    fclose(f_out);
    return 0;
}