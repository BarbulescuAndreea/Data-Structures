#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int numar;
typedef char culoare;

typedef struct List{
    numar n;
    culoare c;
    struct List *next;
} ListNode;

ListNode *insert(ListNode *p, numar nr, culoare cul){
    ListNode *newNode, *node;
    newNode= (ListNode *)malloc(sizeof(ListNode));
    newNode->next = NULL;
    newNode->c = cul;
    newNode->n = nr;
    if(p == NULL){
        p = newNode;
    }else{
        node = p;
        while(node->next != NULL){
            node = node->next;
        }
        node->next = newNode;
    }
    return p;
}

ListNode *initializare(){
    numar nr;
    ListNode *p = NULL;
    for(nr = 15; nr >=2 ; nr--){
        if(nr != 11){
            p = insert(p, nr, 's');
            p = insert(p, nr, 'h');
            p = insert(p, nr, 'c');
            p = insert(p, nr, 'd');
        }
    }
   
    return p;
}

ListNode *inserarefront(ListNode *p, numar nr, culoare cul){
     ListNode *newNode;
    newNode= (ListNode *)malloc(sizeof(ListNode));
    newNode->c = cul;
    newNode->n = nr;
    newNode->next = p;
    return newNode;
}

ListNode *inversare(ListNode *p){
    ListNode *aux, *ultim = NULL;
    aux = p;
    while(aux != NULL){
        ultim = inserarefront(ultim, aux->n, aux->c);
        aux = aux->next;
    }
    return ultim;
}

ListNode *deletelist(ListNode *p){
    ListNode *temp;
    while(p){
        temp = p;
        p = p->next;
        free(temp);
    }
    return NULL;
}

ListNode *NPiles(ListNode *p, int N, int *v){
    ListNode **vlist, *aux, *finallist, **vnou;
    finallist = NULL;
    aux = p;
    vlist = (ListNode **)malloc(N * sizeof(ListNode *));
    vnou = (ListNode **)malloc(N * sizeof(ListNode *));
    int i = 0, dim = 0, dimsubpachet, dimvlist = 0, j, k;
    while(aux != NULL){
        dim++;
        aux = aux->next;
    }
    dimsubpachet = dim / N;
    aux = p;
    while(aux != NULL){
        if(i % dimsubpachet == 0){
            vlist[dimvlist] = aux;
            dimvlist++;
        }
        aux = aux->next;
        i++;
    }
    for(i = 0; i < N; i++){
        vnou[i] = vlist[v[i] - 1];
    }
    for(j = 0; j < N; j++){
        aux = vnou[j];
        for(k = 0; k < dimsubpachet; k++){
            finallist = insert(finallist, aux->n, aux->c);
            aux = aux->next;
        }
    }
    free(vlist);
    free(vnou);
    p = deletelist(p);
    return finallist;
}

void writelist(ListNode *p, FILE *f){
    while(p != NULL){
        if(p->n == 10){
            fprintf(f, "%c", 'T');
        }else if(p->n == 12){
            fprintf(f,"%c", 'J');
        }else if(p->n == 13){
            fprintf(f,"%c", 'Q');
        }else if(p->n == 14){
            fprintf(f,"%c", 'K');
        }else if(p->n == 15){
            fprintf(f,"%c", 'A');
        }else{
            fprintf(f, "%d", p->n);
        }
        if(p->next == NULL){
            fprintf(f, "%c\n", p->c);
        }else{
            fprintf(f, "%c ", p->c);
        }
        p = p->next;
    }
}

