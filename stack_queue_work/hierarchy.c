#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hierarchy.h"
typedef struct cel{
    Tree elem;
    struct cel *next;
} StackCel, *Tstack, QueueCel, *Tqueue;

Tqueue front, rear;

Tree createNode(char *name){
    Tree root;
    root = (Tree)malloc(sizeof(TreeNode));
    root->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(root->name, name);
    root->direct_employees_no = 0;
    root->team = NULL;
    root->manager = NULL;
    return root;
}

Tree Tsearch(Tree tree, char *name){
    Tree aux;
    int i;
    if(tree == NULL){
        return NULL;
    }
    if(strcmp(tree->name, name) == 0){
        return tree;
    }
    if(tree->direct_employees_no == 0){
        return NULL;
    }
    for(i = 0; i < tree->direct_employees_no; i++){
        aux = Tsearch(tree->team[i], name);
        if(aux != NULL){
            return aux;
        }
    }
    return NULL;
}

void InitQueue(){
    front = rear = NULL;
}

int isEmptyQueue(){
    return front == NULL;
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

void Enqueue(Tree tree){
    Tqueue q = (Tqueue)malloc(sizeof(QueueCel));
    if(q == NULL){
        printf("not enough memory\n");
        return;
    }
    q->elem = tree;
    q->next = NULL;
    if(front == NULL && rear == NULL){
        front = rear = q;
        return;
    }
    rear->next = q;
    rear = q;
}


Tstack Push(Tstack s, Tree tree){
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

Tree Dequeue(){
    Tqueue q = front;
    Tree t;
    if(front == NULL){
        printf("empty queue\n");
        return NULL;
    }
    t = front->elem;
    if(front == rear) front = rear = NULL;
    else front = front->next;
    free(q);
    return t;
}

Tstack Pop(Tstack s, Tree *tree){
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

void printTree(Tree tree){
    Tree t;
    Tstack s;
    int i;
    if(tree == NULL){
        return;
    }
    InitQueue();
    Enqueue(tree);
    while(!isEmptyQueue()){
        t = Dequeue(); 
        printf("%s: ", t->name);
        for(i = 0; i < t->direct_employees_no; i++){
            if(t->team[i] != NULL){
                printf("%s ", t->team[i]->name);
                Enqueue(t->team[i]);
            }
        }
        printf("\n");
    }
}


void destroyNode(Tree tree){
    if(tree == NULL){
        return;
    }
    if(tree->name != NULL){
        free(tree->name);
        tree->name = NULL;
    }
    if(tree->team != NULL){
        free(tree->team);
        tree->team = NULL;
    }
    tree->manager = NULL;
    free(tree);
}

void Bubblesort(Tree *team, int n){
    Tree *v_aux = (Tree *)malloc(n * sizeof(Tree));
    int i, k = 0;
    for(i = 0; i < n; i++){
        if(team[i] != NULL){
            v_aux[k++] = team[i];
        }
    }
    for(i = k; i < n; i++){
        v_aux[i] = NULL;
    }

    int sorted = 0; 
    Tree aux;
    while(!sorted) {
        sorted = 1;
        for(i = 0; i < n - 1; i++) {
            if(v_aux[i] != NULL && v_aux[i+1] != NULL && strcmp(v_aux[i]->name, v_aux[i + 1]->name) > 0) {
                aux = v_aux[i];
                v_aux[i] = v_aux[i + 1];
                v_aux[i + 1] = aux;
                sorted = 0;
            }
        }
    }

    for(i = 0; i < n; i++){
        team[i] = v_aux[i];
        v_aux[i] = NULL;
    }
    
    free(v_aux);
}

/* Adauga un angajat nou in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL, 
           atunci employee_name e primul om din ierarhie.
 */
Tree hire(Tree tree, char *employee_name, char *manager_name) {
    Tree newNode, found;
    int i;
    if(tree == NULL){
        newNode = createNode(employee_name);
        return newNode;
    }
    found = Tsearch(tree, manager_name);
    if(found == NULL){
        //managerul nu a fost gasit
        return tree;
    }
    newNode = createNode(employee_name);
    newNode->manager = found;
    if(found->direct_employees_no == 0){
        found->team = (Tree *)malloc(sizeof(Tree));
    }else{
        found->team = (Tree *)realloc(found->team, (found->direct_employees_no + 1) * sizeof(Tree));
    }
    found->team[found->direct_employees_no] = newNode;
    found->direct_employees_no++;
    Bubblesort(found->team, found->direct_employees_no);
    return tree;
}

/* Sterge un angajat din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire(Tree tree, char *employee_name) {
    char v[20], i, j;
    int k = 0;
    Tree *v_aux;
    if(tree == NULL){
        return NULL;
    }
    Tree current, t, boss;
    //cautare element 
    current = Tsearch(tree, employee_name);
    if(current == NULL){
        return tree;
    }
    if(current->manager == NULL){
        //este primul om din ierarhie
        return tree;
    }
    if(current->direct_employees_no == 0){ //nu are copii
        boss = current->manager;
        for(i = 0; i < boss->direct_employees_no; i++){
            if(boss->team[i] != NULL && strcmp(current->name, boss->team[i]->name) == 0){
                break;
            }
        }
        boss->team[i]= NULL; //daca e frunza
        destroyNode(current);
    }else if(current->direct_employees_no == 1){ //are un copil
        boss = current->manager;
        for(i = 0; i < boss->direct_employees_no; i++){
            if(boss->team[i] != NULL && strcmp(current->name, boss->team[i]->name) == 0){
                break;
            }
        }
        
        boss->team[i] = current->team[0];
        current->team[0]->manager = current->manager;  
        destroyNode(current);
        Bubblesort(boss->team, boss->direct_employees_no);
    }else if(current->direct_employees_no >= 2){
        //daca are mai mult de 2 copii
        boss = current->manager;
        for(i = 0; i < boss->direct_employees_no; i++){
            if(boss->team[i] != NULL && strcmp(current->name, boss->team[i]->name) == 0){
                break;
            }
        }
        boss->team[i] = current->team[0];
        boss->team = (Tree *)realloc(boss->team, (boss->direct_employees_no + current->direct_employees_no - 1)*sizeof(Tree));
        for(i = 0; i < current->direct_employees_no; i++){
            if(current->team[i] != NULL){
                current->team[i]->manager = boss;
            }
        }
        for(i = 1; i < current->direct_employees_no; i++){
            boss->team[boss->direct_employees_no + i - 1] = current->team[i];
        }

        boss->direct_employees_no = boss->direct_employees_no + current->direct_employees_no - 1;
        destroyNode(current);
        Bubblesort(boss->team, boss->direct_employees_no);
    }
    return tree;
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin 
 * pe nivelul 2 pentru a putea efectua operatia.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 *
 * return: intoarce ierarhia modificata.
 */

Tree promote(Tree tree, char *employee_name) {
    int i = 0;
    int j, k = 0;
    Tree current, boss, bossmare;
    if(tree == NULL){
        return NULL;
    }
    current = Tsearch(tree, employee_name);
    if(current->manager == NULL){
        return tree;
    }
    if(current->manager->manager == NULL){ // angajatul e minim pe nivelul 2
        return tree;
    }
    //angajatul care treb promovat nu are copii
    
        if(current->direct_employees_no == 0){
            boss = current->manager;
            bossmare = current->manager->manager;
            bossmare->team = (Tree *)realloc(bossmare->team, (bossmare->direct_employees_no + 1)*sizeof(Tree));
            bossmare->team[bossmare->direct_employees_no] = current;
            bossmare->direct_employees_no++;
            
            current->manager = bossmare;
            for(i = 0; i < boss->direct_employees_no; i++){
                if(boss->team[i] != NULL &&strcmp(boss->team[i]->name, current->name ) == 0){
                    break;
                }
            }
            boss->team[i] = NULL;
            Bubblesort(bossmare->team, bossmare->direct_employees_no);
        }
        if(current->direct_employees_no > 0){
            //are mai multi copii
            boss = current->manager;
            bossmare = boss->manager;
            bossmare->team = (Tree *)realloc(bossmare->team, (bossmare->direct_employees_no + 1)*sizeof(Tree));
            boss->team = (Tree *)realloc(boss->team, (boss->direct_employees_no + current->direct_employees_no)*sizeof(Tree));
            for(j = 0; j < current->direct_employees_no; j++){
                if(current->team[j] != NULL){
                    current->team[j]->manager = boss;
                }
                boss->team[boss->direct_employees_no + j] = current->team[j];              
                //pun team ul angajatului promovat in echipa managerului sau
            }

           

            boss->direct_employees_no = boss->direct_employees_no + current->direct_employees_no;
            bossmare->team[bossmare->direct_employees_no] = current;
            bossmare->direct_employees_no++;
            current->manager = bossmare;
            //poz la care tre sa l inserez pe angajatul promovat in echipa managerului managerului
            for(i = 0; i < boss->direct_employees_no; i++){
                if(boss->team[i] != NULL &&strcmp(boss->team[i]->name, current->name ) == 0){
                    break;
                }
            }
            boss->team[i] = NULL;

            Bubblesort(boss->team, boss->direct_employees_no);
            Bubblesort(bossmare->team, bossmare->direct_employees_no);

            free(current->team);
            current->team = NULL;
            current->direct_employees_no = 0;
        }
    return tree;

}

/* Muta un angajat in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name) {
    Tree current, newboss, exboss;
    int i, j, k = 0, var = 0;
    if(tree == NULL){
        return NULL;
    }
    current = Tsearch(tree, employee_name);
    newboss = Tsearch(tree, new_manager_name);
    exboss = current->manager;
    if(current->manager == NULL){ //primul om din ierarhie nu poate fi mutat
        return tree; 
    }
    if(current->manager == newboss){
        return tree;
    }
    if(current->direct_employees_no == 0){ //angajatul care treb mutat nu are copii
        if(newboss->direct_employees_no != 0){
            newboss->team = (Tree *)realloc(newboss->team, (newboss->direct_employees_no + 1)*sizeof(Tree));
        }
        else{
            newboss->team = (Tree *)malloc(sizeof(Tree));
        }
        newboss->team[newboss->direct_employees_no] = current;
        current->manager = newboss; //l am pus pe current la noul manager
        newboss->direct_employees_no++;
        for(i = 0; i < exboss->direct_employees_no; i++){
                if(exboss->team[i] != NULL &&strcmp(exboss->team[i]->name, current->name) == 0){
                    break;
                }
            }
        exboss->team[i] = NULL;
        for(i = 0; i < exboss->direct_employees_no; i++){
            if(exboss->team[i] != NULL){
                var = 1;
            }
        }
        if(var == 0){
            free(exboss->team);
            exboss->team = NULL;
            exboss->direct_employees_no = 0;
        }
        Bubblesort(newboss->team, newboss->direct_employees_no);
    }
    if(current->direct_employees_no > 0){ // are copii
        exboss->team = (Tree *)realloc(exboss->team, (exboss->direct_employees_no + current->direct_employees_no)*sizeof(Tree));
        //exboss
        for(i = 0; i < current->direct_employees_no; i++){
            exboss->team[exboss->direct_employees_no + i] = current->team[i];
            if(current->team[i]!= NULL){
                current->team[i]->manager = exboss;
            }
        }
        for(i = 0; i < exboss->direct_employees_no; i++){
            if(exboss->team[i] != NULL &&strcmp(exboss->team[i]->name, current->name ) == 0){
                break;
            }
        }
        exboss->team[i] = NULL;
        exboss->direct_employees_no = exboss->direct_employees_no + current->direct_employees_no;
        Bubblesort(exboss->team, exboss->direct_employees_no);

        //current
        free(current->team);
        current->team = NULL;
        current->direct_employees_no = 0;
        
        //newboss
        if(newboss->direct_employees_no != 0){
            newboss->team = (Tree *)realloc(newboss->team, (newboss->direct_employees_no + 1)*sizeof(Tree));
        }else{
            newboss->team = (Tree *)malloc(sizeof(Tree));
        }
        newboss->team[newboss->direct_employees_no] = current; //il pun pe angajat la noul sef
        current->manager = newboss;
        newboss->direct_employees_no++;
        Bubblesort(newboss->team, newboss->direct_employees_no);
        
    }
    return tree;
}

/* Muta o echipa in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_team(Tree tree, char *employee_name, char *new_manager_name) {
    Tree current, exboss, newboss;
    int i;
    if(tree == NULL){
        return NULL;
    }
    current = Tsearch(tree, employee_name);
    if(current->manager == NULL){
        return tree;
    }
    //exboss
    exboss = current->manager;
    for(i = 0; i < exboss->direct_employees_no; i++){
        if(exboss->team[i] != NULL &&strcmp(exboss->team[i]->name, current->name ) == 0){
            break;
        }
    }
    exboss->team[i] = NULL;

    //newboss
    newboss = Tsearch(tree, new_manager_name);
    if(newboss->direct_employees_no != 0){
        newboss->team = (Tree *)realloc(newboss->team, (newboss->direct_employees_no + 1)*sizeof(Tree));
    }
    else{
        newboss->team = (Tree *)malloc(sizeof(Tree));
    }
    newboss->team[newboss->direct_employees_no] = current;
    current->manager = newboss;
    newboss->direct_employees_no = newboss->direct_employees_no + 1;
    Bubblesort(newboss->team, newboss->direct_employees_no);

    return tree;
}

/* Concediaza o echipa din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire_team(Tree tree, char *employee_name) {
    Tree current, boss;
    int i;
    if(tree == NULL){
        return NULL;
    }
    current = Tsearch(tree, employee_name);
    if (current == NULL) {
        return tree;
    }
    boss = current->manager;
    if(boss == NULL){
        return tree;
    }
    for(i = 0; i < boss->direct_employees_no; i++){
        if(boss->team[i] != NULL && strcmp(boss->team[i]->name, current->name) == 0){
            break;
        }
    }
    boss->team[i] = NULL;
    destroy_tree(current);
    return tree;
}

/* Afiseaza toti angajatii sub conducerea unui angajat.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */
void get_employees_by_manager(FILE *f, Tree tree, char *employee_name) {
    if(tree == NULL){
        return;
    }
    Tree current;
    current = Tsearch(tree, employee_name);
    if(current == NULL){
        fprintf(f, "\n");
        return;
    }

    Tree t;
    Tstack s;
    int i;
    InitQueue();
    Enqueue(current);
    Tree *aux = (Tree *)malloc(sizeof(Tree));
    aux[0] = current;
    int nrelemaux = 1;
    while(!isEmptyQueue()){
        t = Dequeue(); 
        for(i = 0; i < t->direct_employees_no; i++){
            if(t->team[i] != NULL){
                aux = (Tree *)realloc(aux, (nrelemaux + 1) * sizeof(Tree));
                aux[nrelemaux] = t->team[i];
                nrelemaux++;
                Enqueue(t->team[i]);
            }
        }
    }
    Bubblesort(aux, nrelemaux);
    for(i = 0; i < nrelemaux; i++){
        fprintf(f, "%s ",aux[i]->name);
    }
    fprintf(f, "\n");
    free(aux);
}

/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */

int getlevel(Tree tree){
    int level = 0;
    Tree current = tree;
    if(tree == NULL){
        return -1;
    }
    while(current->manager != NULL){
        level++;
        current = current->manager;
    }
    return level;
}

void get_employees_by_level(FILE *f, Tree tree, int level) {
    Tree t;
    Tstack s;
    int i, nrelemaux = 0;
    Tree *aux = NULL;
    if(tree == NULL || level < 0){
        fprintf(f, "\n");
        return;
    }
    s = InitStack();
    s = Push(s, tree);
    while(!isEmptyStack(s)){
        s = Pop(s, &t); 
        if(t != NULL){
            if(getlevel(t) == -1){ //nu exista t
                fprintf(f, "\n");
            }else{
                if(getlevel(t) == level){
                    if(nrelemaux == 0){
                        aux = (Tree *)malloc(sizeof(Tree));
                        aux[0] = t;
                    }else{
                        aux = (Tree *)realloc(aux, (nrelemaux + 1)* sizeof(Tree));
                        aux[nrelemaux] = t;
                    }
                    nrelemaux++;
                }
            }
            for(i = t->direct_employees_no - 1; i >= 0; i--){
                s = Push(s, t->team[i]);
            }
        }
    }
    Bubblesort(aux, nrelemaux);
    for(i = 0; i < nrelemaux; i++){
        fprintf(f, "%s ", aux[i]->name);
    }
    fprintf(f, "\n");
    
    if (aux != NULL) {
        free(aux);
    }
    return;
}

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */

int getnoofemployee(Tree tree){
    int i, noofem = 0;
    if(tree == NULL){
        return 0;
    }
    for(i = 0; i < tree->direct_employees_no; i++){
        if(tree->team[i] != NULL){
            noofem++;
        }
    }
    return noofem;
}

void get_best_manager(FILE *f, Tree tree) {
    Tree t;
    Tstack s;
    Tree *aux = NULL;
    int i, nrangajatimax = 0, nrelemaux = 0;
    if(tree == NULL){
        return;
    }
    InitQueue();
    Enqueue(tree);
    while(!isEmptyQueue()){
        t = Dequeue(); 
        if(getnoofemployee(t) > nrangajatimax){
            nrangajatimax = getnoofemployee(t);
        }
        for(i = 0; i < t->direct_employees_no; i++){
            if(t->team[i] != NULL){
                Enqueue(t->team[i]);
            }
        }
    }
   
    InitQueue();
    Enqueue(tree);
    while(!isEmptyQueue()){
        t = Dequeue(); 
        if(getnoofemployee(t) == nrangajatimax){
           if(nrelemaux == 0){
               aux = (Tree *)malloc(sizeof(Tree));
               aux[0] = t;
            }else{
               aux = (Tree *)realloc(aux, (nrelemaux + 1) * sizeof(Tree));
               aux[nrelemaux] = t;
            }
            nrelemaux++;
        }
        for(i = 0; i < t->direct_employees_no; i++){
            if(t->team[i] != NULL){
                Enqueue(t->team[i]);
            }
        }
    }
    Bubblesort(aux, nrelemaux);
    for(i = 0; i < nrelemaux; i++){
        fprintf(f, "%s ", aux[i]->name);
    }
    fprintf(f, "\n");
    
    if (aux != NULL) {
    free(aux);
    }
    return;
}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */
Tree reorganize(Tree tree, char *employee_name) {
    Tree winner;
    if(tree == NULL){
        return NULL;
    }
    winner = Tsearch(tree, employee_name);
    return tree;
}

/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void preorder_traversal(FILE *f, Tree tree) {
    Tree t;
    Tstack s;
    int i;
    if(tree == NULL){
        return;
    }
    s = InitStack();
    s = Push(s, tree);
    while(!isEmptyStack(s)){
        s = Pop(s, &t); 
        if(t != NULL){
        if(t->manager == NULL){
            fprintf(f, "%s ", tree->name);
        }else{
            fprintf(f, "%s-%s ", t->name, t->manager->name);
        }
        for(i = t->direct_employees_no - 1; i >= 0; i--){
            s = Push(s, t->team[i]);
        }
        }
    }
    fprintf(f, "\n");
}

/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */
void destroy_tree(Tree tree) {
    Tree t;
    Tstack s;
    int i;
    if(tree == NULL){
        return;
    }
    s = InitStack();
    s = Push(s, tree);
    while(!isEmptyStack(s)){
        s = Pop(s, &t);
        if(t != NULL){
        for(i = t->direct_employees_no - 1; i >= 0; i--){
            s = Push(s, t->team[i]);
        }
        destroyNode(t);
        }
    }
    return;
}
