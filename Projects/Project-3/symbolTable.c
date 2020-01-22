// Author: Nicholas Graca 
// Date: 12/4/17
//This is my modified hash table from project 2

#include "symbolTable.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/**
Create a pointer to the Dictionary and populates it with defeault values.
Also allocates space for a Symbol array to hold the variables.
**/
Table* create(){
	Table* Hmap = (Table*) calloc(1, sizeof(struct Table_t));
	Hmap->table  = calloc(10, sizeof(Symbol*));
	if (Hmap->table == NULL) {
        fprintf(stderr, "table::create() failed to allocate space");
        assert(NULL);
    }
	Hmap->size = 0;
	Hmap->capacity = 10;
	Hmap->rehashes = 0;
	return Hmap;
}

/**
Goes through each index in the table and frees it, then it frees the Symbol
Table and then frees the Hash Map.
**/
void destroy(Table* t){
	for(unsigned int i = 0; i < t->capacity; i++){
		if(t->table[i] != NULL)
			free(t->table[i]);
	}
	free(t->table);
	free(t);
}

/**
Prints out the data in the format that is defined in the instructions 
**/
void dump(char* name, Value value, Type type){
	int len = strlen(name);
	char* varType;
	if(type == Integer){
		varType = "integer";
	}
	if(type == Float){
		varType = "real";
	}
	if(type == Unknown){
		varType = "Unknown";
	}
	printf("%s", name);
	for(int i = 0; i < (8 - len); i++){
		printf(" ");
	}
	printf("%s", varType);
	for(unsigned int i = 0; i < (8 - (strlen(varType))); i++){
		printf(" ");
	}
	if(strcmp(varType, "integer") == false){
		printf("%d\n", value.iVal);
		return;	
	}
	printf("%.3f\n", value.fVal);
	return;
}

/**
Returns the Symbol pointer that is associated with the name that is passed in.
**/
Symbol* get(Table* t, char* name){
	int max = (int)t->capacity;	
	for(int x = 0; x < max; x++){
		if(strcmp(name, t->table[x]->name) == 0){
			return t->table[x];
		}
	}
	return NULL;
}

/**
Checks to see if the name is already in the table
**/
bool has(Table* t, char* name){
	int max = (int)t->capacity;
	for(int x = 0; x < max; x++){
		if(t->table[x] != NULL){
			if(strcmp(name, t->table[x]->name) == 0){
				return true;
			}
		}
	}
	return false;
}

/**
Increases the size of the table and puts all the old names
 in the new table as well as updating the size of capacity
**/
void rehash(Table* t){
	int capacity = t->capacity*RESIZE_FACTOR;
	Symbol** old = t->table;
	int oldSize = (int)t->size;
	t->table = calloc(capacity, sizeof(struct Entry*));
	if (t->table == NULL) {
        fprintf(stderr, "table::create() failed to allocate space");
        assert(NULL);
    }
	t->capacity = capacity;
	t->size = 0;
	for( int i = 0; i < oldSize; i++){
		if(old[i] != NULL){	
			put(t, old[i]->name, old[i]->value, old[i]->type);
			free(old[i]);
		}
	}
	free(old);
}

/**
Checks if the table size needs to be increased. Put the Symbol pointer
in the next open spot in the table
**/
void put(Table* t, char* name, Value v, Type type){
	Symbol* new = calloc(1, sizeof(Symbol));
	if(name == NULL){
		return;
	}
	float size = (float)t->size;
	float cap = (float)t->capacity;
	if(size/cap >= LOAD_THRESHOLD){
		rehash(t);
	}
	
	int insert = t->size;
	new->value = v;
	strncpy(new->name, name, 7);
	new->type = type;
	t->table[insert] = new;
	t->size = t->size + 1;
}

/**
Puts the table in alphabetical order and then prints each variable in the table
**/
void dumpSymbolTable(Table* t){
	for(unsigned int i = 0; i < t->size; i++){
		for(unsigned int x = 0; x < t->size; x++){
			if(strcmp(t->table[i]->name, t->table[x]->name) < 0){
				Symbol* s1 = t->table[i];
				Symbol* s2 = t->table[x];
				t->table[i] = s2;
				t->table[x] = s1;
			}
		
		}
	}
	printf("\nSymbol Table Contents\nName    Type    Value\n=====================\n");
	for(unsigned int i = 0; i < t->size; i++){
		dump(t->table[i]->name, t->table[i]->value, t->table[i]->type);
	}
}

/**
Reads through a file and puts entries into a symbol table
**/
Table* readSymbolTable(FILE* fp, Table* t){
	char line[100];
	if(t == NULL){
		t = create();
	}
	char* tok;
	//gets one line in the file
	while(fgets(line, 100, fp)){
		Value value;
		Type type;
		// get the type of variable
		tok = strtok(line, " \n");
		char* varType = tok;
		if(strcmp(varType, "Integer") == 0 || strcmp(varType, "integer") == 0){
			type = Integer;
		}
		else if(strcmp(varType, "Float") == 0 || strcmp(varType, "real") == 0){
			type = Float;
		}
		else if(strcmp(varType, "Unknown") ==0){
			type = Unknown;
		}
		char* name;
		//get the name of the variable
		tok = strtok(NULL, " \n");
		name = tok;
		//get the value of the variable and assign it to either a int or a float
		tok = strtok(NULL, " \n");
		if(strcmp(varType, "Integer") == 0 || strcmp(varType, "integer") == 0){
			value.iVal = atoi(tok);
		}
		else{
			value.fVal = atof(tok);
		}
		put(t, name, value, type);
	}
	fclose(fp);
	return t;
}