void writelistwithoutenter(ListNode *p, FILE *f){
     while(p != NULL){
        if(p->n == 10){
            fprintf(f, "%c", 'T');
        }else if(p->n == 12){
            fprintf(f,"%c", 'J');
        }else if(p->n == 13){
            fprintf(f,"%c", 'Q');
        }else if(p->n == 14){
            fprintf(f,"%c", 'K');
        }else if(p->n == 15){
            fprintf(f,"%c", 'A');
        }else{
            fprintf(f, "%d", p->n);
        }
        if(p->next == NULL){
            fprintf(f, "%c", p->c);
        }else{
            fprintf(f, "%c ", p->c);
        }
        p = p->next;
    }
}
void printList(ListNode *p){
    if(p == NULL){
        printf("lista este goala\n");
        return;
    }
    while(p != NULL){
        if(p->n == 10){
            printf("%c", 'T');
        }else if(p->n == 12){
            printf("%c", 'J');
        }else if(p->n == 13){
            printf("%c", 'Q');
        }else if(p->n == 14){
            printf("%c", 'K');
        }else if(p->n == 15){
            printf("%c", 'A');
        }else{
            printf("%d", p->n);
        }
        if(p->next == NULL){
            printf("%c\n", p->c);
        }else{
            printf("%c ", p->c);
        }
        p = p->next;
    }
}
void printlistwithoutenter(ListNode *p){
    while(p != NULL){
        if(p->n == 10){
            printf("%c", 'T');
        }else if(p->n == 12){
            printf("%c", 'J');
        }else if(p->n == 13){
            printf("%c", 'Q');
        }else if(p->n == 14){
            printf("%c", 'K');
        }else if(p->n == 15){
            printf("%c", 'A');
        }else{
            printf("%d", p->n);
        }
        if(p->next == NULL){
            printf("%c", p->c);
        }else{
            printf("%c ", p->c);
        }
        p = p->next;
    }
}

int length(ListNode *p){
    int lungime = 0;
    while(p != NULL){
        lungime++;
        p = p->next;
    }
    return lungime;
}

ListNode *split(ListNode *p, ListNode *lstanga, ListNode *ldreapta){
    int lungime = length(p), i;
    ListNode *current = p;
    if(lungime < 2){
        lstanga = p;
        ldreapta = NULL;
        return lstanga;
    }
    int contor = (lungime - 1)/2;
    for(i = 0; i < contor; i++){
        current = current->next;
    }
    lstanga = p; 

    ldreapta = current->next;
    current->next = NULL;
    struct List dummycel;
    ListNode *aux = &dummycel;
    while(1){
        if(lstanga == NULL){
            aux->next = ldreapta;
            break;
        }
        else if(ldreapta == NULL){
            aux->next = lstanga;
            break;
        }
        else{
            aux->next = lstanga;
            aux = lstanga;
            lstanga = lstanga->next;

            aux->next = ldreapta;
            aux = ldreapta;
            ldreapta = ldreapta->next;
        }
    }
    return dummycel.next;

}

ListNode *mongean(ListNode *p){
    ListNode *prima = p, *temp = NULL, *ultimul = p;
    int i, lungime = 0;
    p = p->next;
    prima->next = NULL;
    lungime = length(p);
    for(i = 0; i < lungime; i++){
       //if i % 2 == 0 insert la inceput
       if(i % 2 == 0){
           temp = p->next;
           p->next = prima;
           prima = p;
           p = temp;
           /*if(p){
               p = p->next;
           }*/
       }else{
           temp = p->next;
           ultimul->next = p;
           ultimul = p;
           ultimul->next = NULL;
           p = temp;
       }
       // i % 2 == 1 insert la sfarsit
    }
    return prima;
}

