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
    echipa *aux = heade;
    while(aux != NULL)
    {
        fprintf(f_out, "%s\n", aux->nume);
        aux = aux->next;
    }
}
float calcPunctajMinim(echipa *heade)
{
    echipa *aux = heade;
    float mini = aux->punctaj;
    while(aux != NULL)
    {
        if(aux->punctaj < mini)
            mini = aux->punctaj;
        aux = aux->next;
    }
    return mini;
}
void eliminareEchipe(echipa **heade, int nr_echipe)
{
    int n = 1;
    while(n * 2 <= nr_echipe)
        n = n * 2;
    while(nr_echipe != n)
    {
        echipa *aux = *heade;
        float mini = calcPunctajMinim(aux);
        aux = *heade;
        int gasit = 1;
        //verific daca echipa care trebuie eliminata este prima si daca da, o elimin
        if(mini == aux->punctaj)
        {
            *heade = (*heade)->next;
            free(aux);
        }
        else
        {   //daca nu e prima echipa, caut echipa cu punctajul minim si o elimin
            echipa *prev = aux;
            aux = aux->next;
            while(aux != NULL && gasit)
            {
                if(mini == aux->punctaj) //daca am gasit echipa ma opresc 
                {
                    prev->next = aux->next;
                    free(aux);
                    gasit = 0;
                }
                else //caut in continuare echipa
                {
                    prev = aux;
                    aux = aux->next;
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
int coadaEsteGoala(coada* q)
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
void extragereEchipeDinCoada(coada *q, echipa **d1, echipa **d2)
{
    if(coadaEsteGoala(q)) 
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
    free(aux);
}

//stiva
int stivaEsteGoala(stiva *s)
{
    return (s == NULL);
}
void adaugareEchipaInStiva(stiva **top_castigatori, echipa *head_echipa)
{
    stiva *nou = (stiva*)malloc(sizeof(stiva));
    nou->echipe = head_echipa;
    nou->next = *top_castigatori;
    *top_castigatori = nou;
}
echipa *extragereEchipaDinStiva(stiva **s)
{   // returneaza informatia stocata in varf si sterge nodul
    if(stivaEsteGoala(*s))  
        return NULL;
    stiva *temp = (*s); //stocheaza adresa varfului in temp
    echipa *aux = (*s)->echipe; //stocheaza valoarea din varf in aux
    //sterge elementul din varf
    *s = (*s)->next;
    free(temp);
    return aux;
}
void aflareCastigatoriSiNecastigatori(stiva **top_castigatori, stiva **top_necastigatori, coada *q)
{
    while(!coadaEsteGoala(q))
    {
        echipa *echipa1 = NULL, *echipa2 = NULL;
        extragereEchipeDinCoada(q, &echipa1, &echipa2);
        if(echipa1->punctaj > echipa2->punctaj)
        {
            adaugareEchipaInStiva(top_castigatori, echipa1);
            adaugareEchipaInStiva(top_necastigatori, echipa2);
        }
        else
        {
            adaugareEchipaInStiva(top_castigatori, echipa2);
            adaugareEchipaInStiva(top_necastigatori, echipa1);
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
        while(!stivaEsteGoala(*top_necastigatori))
            extragereEchipaDinStiva(top_necastigatori);             //golim stiva necastigatori
        fprintf(f_out,"WINNERS OF ROUND NO:%d\n", n++);
        while(!stivaEsteGoala(*top_castigatori))
        {
            echipa *echipe = extragereEchipaDinStiva(top_castigatori);
            echipa *echipe1 = extragereEchipaDinStiva(top_castigatori);
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

//BST
clasament *nodNou(echipa *echipa_noua)
{
    clasament *nod = (clasament*)malloc(sizeof(clasament));
    nod->echipe = echipa_noua;
    nod->left = nod->right = NULL;
    return nod;
}
clasament *inserareEchipaInBST(clasament *nod, echipa *key)
{
    //daca (sub)arborele este gol, creeaza nod
    if(nod == NULL)
        return nodNou(key);
    //altfel, coboara la stanga sau la dreapta, verificand conditia cu punctajul
    if((key->punctaj < nod->echipe->punctaj) || (key->punctaj == nod->echipe->punctaj && strcmp(key->nume, nod->echipe->nume) < 0))
        nod->left = inserareEchipaInBST(nod->left, key);
    else if((key->punctaj > nod->echipe->punctaj) || (key->punctaj == nod->echipe->punctaj && strcmp(key->nume, nod->echipe->nume) > 0))
        nod->right = inserareEchipaInBST(nod->right, key);
    return nod;
}
void inorder(FILE *f_out, clasament *root)
{
    if(root)
    {
        inorder(f_out,root->right);
        fprintf(f_out, "%s ", root->echipe->nume);
        afisareCuSpatii(f_out,root->echipe);
        fprintf(f_out, "-  %.2f\n", root->echipe->punctaj);
        inorder(f_out,root->left);
    }
}
void afisareBST(FILE *f_out, top8 *lista8, clasament **root)
{
    fprintf(f_out, "\nTOP 8 TEAMS:\n");
    while(lista8 != NULL)
    {
        (*root) = inserareEchipaInBST((*root), lista8->castigatoare);
        lista8 = lista8->next;
    }
    free(lista8);
    inorder(f_out, (*root));
}

//AVL
int nodeHeight(arbore *root)
{
    if(root == NULL)
        return -1; 
    else
        return root->height;
}
arbore *leftRotation(arbore *z)
{
    arbore *y = z->right;
    arbore *T2 = y->left;
    //roteste
    y->left = z;
    z->right = T2;
    //modifica inaltimile pentru z si y
    z->height = max(nodeHeight(z->left), nodeHeight(z->right)) + 1;
    y->height = max(nodeHeight(y->left), nodeHeight(y->right)) + 1;
    return y;
}
arbore *rightRotation(arbore *z)
{
    arbore *y = z->left;
    arbore *T3 = y->right;
    y->right = z;
    z->left = T3;
    z->height = max(nodeHeight(z->left), nodeHeight(z->right)) + 1;
    y->height = max(nodeHeight(y->left), nodeHeight(y->right)) + 1;
    return y;
}
arbore *LRRotation(arbore *Z)
{
    Z->left = leftRotation(Z->left);
    return rightRotation(Z);
}
arbore *RLRotation(arbore *Z)
{
    Z->right = rightRotation(Z->right);
    return leftRotation(Z);
}
int max(int a, int b)
{
    return ((a > b) ? a : b);
}
arbore *inserareEchipaInArbore(arbore *nod, echipa *nou)
{
    //inserare nod
    if(nod == NULL)
    {
        nod = (arbore*)malloc(sizeof(arbore));
        nod->echipe = (echipa*)malloc(sizeof(echipa));
        nod->echipe->nume = (char*)malloc(sizeof(char) * strlen(nou->nume));
        strcpy(nod->echipe->nume, nou->nume);
        nod->echipe->punctaj = nou->punctaj;
        nod->height = 0; //adaugare ca frunza
        nod->left = nod->right = NULL;
        return nod;
    }
    if(nou->punctaj < nod->echipe->punctaj || (nou->punctaj == nod->echipe->punctaj && strcmp(nou->nume, nod->echipe->nume) < 0))
        nod->left = inserareEchipaInArbore(nod->left, nou);
    else if(nou->punctaj > nod->echipe->punctaj || (nou->punctaj == nod->echipe->punctaj && strcmp(nou->nume, nod->echipe->nume) > 0))
        nod->right = inserareEchipaInArbore(nod->right, nou);
    else
        return nod; //nu exista chei duplicat
    //updateaza inaltimea nodurilor stramos de jos in sus la iesirea din apelul recurent
    nod->height = 1 + max(nodeHeight(nod->left), nodeHeight(nod->right));
    //afla factorul de echilibru al nodului stramos
    //testul se face de jos in sus pentru toate nodurile intre cel adaugat si radacina
    int k  = (nodeHeight(nod->left) - nodeHeight(nod->right));
    //daca arborele nu e echilibrat, echilibreaza in fct de punctaj
    if((k > 1 && nou->punctaj < nod->left->echipe->punctaj) || (k > 1 && nou->punctaj == nod->left->echipe->punctaj && strcmp(nou->nume, nod->left->echipe->nume) < 0))
        return rightRotation(nod);
    if(k < -1 && nou->punctaj > nod->right->echipe->punctaj || (k < -1 && nou->punctaj == nod->left->echipe->punctaj && strcmp(nou->nume, nod->left->echipe->nume) > 0))
        return leftRotation(nod);
    if(k > 1 && nou->punctaj > nod->left->echipe->punctaj || (k > 1 && nou->punctaj == nod->left->echipe->punctaj && strcmp(nou->nume, nod->left->echipe->nume) > 0))
        return LRRotation(nod);
    if(k < -1 && nou->punctaj < nod->right->echipe->punctaj || (k < -1 && nou->punctaj == nod->left->echipe->punctaj && strcmp(nou->nume, nod->left->echipe->nume) < 0))
        return RLRotation(nod);
    return nod; //returneaza nodul nemodificat
}
void adaugareDinBSTinLista(clasament *root, echipa **head)
{
    if(root)
    {
        adaugareDinBSTinLista(root->left, head);
        echipa *noua = (echipa*)malloc(sizeof(echipa));
        noua->punctaj = root->echipe->punctaj;
        noua->nume = (char*)malloc(sizeof(char) * strlen(root->echipe->nume));
        strcpy(noua->nume, root->echipe->nume);
        free(root->echipe);
        free(root);
        noua->next = *head;
        *head = noua;
        adaugareDinBSTinLista(root->right, head);
    }
}
void afisareNivel(FILE *f_out, arbore *root, int level)
{
    if(root == NULL)
        return;
    if(level == 0) //consideram 0 nivelul radacinii
        fprintf(f_out, "%s\n", root->echipe->nume);
    else if(level > 0)
    {
        afisareNivel(f_out, root->right, level-1);
        afisareNivel(f_out, root->left, level-1);
    }
}
void afisareAVL(FILE *f_out, arbore *avl, echipa *head, clasament *root)
{
    adaugareDinBSTinLista(root, &head);
    do
    {
        avl = inserareEchipaInArbore(avl, head);
        echipa *aux = head;
        free(head);
        head = head->next;
    }while(head != NULL);
    fprintf(f_out, "\nTHE LEVEL 2 TEAMS ARE: \n");
    afisareNivel(f_out, avl, 2); 
}
//eliberare memorie
void eliberareListaJucatori(jucator *head)
{
    jucator *aux;
    while(head != NULL)
    {
        aux = head;
        head = head->next;
        free(aux->nume);
        free(aux->prenume);
        free(aux);
    }
}
void eliberareListaEchipe(echipa *head)
{
    echipa *aux;
    while(head != NULL)
    {
        aux = head;
        head = head->next;
        jucator *aux1 = head->jucatori;
        while(head->jucatori != NULL)
        {
            aux1 = head->jucatori;
            head->jucatori = head->jucatori->next;
            eliberareListaJucatori(aux1);
        }
    }
}
void eliberareStiva(stiva *top)
{
    stiva *aux;
    while(!stivaEsteGoala(top))
    {
        aux = top;
        top = top->next;
        eliberareListaEchipe(aux->echipe);
        free(aux);
    }
}
void eliberareTop8(top8 *head)
{
    top8 *aux;
    while(head != NULL)
    {
        aux = head;
        head = head->next;
        free(aux->castigatoare->nume);
        free(aux);
    }
}
void eliberareArbore(arbore *root)
{
    if(root != NULL)
    {
        eliberareArbore(root->left);
        eliberareArbore(root->right);
        eliberareListaEchipe(root->echipe);
        free(root);
    }
}