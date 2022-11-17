#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    char *elem;
    struct Node *next;
}Node;

typedef struct cel{
    Node *elem;
    struct cel *next;
} StackCel, *Tstack;

typedef struct cuv{
    char *s;
}Cuv;

Node *create(char *elem){
    Node *newNode;
    newNode = (Node *)malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->elem = (char *)malloc((strlen(elem) + 1) * sizeof(char));
    strcpy(newNode->elem, elem);
    return newNode;
}

Node *insert(Node *list, Node *newNode){
    Node *current;
    if(list == NULL){
        return newNode;
    }
    current = list;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = newNode;
    return list;
}

void printList(Node *list){
    Node *current;
    current = list;
    while(current != NULL){
        printf("%s ", current->elem);
        current = current->next;
    }
    printf("\n");
}

Tstack InitStack(){
    Tstack s;
    return s = NULL;
}

int isEmptyStack(Tstack s){
    if(s == NULL){
        return 1;
    }else{
        return 0;
    }
}

Tstack Push(Tstack s, Node *tree){
    Tstack t;
    t = (Tstack)malloc(sizeof(StackCel));
    if(t == NULL){
        printf("memorie insuficienta! \n");
        return NULL;
    }
    t->elem = tree;
    t->next = s;
    return t;
}

Tstack Pop(Tstack s, Node **tree){
    Tstack t;
    if (s == NULL){
        printf("stiva vida! \n");
        return NULL;
    }
    *tree = s->elem;
    t = s;
    s = s->next;
    free(t);
    return s;
}

int getposition(Node **vlist, int n, char *elem){
    int i;
    for(i = 0; i < n; i++){
        if(strcmp(vlist[i]->elem, elem) == 0){
            return i;
        }
    }
    
    return -1;
}

void freenode(Node *node){
    if(node == NULL){
        return;
    }
    if(node->elem){
        free(node->elem);
    }
    node->elem = NULL;
    node->next = NULL;
    free(node);
}

void freelist(Node *list){
    if(list == NULL){
        return;
    }
    Node *primul;
    while(list != NULL){
        primul = list;
        list = list->next;
        freenode(primul);
        primul = NULL;
    }
}

void freeVlist(Node **vlist, int n){
    int i;
    for(i = 0; i < n; ++i){
        freelist(vlist[i]);
        vlist[i] = NULL;
    }
    free(vlist);
    vlist = NULL;
}

int visitNR(Node **vlist, int n, char *elem){ 
    Node *t, *prim, *ultim;
    int k, c = 0;
    int *val = (int *)calloc(n, sizeof(int));
    Tstack s;
    s = InitStack();
    prim = create(elem);
    s = Push(s, prim);
    while(!isEmptyStack(s)){
        s = Pop(s, &t);
        k = getposition(vlist, n, t->elem);
        if(strcmp(elem, t->elem) != 0){
            val[k] = 1; //pt primul nu o sa l faca 1 
        }
        printf("%s ",t->elem);
        c++;
        ultim = t;
        if(strcmp(prim->elem, ultim->elem) == 0 && c > 1){
            printf("0\n");
            free(val);
            return 0;
        }
        for(t = vlist[k]->next; t != NULL; t=t->next){
            if(val[getposition(vlist, n, t->elem)] == 0){
               s = Push(s,t);
               //printf("%s", t->elem);
               val[getposition(vlist, n, t->elem)] = -1;
            }
        }
    }
    //printf("\n");
    //printf("%s-%s", prim->elem, ultim->elem);
    printf("\n");
    printf("1\n");
    free(val);
    return 1;
}

Node** visit_stramosi(Node **vlist, int n, char *elem_start, char *elem_stop, int *c){ 
    Node *t, *prim, *ultim;
    int k;
    *c = 0;
    int *val = (int *)calloc(n, sizeof(int));
    Node **v_stramosi;
    Tstack s;
    s = InitStack();
    prim = create(elem_start);
    s = Push(s, prim);
    while(!isEmptyStack(s)){
        s = Pop(s, &t);
        k = getposition(vlist, n, t->elem);
        if(strcmp(elem_start, t->elem) != 0){
            val[k] = 1; //pt primul nu o sa l faca 1
            v_stramosi = (Node **)realloc(v_stramosi, ((*c)+1)*sizeof(Node *));
        }
        else{
            v_stramosi = (Node **)malloc(sizeof(Node *));
        }
        //printf("%s ",t->elem);
        if(strcmp(t->elem, elem_stop) == 0){
            free(val);
            return v_stramosi;
        }
        v_stramosi[*c] = t;

        (*c)++;
        ultim = t;
        if(strcmp(prim->elem, ultim->elem) == 0 && (*c) > 1){
            free(val);
            free(v_stramosi);
            return NULL;
        }
        for(t = vlist[k]->next; t != NULL; t=t->next){
            if(val[getposition(vlist, n, t->elem)] == 0){
               s = Push(s,t);
               val[getposition(vlist, n, t->elem)] = -1;
            }
        }
    }
    free(val);
    return NULL;
}