int minim(ListNode *p, int x){
    int lungime = length(p);
    if(lungime < x){
        return lungime;
    }else{
        return x;
    }
}
int razboi(ListNode *p, int *carte){
    ListNode *unu, *doi, *temp, *copie1, *copie2, *aux, *ultim, *newlist2, *newlist1, *currentx;
    int lungime = length(p), i, Nrcartiintoarse, x, y, maini = 0, vf_unu[15] = {0}, vf_doi[15] = {0};
    ListNode *current = p;
    if(lungime < 2){
        unu = p;
        doi = NULL;
    }
    int contor = (lungime - 1)/2;
    for(i = 0; i < contor; i++){
        current = current->next;
    }
    unu = p; 
    doi = current->next;
    current->next = NULL;
    while(1){
    	    maini++;
    	    if (maini == 101) {
    	    	aux = unu;
    	    	while(aux != NULL) {
    	    	    vf_unu[aux->n]++;
		    aux = aux->next;
		}
		
		aux = doi;
    	    	while(aux != NULL) {
    	    	    vf_doi[aux->n]++;
		    aux = aux->next;
		}
		
		for(i = 15; i > 2; i--) {
		    if (vf_unu[i] > vf_doi[i]) {
		        *carte = i;
		    	return 1;
		    }
		    else if (vf_unu[i] < vf_doi[i]) {
		        *carte = i;
		    	return 2;
		    }
		}
    	    }
    	    
	    if(unu->n > doi->n){
		current = unu;
		while(current->next != NULL){
		    current = current->next;
		}
		temp = unu->next;
		current->next = unu;
		unu->next = NULL;
		current = unu;
		unu = temp;
		
		temp = doi->next;
		current->next = doi;
		doi->next = NULL;
		current = doi;
		doi = temp;
		if(length(doi) == 0){
		    return 1;
		}
	    }
	    else if(unu->n < doi->n){ 
		
		current = doi;
		while(current->next != NULL){
		    current = current->next;
		}
		temp = doi->next;
		current->next = doi;
		doi->next = NULL;
		current = doi;
		doi = temp;

		temp = unu->next;
		current->next = unu;
		unu->next = NULL;
		current = unu;
		unu = temp;    
		if(length(unu) == 0){
		    return 2;
		}
	    }
	    else if(unu->n == doi->n){
		x = minim(unu, unu->n);
		y = minim(doi, doi->n);
		if(x > y){
		    Nrcartiintoarse = y;
		}
		else{
		    Nrcartiintoarse = x;
		}
		if(Nrcartiintoarse == 1){
		    if(unu->n >= doi->n ){
		        currentx = unu;
		        while(currentx->next != NULL){
		            currentx = currentx->next;
		        }
		        currentx->next = doi;
		        if(length(doi) == 1){
		            return 1;
		        }
		    }
		}
		copie1 = unu;
		copie2 = doi;
		for(i = 0; i < Nrcartiintoarse; i++){
		   copie1 = copie1->next;
		   copie2 = copie2->next;
		   if(copie1->next == NULL || copie2->next == NULL) {
		       break;
		   }
		}
		
		if(copie2->n > copie1->n){
		    
		    ultim = copie2;
		    while(ultim->next != NULL){
		        ultim = ultim->next;
		    }
		    if (ultim != copie2) {
		    	newlist2 = copie2->next;
		    	ultim->next = copie2;
		    	copie2->next = NULL;
		    }
		    else {
		    	newlist2 = copie2;
		    }
		    
		    temp = doi->next;
		    copie2->next = doi;
		    doi->next = NULL;
		    ultim = doi;
		    doi = temp;
		    aux = ultim;
		    for(i = 0; i < Nrcartiintoarse - 1; i++){
		        temp = doi->next;
		        copie2->next = doi;
		        doi->next = aux;
		        aux = doi;
		        doi = temp;
		    }

		    //pun cartile primului jucator in al doilea
		    newlist1 = copie1->next;

		    ultim->next = copie1;
		    copie1->next = NULL;

		    copie1->next = unu;
		    temp = unu->next;
		    unu->next = NULL;
		    ultim = unu;
		    aux = unu;
		    unu = temp;
		    for(i = 0; i < Nrcartiintoarse - 1; i++){
		        temp = unu->next;
		        copie1->next = unu;
		        unu->next = aux;
		        aux = unu;
		        unu = temp;
		    }
		    unu = newlist1;
		    doi = newlist2;
		    if(length(newlist1) == 0){
		        return 2;
		    }
		}else{
		    ultim = copie1;
		    while(ultim->next != NULL){
		        ultim = ultim->next;
		    }
		    if (ultim != copie1) {
		    	newlist1 = copie1->next;
		    	ultim->next = copie1;
		    	copie1->next = NULL;
		    }
		    else {
		    	newlist1 = copie1;
		    }

		    temp = unu->next;
		    copie1->next = unu;
		    unu->next = NULL;
		    ultim = unu;
		    unu = temp;
		    aux = ultim;
		    for(i = 0; i < Nrcartiintoarse - 1; i++){
		        temp = unu->next;
		        copie1->next = unu;
		        unu->next = aux;
		        aux = unu;
		        unu = temp;
		    }

		    //pun cartile primului jucator in al doilea
		    newlist2 = copie2->next;

		    ultim->next = copie2;
		    copie2->next = NULL;

		    copie2->next = doi;
		    temp = doi->next;
		    doi->next = NULL;
		   
		    aux = doi;
		    doi = temp;
		    for(i = 0; i < Nrcartiintoarse - 1; i++){
		        temp = doi->next;
		        copie2->next = doi;
		        doi->next = aux;
		        aux = doi;
		        doi = temp;
		    }
		    unu = newlist1;
		    doi = newlist2;
		    if(length(newlist2) == 0){
		        return 1;
		        }
            }
	    }
    }
    return -1;
}

