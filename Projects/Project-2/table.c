// Author: Nicholas Graca
// Mechanics of Programming Project 2
// 11/6/2017

#include "table.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/**
Create a pointer to the Hash Map and populates it with defeault values.
Also allocates space for an Entry array to hold the keys and values.
**/
Table* create(long (*hash)(void* key), 
			  bool (*equals)(void* key1, void* key2),
			  void (*print)(void* key1, void* key2)){
	Table* Hmap = (Table*) calloc(1, sizeof(struct Table_t));
	Hmap->table  = calloc(INITIAL_CAPACITY, sizeof(Entry*));
	if (Hmap->table == NULL) {
        fprintf(stderr, "table::create() failed to allocate space");
        assert(NULL);
    }
	Hmap->hash = hash;
	Hmap->equals = equals;
	Hmap->print = print;
	Hmap->size = 0;
	Hmap->capacity = INITIAL_CAPACITY;
	Hmap->collisions = 0;
	Hmap->rehashes = 0;
	return Hmap;
}

/**
Goes through each index in the table and frees it, then it frees the Entry
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
void dump(Table* t, bool full){
	printf("Size: %lu\nCapacity: %lu\nCollisions: %lu\nRehashes: %lu\n"
			, t->size, t->capacity, t->collisions, t->rehashes);
	if(full){
		for(unsigned int i = 0; i < t->capacity; i++){
			if(t->table[i] != NULL){
				printf("%d: (", i);
				t->print(t->table[i]->key, t->table[i]->value);
				printf(")\n");
			}
			else{
				printf("%d: null\n", i);
			}
		}
	}
}

/**
Returns the value that is associated with the key that is passed in.
**/
void* get(Table* t, void* key){
	long hash = t->hash(key);
	hash = hash % t->capacity;
	int i = hash;
	int max = (int)t->capacity;
	for(int x = 0; x < 2; x++){
		for(;i < max; i++){
			if(t->table[i] != NULL){
				if(t->equals(t->table[i]->key, key)){
					return t->table[i]->value;
				}
				t->collisions++;
			}
			else{
				return NULL;
			}
		}
	i = 0;
	max = hash;
	}
	return NULL;
}

/**
Checks to see if the key passed in is already in the table and returns true
or false
**/
bool has(Table* t, void* key){
	long hash =  t->hash(key);
	hash = hash % t->capacity;
	int i = hash;
	int max = (int)t->capacity;
	for(int x = 0; x < 2; x++){
		for(;i < max; i++){
			if(t->table[i] != NULL){
				if(t->equals(t->table[i]->key, key)){
					return true;
				}
				t->collisions++;
			}
			else{
				return false;
			}
		}
	i = 0;
	max = hash;
	}
	return false;
}

/**
Returns a list of all the keys in the table that are not NULL
**/
void** keys(Table* t){
	void** collec = malloc(t->size*sizeof(void**));
	if (collec == NULL) {
        fprintf(stderr, "keys::create() failed to allocate space");
        assert(NULL);
    }
	// Variable count is to make sure that it does not increment past
	// the size of the count since it is going up to the capacity of the table
	int count = 0;
	for(unsigned int i = 0; i < t->capacity; i++){
		if(t->table[i] != NULL){
			collec[count] = t->table[i]->key;
			count++;
		}
	}
	return collec;
}

/**
Increases the size of the table in the hash map and puts all the old keys
and values in the new table as well as updating the size of capacity
**/
void rehash(Table* t){
	t->rehashes++;
	int capacity = t->capacity*RESIZE_FACTOR;
	Entry** old = t->table;
	int oldCap = (int)t->capacity;
	t->table = calloc(capacity, sizeof(struct Entry*));
	if (t->table == NULL) {
        fprintf(stderr, "table::create() failed to allocate space");
        assert(NULL);
    }
	t->capacity = capacity;
	t->size = 0;
	for( int i = 0; i < oldCap; i++){
		if(old[i] != NULL){	
			put(t, old[i]->key, old[i]->value);
			free(old[i]);
		}
	}
	free(old);
}

/**
Checks if the table size needs to be increased, if the key is already in the
table it will update the value and return the old value. If two things get
hashed to the same place, it uses open addressing to find the next open spot
to put the key and value. Otherwise, it will just put the key and value at
the location provided by the hash function.
**/
void* put(Table* t, void* key, void* value){
	if(key == NULL)
		return NULL;
	float size = (float)t->size;
	float cap = (float)t->capacity;

	if(size/cap >= LOAD_THRESHOLD){
		rehash(t);
	}
	long hash = t->hash(key);
	int insert = (int) hash%t->capacity;

	// If there is nothing in the postion that the key hashed to, just add it
	if(t->table[insert] == NULL){
		Entry* new = calloc(1,sizeof(struct Entry_t));
		t->table[insert] = new;
		t->table[insert]->key = key;
		t->table[insert]->value = value;
		t->size++;
		return NULL;
	}

	while(t->table[insert] != NULL){
		if(t->equals(t->table[insert]->key, key)){
			//if the keys are the same, update the value and return the old one
			void* old = t->table[insert]->value;
			t->table[insert]->value = value;
			return old;
		}
		// if the keys are not the same then there is a collision
		t->collisions++;
		insert++;
		// if the loop reaches the end of the map, this sends it to the front
		if(insert == (int)t->capacity){
			insert = 0;
		}
	}
	// This is the open addressing
	Entry* new = calloc(1,sizeof(struct Entry_t));
	t->table[insert] = new;
	t->table[insert]->key = key;
	t->table[insert]->value = value;
	t->size++;
	return NULL;
}

/**
Returns a list of all the values in the table that are not NULL.
**/
void** values(Table* t){
	void** collec = (void**)calloc(t->capacity, sizeof(void**));
	if (collec == NULL) {
        fprintf(stderr, "values::create() failed to allocate space");
        assert(NULL);
    }
	// Variable count is to make sure that it does not increment past
	// the size of the list since it iterates up to the capacity of the table
	int count = 0;
	for(unsigned int i = 0; i < t->capacity; i++){
		if(t->table[i] != NULL){
			collec[count] = t->table[i]->value;
			count++;
		}
	}
	return collec;
}
