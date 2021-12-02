#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT_LINE_SIZE 300
#define MAX_NAME_SIZE 50

struct Dir;
struct File;

typedef struct Dir{
	char *name; 
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

File* initFile (Dir* parent, char* name) {
	File *aux;
	
	aux = malloc(sizeof(File));   //implicit allocation and verification of a file struct
	if(!aux) {
		printf("Error at allocation of new File.");
		free(aux);
		return NULL;
	}
	
	aux->name = malloc(MAX_NAME_SIZE * sizeof(char));
	if(!aux->name) {
		printf("Error at allocation of new File.");
		free(aux->name);
		free(aux);
		return NULL;
	}

	strcpy(aux->name, name);
	aux->parent = parent;
	aux->next = NULL;
	
	return aux;
}

Dir* initDir (Dir* parent, char* name) {
	Dir *aux;

	aux = malloc(sizeof(Dir));   //implicit allocation and verification of a dir struct
	if(!aux) {
		printf("Error at allocation of new Directory.");
		free(aux);
		return NULL;
	}
	
	aux->name = malloc(MAX_NAME_SIZE * sizeof(char));
	if(!aux->name) {
		printf("Error at allocation of new Directory.");
		free(aux->name);
		free(aux);
		return NULL;
	}

	strcpy(aux->name, name);
	aux->parent = parent;
	aux->head_children_files = NULL;
	aux->head_children_dirs = NULL;
	aux->next = NULL;

	return aux;
}

void touch (Dir* parent, char* name) {
	File *crawlF, *new;
	Dir *crawlD;
	
	for (crawlF = parent->head_children_files; crawlF != NULL; crawlF = crawlF->next) {  //verify if the list have the new file name, file list
		
		if(strcmp(crawlF->name, name) == 0) {
			printf("File already exists\n");
			return;
		}
	}

	for (crawlD = parent->head_children_dirs; crawlD != NULL; crawlD = crawlD->next) {  //verify if the list have the new file name, dir list
		
		if (strcmp(crawlD->name, name) == 0) {
			printf("File already exists\n");
			return;
		}
	}

	new = initFile(parent, name);  //new file struct

	if (!parent->head_children_files)  //if the list file is empty
		parent->head_children_files = new;
	
	else {
		for (crawlF = parent->head_children_files; crawlF->next != NULL; crawlF = crawlF->next);  //insertion at final position in list
		crawlF->next = new;
	}
}

void mkdir (Dir* parent, char* name) {
	File *crawlF;
	Dir *crawlD, *new;
	
	for (crawlF = parent->head_children_files; crawlF != NULL; crawlF = crawlF->next) {  //verify if the list have the new dir name, file list
		
		if(strcmp(crawlF->name, name) == 0) {
			printf("Directory already exists\n");
			return;
		}
	}

	for (crawlD = parent->head_children_dirs; crawlD != NULL; crawlD = crawlD->next) {  //verify if the list have the new dir name, dir list
		
		if(strcmp(crawlD->name, name) == 0) {
			printf("Directory already exists\n");
			return;
		}
	}

	new = initDir(parent, name);  //new dir struct

	if (!parent->head_children_dirs)  //if the list file is empty
		parent->head_children_dirs = new;
	else {
		for(crawlD = parent->head_children_dirs; crawlD->next != NULL; crawlD = crawlD->next);  //insertion at final position in list
		crawlD->next = new;
	}

}

void ls (Dir* parent) {
	File *crawlF;
	Dir *crawlD;

	if (parent->head_children_dirs) {  //verify if the file list is empty
		for(crawlD = parent->head_children_dirs; crawlD != NULL; crawlD = crawlD->next)
			printf("%s\n", crawlD->name);
	}
	
	if (parent->head_children_files) {  //verify if the dir list is empty
		for(crawlF = parent->head_children_files; crawlF != NULL; crawlF = crawlF->next)
			printf("%s\n", crawlF->name);
	}
}

void rm (Dir* parent, char* name) {
	File *crawlF, *crawlBack;
	
	if (!parent->head_children_files)  //1st case, file list is empty
		printf("Could not find the file\n");
	
	else if (strcmp(parent->head_children_files->name, name) == 0) {  //2nd case, file struct is in the first position 
		crawlF = parent->head_children_files;
		parent->head_children_files = parent->head_children_files->next;  //modify head of list
		
		free(crawlF->name);
		free(crawlF);
	}

	else {  //3th case, the file is in middle or in last position
		for (crawlF = parent->head_children_files; crawlF != NULL; crawlBack = crawlF, crawlF = crawlF->next) {
			if (strcmp(crawlF->name, name) == 0) {
				crawlBack->next = crawlF->next;

				free(crawlF->name);  //free memory
				free(crawlF);
				
				return;
			}
		}

		printf("Could not find the file\n");  //if the function doesnt return, the list dont have the file
	}

}

void rmdir (Dir* parent, char* name) {
	Dir *crawlD, *crawlBack;
	File *crawlF, *aux;

	if(!parent->head_children_dirs)  //1st case, dir list is empty
		printf("Could not find the dir\n");
	
	else if (strcmp(parent->head_children_dirs->name, name) == 0) { //2nd case, file struct is in the first position 
		crawlD = parent->head_children_dirs;
		parent->head_children_dirs = parent->head_children_dirs->next; //modify head of list

		crawlF = crawlD->head_children_files;
		
		while (crawlF) {  //delete all the files
			aux = crawlF;
			crawlF = crawlF->next;
			free(aux->name);
			free(aux);
		}

		free(crawlD->name);  //free memory
		free(crawlD);
	}

	else {  //3th case, the file is in middle or in last position
		for (crawlD = parent->head_children_dirs; crawlD != NULL; crawlBack = crawlD, crawlD = crawlD->next) {
			if (strcmp(crawlD->name, name) == 0) {
				crawlBack->next = crawlD->next;

				crawlF = crawlD->head_children_files;
		
				while (crawlF) {  //delete all the files
					aux = crawlF;
					crawlF = crawlF->next;
					free(aux->name);
					free(aux);
				}

				free(crawlD->name);  //free memory
				free(crawlD);
				return;
			}
		}

		printf("Could not find the dir\n");  //if the function doesnt return, the list dont have the dir
	}

}

void cd(Dir** target, char *name) {
	Dir *crawlD, *currentDir;
	currentDir = *target; //simple cast from address to pointer

	if (strcmp(name, "..") == 0) {
		if (!currentDir->parent)  // cd .. in home, we dont have parent :(
			return;
		else
			*target = currentDir->parent;  // go to parent directory
	}
	
	else {
		
		for (crawlD = currentDir->head_children_dirs; crawlD != NULL; crawlD = crawlD->next) {  //search name in directory list
			if (strcmp(crawlD->name, name) == 0) {
				*target = crawlD;
				return;
			}
		}

		printf("No directories found!\n");
		
	}
}

char *pwd (Dir* target) {
	static char *results;
	if (!target->parent) {
		results = malloc(MAX_NAME_SIZE * sizeof(char));  //alloc happends only once! when we hit home in recursion
		strcat(results, "/");
		strcat(results, target->name);
	}
	
	else {
		pwd(target->parent);
		strcat(results, "/");
		strcat(results, target->name);  //when the function get back here -> its concatenate the strings
	}
	
	return results;  
}

void freeMem(Dir** target) {
	Dir *crawlD;
	File *crawlF;

	crawlD = *target;

	if(!(*target)) return;  //recursion condition

	while ((*target)->head_children_files) {  //delete all the files
		crawlF = (*target)->head_children_files->next;
		free((*target)->head_children_files->name);
		free((*target)->head_children_files);
		(*target)->head_children_files = crawlF;
	}

	while ((*target)->head_children_dirs) { //delete all the dirs
		crawlD = (*target)->head_children_dirs->next;
		freeMem(&((*target)->head_children_dirs));  //recursion call on dir for all subdirectors
		free((*target)->head_children_dirs->name);
		free((*target)->head_children_dirs);
		(*target)->head_children_dirs = crawlD;
	}
}	

void stop (Dir* target) {
	freeMem(&target);  //function that clear the memory
	free(target->name);  
	free(target);  //delete home folder
	exit(0);
}

void tree (Dir* target, int level) {
	Dir* crawlD;
	File* crawlF;
	
	for (crawlD = target->head_children_dirs; crawlD != NULL; crawlD = crawlD->next) {
		for(int i = 0; i < level; i++)
			printf("    ");
		printf("%s\n", crawlD->name);
		tree(crawlD, level + 1);
	}
	
	for (crawlF = target->head_children_files; crawlF != NULL; crawlF = crawlF->next) {
		for(int i = 0; i < level; i++)
			printf("    ");
		printf("%s\n", crawlF->name);
	}
	
}

void mv (Dir* parent, char *oldname, char *newname) {
	Dir *crawlD, *oldCrawlD, *backCrawlD, *newCrawlD;
	File *crawlF;

	for (crawlF = parent->head_children_files; crawlF != NULL; crawlF = crawlF->next) { //search for same oldname in files
		if (strcmp(crawlF->name, oldname) == 0) {
			for (crawlF = parent->head_children_files; crawlF != NULL; crawlF = crawlF->next) { //search for same newname in files
				if(strcmp(crawlF->name, newname) == 0) {
					printf("File/Director already exists\n");
					return;
				}
			}
		
			for (crawlD = parent->head_children_dirs; crawlD != NULL; crawlD = crawlD->next) { //search for same newname in directors
				if(strcmp(crawlD->name, newname) == 0) {
					printf("File/Director already exists\n");
					return;
				}
			}

			rm(parent, oldname); 
			touch(parent, newname);  //reuse the rm and touch functions
			return;
		}
			
	}

	for (oldCrawlD = parent->head_children_dirs; oldCrawlD != NULL; oldCrawlD = oldCrawlD->next) { //search for same oldname in directors
		if (strcmp(oldCrawlD->name, oldname) == 0) {
			for (crawlF = parent->head_children_files; crawlF != NULL; crawlF = crawlF->next) { //search for same newname in files
				if(strcmp(crawlF->name, newname) == 0) {
					printf("File/Director already exists\n");
					return;
				}
			}
		
			for (crawlD = parent->head_children_dirs; crawlD != NULL; crawlD = crawlD->next) { //search for same newname in directors
				if(strcmp(crawlD->name, newname) == 0) {
					printf("File/Director already exists\n");
					return;
				}
			}

			mkdir(parent, newname);

			if (strcmp(parent->head_children_dirs->name, oldname) == 0) { //first directory in list
				parent->head_children_dirs = parent->head_children_dirs->next; //modification of list head
				
				for (newCrawlD = parent->head_children_dirs; newCrawlD->next != NULL; newCrawlD = newCrawlD->next); //new directory is in last position
				newCrawlD->head_children_dirs = oldCrawlD->head_children_dirs;
				newCrawlD->head_children_files = oldCrawlD->head_children_files;
				
				free(oldCrawlD->name);
				free(oldCrawlD);
				
				return;
			}

			else { //directory in the middle position in list
				for (backCrawlD = NULL, oldCrawlD = parent->head_children_dirs; strcmp(oldCrawlD->name, oldname); backCrawlD = oldCrawlD, oldCrawlD = oldCrawlD->next);
				backCrawlD->next = oldCrawlD->next;
				
				for (newCrawlD = parent->head_children_dirs; newCrawlD->next != NULL; newCrawlD = newCrawlD->next); //new directory is in last position
				newCrawlD->head_children_dirs = oldCrawlD->head_children_dirs;
				newCrawlD->head_children_files = oldCrawlD->head_children_files;
				
				free(oldCrawlD->name);
				free(oldCrawlD);
				
				return;
			}
		}
	}

	printf("File/Director not found\n");

}

int main () {
	
	char *line, *function, *name, *newname, *oldname; //declaration of variables
	line = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));
	
	Dir* home, *currentDir; //create dir HOME and currentDir->HOME
	home = initDir(NULL, "home");
	currentDir = home;

	do {
		fgets(line, MAX_INPUT_LINE_SIZE, stdin); //read the input
		line[strlen(line) - 1] = '\0';

		function = strtok(line, " \0");

		//execute for every input
		if (strcmp(function, "touch") == 0) {
			name = strtok(NULL, "\0");
			touch(currentDir, name);
		}

		else if (strcmp(function, "mkdir") == 0) {
			name = strtok(NULL, "\0");
			mkdir(currentDir, name);
		}

		else if (strcmp(function, "ls") == 0) {
			ls(currentDir);
		}

		else if (strcmp(function, "rm") == 0) {
			name = strtok(NULL, "\0");
			rm(currentDir, name);
		}

		else if (strcmp(function, "rmdir") == 0) {
			name = strtok(NULL, "\0");
			rmdir(currentDir, name);
		}

		else if (strcmp(function, "cd") == 0) {
			name = strtok(NULL, "\0");
			cd(&currentDir, name);
		}
		
		else if (strcmp(function, "tree") == 0) {
			name = strtok(NULL, "\0");
			tree(currentDir, 0);
		}

		else if (strcmp(function, "pwd") == 0) {
			char *results = pwd(currentDir);
			printf("%s", results);
			free(results);
		}
		
		else if (strcmp(function, "stop") == 0) {
			free(line);
			stop(home);
		}

		else if (strcmp(function, "mv") == 0) {
			oldname = strtok(NULL, " ");
			newname = strtok(NULL, "\0");
			mv(currentDir, oldname, newname);
		}

	} while (true);
}