int main(){
    ListNode *p;
    FILE *fout, *fin;
    int jucator;
    char task[15], operatie[20];
    int N, i, j, carte = -1;
    int nrop;
    int *v;
    ListNode *a = NULL;
    ListNode *b = NULL;
    fin = fopen("in", "r");
    if(!fin){
        printf("Eroare deschidere in");
        exit(-1);
    }
    fout = fopen("out", "w");
    if(!fout){
        printf("Eroare deschidere out");
        exit(-1);
    }

    fscanf(fin, "%s", task);
    p = initializare();
    if(strcmp(task, "amestecare") == 0){
        fscanf(fin, "%d", &nrop);
        for(i = 0; i < nrop; i++){
            fscanf(fin, "%s", operatie);
            if(strcmp(operatie, "inversare") == 0){
                p = inversare(p);
            }else if(strcmp(operatie, "npiles") == 0){
                fscanf(fin, "%d", &N);
                v = (int *)malloc(N * sizeof(int));
                for(j = 0; j < N; j++){
                    fscanf(fin, "%d", &v[j]);
                }
                p = NPiles(p, N, v);
            }else if(strcmp(operatie, "intercalare") == 0){
                p =split(p, a, b);
            }else if(strcmp(operatie, "mongean") == 0){
                p = mongean(p);
            }
        }
        writelist(p,fout);
    }else if(strcmp(task, "razboi") == 0){
        fscanf(fin, "%d", &nrop);
        for(i = 0; i < nrop; i++){
             fscanf(fin, "%s", operatie);
            if(strcmp(operatie, "inversare") == 0){
                p = inversare(p);
            }else if(strcmp(operatie, "npiles") == 0){
                fscanf(fin, "%d", &N);
                v = (int *)malloc(N * sizeof(int));
                for(j = 0; j < N; j++){
                    fscanf(fin, "%d", &v[j]);
                }
                p = NPiles(p, N, v);
            }else if(strcmp(operatie, "intercalare") == 0){
                p =split(p, a, b);
            }else if(strcmp(operatie, "mongean") == 0){
                p = mongean(p);
            }
        }
        //printList(p);
        jucator = razboi(p, &carte);
        fprintf(fout, "%d", jucator);
        if (carte != -1) {
            if(carte == 10){
            	fprintf(fout, " %c", 'T');
            }else if(carte == 12){
            	fprintf(fout, " %c", 'J');
            }else if(carte == 13){
            	fprintf(fout, " %c", 'Q');
            }else if(carte == 14){
            	fprintf(fout, " %c", 'K');
            }else if(carte == 15){
            	fprintf(fout, " %c", 'A');
            }else{
            	fprintf(fout, " %d", carte);
            }
        
        }
    }
    fclose(fout);
    fclose(fin);
    return 0;
}
