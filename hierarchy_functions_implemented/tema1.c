#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name; // numele directorului
	struct Dir* parent;  // pointer catre parintele directorului(null pentru radacina)
	struct File* head_children_files; // pointer catre primul element de tip File din interiorul directorului
	struct Dir* head_children_dirs; // pointer catre primul element de tip Dir din interiorul directorului
	struct Dir* next; // pointer catre urmatorul element din lista in care se afla directorul
} Dir; //structura de tip director

typedef struct File {
	char *name; // numele fisierului
	struct Dir* parent; // pointer catre directorul parent
	struct File* next; // pointer catre urmatorul element din lista de fisiere
} File; // structura de tip fisier


void freeDir(Dir* dir){
	if(dir == NULL){
		return;
	}
	//printf("se elibereaza memoria pentru dir %s\n", dir->name);
	free(dir->name);
	free(dir);
}


void freeFile(File* file){
	if(file == NULL){
		return;
	}
	//se elibereaza memoria pentru fis file->name;
	free(file->name);
	free(file);
}

void stop (Dir* target) {
	if(target != NULL){
		File* currentlist = target->head_children_files;
		while(currentlist != NULL){
			File* tmp = currentlist;
			currentlist = currentlist->next;
			freeFile(tmp);
		}
		Dir* currentdir = target->head_children_dirs;
		while(currentdir != NULL){
			if(currentdir->head_children_dirs){
				stop(currentdir->head_children_dirs);
			}
			if(currentdir->head_children_files){
				while(currentdir->head_children_files){
					File* temp_file = currentdir->head_children_files;
					currentdir->head_children_files = currentdir->head_children_files->next;
					freeFile(temp_file);
				}
			}
			Dir* temp = currentdir;
			currentdir = currentdir->next;
			freeDir(temp);
		}
	}
	freeDir(target);
}

Dir* createDirector(Dir* parent, char* name){
	Dir* director = (Dir*)malloc(sizeof(Dir));
	director->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(director->name, name);
	director->parent = parent;
	director->next = NULL;
	director->head_children_files = NULL;
	director->head_children_dirs = NULL;
	return director;
}

File* createFile(Dir* parent, char* name){
	File* fisier = (File*)malloc(sizeof(File));
	fisier->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(fisier->name, name);
	fisier->parent = parent;
	fisier->next = NULL;
	return fisier;
}

void touch (Dir* parent, char* name) {

	//creez fisierul pe care vreau sa il inserez in lista de fisiere din directorul curent
	File* newFile;
	File* fileslist = parent->head_children_files;
	Dir* dirList = parent->head_children_dirs;
	//daca lista de fisiere e goala, il inserez primul si fac legaturile la directorul parinte 
	//parent si ies din functie
	if(fileslist == NULL){
		newFile = createFile(parent, name);
		parent->head_children_files = newFile;
		newFile->parent = parent;
		return;
	}
	File* aux = fileslist;
	//daca exista un fisir/director cu acelasi nume, nu il mai inserez pe cel nou 
	File* currentFile = fileslist;
	Dir* currentDir = dirList;
	while(currentFile != NULL){
		if(strcmp(currentFile->name, name) == 0){
			printf("File already exists\n");
			return;
		}
		currentFile = currentFile->next;
	}
	while(currentDir != NULL){
		if(strcmp(currentDir->name, name) == 0){
			printf("File already exists\n");
			return;
		}
		currentDir = currentDir->next;
	}
	//inserarea la final
	while(aux->next != NULL){
		aux = aux->next;
	}
	newFile = createFile(parent, name);
	aux->next = newFile;
}

void mkdir (Dir* parent, char* name) {
	Dir* newDir;
	Dir* dirList = parent->head_children_dirs;
	File* fileList = parent->head_children_files;
	//daca lista de directoare e goala, atunci il inserez la inceput si fac legatura cu directorul parinte
	if(dirList == NULL){
		newDir = createDirector(parent, name);
		parent->head_children_dirs = newDir;
		newDir->parent = parent;
		return;
	}
	File* currentFile = fileList;
	Dir* currentDir = dirList;
	Dir* aux = dirList;
	while(currentFile != NULL){
		if(strcmp(currentFile->name, name) == 0){
			printf("Directory already exists\n");
			return;
		}
		currentFile = currentFile->next;
	}
	while(currentDir != NULL){
		if(strcmp(currentDir->name, name) == 0){
			printf("Directory already exists\n");
			return;
		}
		currentDir = currentDir->next;
	}
	//inserare la final
	while(aux->next != NULL){
		aux = aux->next;
	}
	newDir = createDirector(parent, name);
	aux->next = newDir;
}