int main(int argc, char **argv){
    int n, m, i, j, rez = 1, nr;
    FILE *fout;
    char aux[100], dest[100];
    Node **vlist;
    if(strcmp(argv[1],"-c1") == 0){
       printf("cerinta 1\n"); 
       FILE *f = fopen("bnet.in", "r");
       if(!f){
           printf("eroare deschidere fisier\n");
           exit(-1);
       }
       fscanf(f, "%d", &n);
       fscanf(f, "%d", &m);
       vlist = (Node **)malloc(n * sizeof(Node *)); //pt fiecare nod cate o lista de vecini(noduri adiacente)
       for(i = 0; i < n; i++){
           fscanf(f, "%s", aux);
           vlist[i] = create(aux); //punem in lista de adiacenta nodurile initiale
       }
       for(i = 0; i < n; i++){
           printList(vlist[i]);
       }
       for(j = 0; j < m; ++j){
           fscanf(f, "%s %s", aux, dest);
           for(i = 0; i < n; ++i){
               if(strcmp(vlist[i]->elem, aux) == 0){
                   vlist[i] = insert(vlist[i], create(dest));
                   break;
               }
           }
           /*for(i = 0; i < n; ++i){
               if(strcmp(vlist[i]->elem, dest) == 0){
                   vlist[i] = insert(vlist[i], create(aux));
                   break;
               }
           }*/
       }
       for(i = 0; i < n; i++){
           printList(vlist[i]);
       }
       fout = fopen("bnet.out", "w");
       for(i = 0; i < n; ++i){
           if(visitNR(vlist, n, vlist[i]->elem) == 0){
               fprintf(fout, "imposibil\n");
               rez = 0;
               break;
           }
       }
       if(rez == 1){
           fprintf(fout, "corect\n");
       }
       fclose(fout);
       freeVlist(vlist, n);
    }
    if(strcmp(argv[1],"-c2") == 0){
        char *A, *B;
        printf("cerinta 2\n"); 
       FILE *f = fopen("bnet.in", "r");
       if(!f){
           printf("eroare deschidere fisier\n");
           exit(-1);
       }
       fscanf(f, "%d", &n);
       fscanf(f, "%d", &m);
       vlist = (Node **)malloc(n * sizeof(Node *)); //pt fiecare nod cate o lista de vecini(noduri adiacente)
       for(i = 0; i < n; i++){
           fscanf(f, "%s", aux);
           vlist[i] = create(aux); //punem in lista de adiacenta nodurile initiale
       }
       for(i = 0; i < n; i++){
           printList(vlist[i]);
       }
       for(j = 0; j < m; ++j){
           fscanf(f, "%s %s", aux, dest);
           for(i = 0; i < n; ++i){
               if(strcmp(vlist[i]->elem, aux) == 0){
                   vlist[i] = insert(vlist[i], create(dest));
                   break;
               }
           }
       }
       for(i = 0; i < n; i++){
           printList(vlist[i]);
       }
       fscanf(f, "%d\n", &nr);
       int dim, j, dimga, c, k, m;
       char *p1, *p2, *c_aux;
       Node **grafancestral, **v_stramosi;
       for(i = 0; i < nr; ++i){
            fgets(aux, 100, f);
            aux[strlen(aux) - 1] = '\0';
            c_aux = strdup(aux);
            char *p = strtok(aux, " ; ");
            dimga = 0;
            A = (char *)malloc(strlen(p) * sizeof(char));
            strcpy(A, p);
            if(i == 0){
                grafancestral = (Node **)malloc(sizeof(Node *));
            }else{
                grafancestral = (Node **)realloc(grafancestral, (dimga+1)*sizeof(Node *));
            }
            grafancestral[dimga] = create(A); //se pune o clona a nodului A in graful ancestral
            dimga++;

            p = strtok(NULL, " ; ");
            p1 = strtok(p, "|");
            B = (char *)malloc(strlen(p1) * sizeof(char));
            strcpy(B, p1);
            grafancestral = (Node **)realloc(grafancestral, (dimga+1)*sizeof(Node *));
            grafancestral[dimga] = create(B);
            dimga++;

            p1 = strchr(c_aux, '|');
            p1++;
            p2 = strtok(p1, " ");
            Cuv *v_cuv;
            if(p2 != NULL){
                dim = 0;
                v_cuv = (Cuv *)malloc(sizeof(Cuv));
                v_cuv[0].s = strdup(p2);
                dim++;
                while(p2 != NULL ){
                    p2 = strtok(NULL, " ");
                    if(p2 == NULL || p2[0] == '\n'){
                        continue;
                    }
                    v_cuv = (Cuv *)realloc(v_cuv, (dim+1) * sizeof(Cuv));
                    v_cuv[dim].s = strdup(p2);
                    dim++;
                }
                for(j = 0; j < dim; j++){
                    grafancestral = (Node **)realloc(grafancestral, (dimga+1)*sizeof(Node *));
                    grafancestral[dimga] = create(v_cuv[j].s);
                    dimga++;
                }
            }
            c = 0;
            for(j = 0; j < dimga; j++){
                printf("%s \n   ", grafancestral[j]->elem);
                for(k = 0; k < n; ++k){
                    v_stramosi = visit_stramosi(vlist, n, vlist[k]->elem, grafancestral[j]->elem, &c);
                    for(m = 0; i < c; i++){
                        printf("%s ", v_stramosi[m]->elem);
                    }
                    printf("\n");
                }
            }
            printf("\n");
       }
       free(grafancestral);
    }
}