void ls (Dir* parent) {
	if(parent == NULL){
		printf("director null!\n");
		return;
	}
	File* listaFisiere = parent->head_children_files;
	Dir* listaDirectoare = parent->head_children_dirs;
	while(listaDirectoare != NULL){
		printf("%s\n", listaDirectoare->name);
		listaDirectoare = listaDirectoare->next;
	}
	while(listaFisiere != NULL){
		printf("%s\n", listaFisiere->name);
		listaFisiere = listaFisiere->next;
	}
}

void rm (Dir* parent, char* name) {
	if(parent->head_children_files != NULL){
		int isFound = 0;
		File* aux;
		File* prev= parent->head_children_files;
		File* x = parent->head_children_files;
		File* current = parent->head_children_files->next;
		Dir* dir = parent->head_children_dirs;
		while(x != NULL){
			if(strcmp(x->name, name) == 0){
				isFound = 1;
				break;
			}
			x = x->next;
		}
		if(isFound == 0){
			printf("Could not find the file\n");
			return;
		}
		if(strcmp(prev->name, name) == 0){ //daca cel care trebuie sters e primul
			aux = parent->head_children_files;
			parent->head_children_files = parent->head_children_files->next;
			freeFile(aux);
		}else if(strcmp(current->name, name) == 0){ //daca cel care trebuie sters e al doilea
			File* tmp = prev->next;
			prev->next = prev->next->next;
			freeFile(tmp);
		}else{
			while(current->next != NULL){ 
				prev = current;
				current = current->next;
				if(strcmp(current->name, name) == 0){
					File* tmp2 = prev->next;
					prev->next = prev->next->next;
					freeFile(tmp2);
					break;
				}
			}
		}
	}else{
		printf("Could not find the file\n");
		return;
	}
}

void rmdir (Dir* parent, char* name) {
	/*Functia elimina directorul cu numele name din directorul curent si toate fisierele care se gasesc in acesta. 
	 Daca nu a fost gasit niciun director cu numele dat, 
	 se va afisa mesajul de eroare Could not find the dir\n. 
	 Numele directorului nu contine ”/”(nu va reprezenta o cale).
	*/ 
	if(parent->head_children_dirs != NULL){
		int isFound = 0;
		Dir* aux = NULL;
		Dir* dirr = parent->head_children_dirs;
		Dir* x = parent->head_children_dirs;
		Dir* current = parent->head_children_dirs->next;
		while(x != NULL){
			if(strcmp(x->name, name) == 0){
				isFound = 1;
				break;
			}
			x = x->next;
		}
		if(isFound == 0){
			printf("Could not find the dir\n");
			return;
		}
		if(strcmp(dirr->name, name) == 0){ //daca cel care trebuie sters e primul
	
			aux = parent->head_children_dirs;
			parent->head_children_dirs = parent->head_children_dirs->next;
			if(aux->head_children_files != NULL){
				File *fileinside = aux->head_children_files;
				while(fileinside->next != NULL){
					rm(aux, fileinside->name);
					fileinside = fileinside->next;
				}
			}
			stop(aux);
		}else if(strcmp(current->name, name) == 0){ //daca cel care trebuie sters e al doilea
			aux = dirr->next;
			dirr->next = dirr->next->next;
			if(aux->head_children_files != NULL){
				File *fileinside = aux->head_children_files;
				while(fileinside->next != NULL){
					rm(aux, fileinside->name);
					fileinside = fileinside->next;
				}
			}
			stop(aux);
		}else{
			while(current->next != NULL){
				dirr = current;
				current = current->next;
				if(strcmp(current->name, name) == 0){
					aux = dirr->next;
					dirr->next = dirr->next->next;
					if(aux->head_children_files != NULL){
						File *fileinside = aux->head_children_files;
						while(fileinside->next != NULL){
							rm(aux, fileinside->name);
							fileinside = fileinside->next;
						}
					}
				}
			}
		}
	}else{
		printf("Could not find the dir\n");
	}

}

void cd(Dir** target, char *name) {
	Dir* dir = (*target); //directorul curent
	Dir* current = dir->head_children_dirs;//(*target)->head_children_dirs;
	if(strcmp(name, "..") == 0){
		//schimbarea pt directorul parinte
		if(dir->parent == NULL){
			return; //directorul curent nu are parinte
		}
		if(dir->parent != NULL){ 
			(*target) = (*target)->parent;
			return;
		}
	}
	int contor = 0;
	int isfound = 0;
	while(current!= NULL){
		if(strcmp(current->name,name) == 0){
			isfound = 1;
			break; //in current este director cu care treb schimbat
		}
		current = current->next;
	}
	if(isfound == 0){
		printf("No directories found!\n");
		return;
	}
	*target = current;
}

char *pwd (Dir* target) {
	/*Functia intoarce un sir de caractere ce reprezinta calea de
	 la directorul radacina(home) pana la directorul curent, cu numele target. 
	 Formatul aplicat pentru afisare este: ”/home/(parent_name)/…./(target-name)”. 
	 Rezultatul functiei trebuie afisat la stdout, in afara functiei pwd.
	*/
	if(target == NULL){
		return NULL;
	}

	char* path = (char *)malloc(sizeof(char));
	char aux;
	char *copie;
	path[0] = '\0';
	
	Dir* current = target;
	while(current) {
		path = realloc(path, strlen(path) + 2 + strlen(current->name));


		copie = strdup(current->name);
		for(int i = 0; i < strlen(copie)/2; ++i){
			aux = copie[i];
			copie[i] = copie[strlen(copie) - 1 - i];
			copie[strlen(copie) - 1 - i] = aux;
		}
		strcat(path, copie);
		strcat(path, "/");
		current = current->parent;

		free(copie);
	}

	for(int i = 0; i < strlen(path)/2; ++i){
		aux = path[i];
		path[i] = path[strlen(path) - 1 - i];
		path[strlen(path) - 1 - i] = aux;
	}
	return path;
}



void printTree(char* a, int level) {
    for(int i=0; i<level; i++){
		printf("    ");
	}
    printf("%s\n", a);
}
void tree (Dir* target, int level) {
	if(target == NULL){
		return;
	}
	Dir* t = target->head_children_dirs;
	while(t != NULL){
		printTree(t->name, level);
		tree(t, level+1);
		t = t->next;
	}
	File* f = target->head_children_files;
	while(f != NULL){
		printTree(f->name, level);
		f = f->next;
	}
	return;
}

void mv(Dir* parent, char *oldname, char *newname) {}

int main () {

	int condition = 1;
	int dim;	
	char* str, *cmd, c, *param1;
	Dir* homeDir;
	char* homestr = strdup("home");
	homeDir = createDirector(NULL, homestr);
	Dir* homeDirCopy = homeDir;
	do
	{
		scanf("%c", &c);
		dim = 2;
		str = (char*)malloc(dim*sizeof(char));
		str[0] = c;
		str[1] = '\0';
		while(scanf("%c", &c)){
			if(c == '\n'){
				str[dim - 1] = '\0';
				break;
			}else{
				str = (char *)realloc(str, (dim + 1)*sizeof(char));
				str[dim - 1] = c;
				dim++;
			}
		}
		cmd = strtok(str," ");
		if(cmd == NULL){
			printf("cmd is null!\n");
			exit(-1);
		}
		if(strcmp(cmd, "stop") == 0){
			condition = 0;
			stop(homeDirCopy);
		}else if(strcmp(cmd, "touch") == 0){
			param1 = strtok(NULL, " ");
			if(param1 == NULL){
				printf("comanda touch invalida!\n");
				exit(-1);
			}
			touch(homeDir, param1);
		}else if(strcmp(cmd, "ls") == 0){
			ls(homeDir);
		}else if(strcmp(cmd, "mkdir") == 0){
			param1 = strtok(NULL, " ");
			if(param1 == NULL){
				printf("comanda mkdir invalida!\n");
				exit(-1);
			}
			mkdir(homeDir, param1);
		}else if(strcmp(cmd, "rm") == 0){
			param1 = strtok(NULL, " ");
			if(param1 == NULL){
				printf("comanda rm invalida!\n");
				exit(-1);
			}
			rm(homeDir, param1);
		}else if(strcmp(cmd, "rmdir") == 0){
			param1 = strtok(NULL, " ");
			if(param1 == NULL){
				printf("comanda rmdir invalida!\n");
				exit(-1);
			}
			rmdir(homeDir, param1);
		}else if(strcmp(cmd, "cd") == 0){
			param1 = strtok(NULL, " ");
			if(param1 == NULL){
				printf("comanda cd invalida!\n");
				exit(-1);
			}
			cd(&homeDir, param1);
		}else if(strcmp(cmd, "pwd") == 0){
			char* s = pwd(homeDir);
			printf("%s",s);
			free(s);
		}else if(strcmp(cmd, "tree") == 0){
			tree(homeDir, 0);
		}

		free(str);
		/*Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.*/
		
	} while (condition);

	free(homestr);
	return 0;
}
