// Author: Nicholas Graca
// 10/24/17
// Description: Implement what is defined in mapADT.h

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h> 

typedef struct kvStruct{
	void *key;
	void *value;
} kvPair;

struct mapStruct {
	int  size;
	int capacity;
	bool (*equals)(const void *a, const void *b);
	kvPair table[];
};

typedef struct mapStruct *MapADT;
#define _MAP_IMPL_
#include "mapADT.h"

//creates a new map struct
MapADT map_create(bool(*equals)(const void *a, const void *b)){
	MapADT new = (MapADT) malloc(sizeof(struct mapStruct)+sizeof(struct kvStruct)*5);
	new->size = 0;
	new->capacity = 5; 
	new->equals = equals;
	return new;	
}

// frees the map that is passed in
void map_destroy( MapADT map){
	assert( map !=0);
	map_clear(map);
	free(map);
}

// empties the map of all keys and values
void map_clear(MapADT map){
	for(int i = 0; i<map->size; i++){
		map->table[i].key = NULL;
		map->table[i].value = NULL;
		free(map->table[i].key);
	}
	map->size = 0;	
}

// sees if the supplied key is in the supplied map
bool map_contains(MapADT map, void *key){
	void* value = (void*) map_get(map, key);
		if(value == NULL){
			return false;
	}
	return true;
}

// increases the size of the map supplied and copies everything over to the new, bigger map
MapADT resize(MapADT map){
	int t = (map->capacity)*2;
	int z = (sizeof(struct mapStruct)+sizeof(struct kvStruct)*t);
	fflush(stdout);
	MapADT new = (MapADT) realloc(map, z);
	new->capacity = t;
	return new;
}

// puts a new key value pair in the map but first checks to see if the key
// already exists and if it does, it replaces the value and returns the old
// value
void* map_put(MapADT map, void *key, void *value){
	int size = map->size;
	int capacity = map->capacity;
	void* tempkey = key;
	if((capacity-size) <= 1){
		MapADT new = resize(map);
		*map = *new;
	}
	if(map_contains(map, tempkey)){
		void* old =  (void*) map_get(map, tempkey);
		for(int i = 0; i < map->size; i++){
			if(map->equals(map->table[i].key, tempkey)){
				map->table[i].value = value;
				return old;
			}
		}
	}
	map->table[size].key = tempkey;
	map->table[size].value = value;
	map->size = size + 1;
	return NULL;
}

// gets the value of the linked to the supplied key in the 
// supplied map
const void *map_get(MapADT map, void *key){
	assert(map != 0);
	void* old = NULL;
	for(int i = 0; i < map->size; i++){
		if(map->equals(map->table[i].key, key)){
			old = map->table[i].value;
			return old;
		}
	}
	return NULL;
}

// deletes a key in the supplied map and returns the value
void *map_delete(MapADT map, void *key){
	assert(map->size != 0);
	void* tempkey = key;
	void* entry = (void*) map_get(map, tempkey);
	for(int i = 0; i < map->size; i++){
		fflush(stdout);
		kvPair pair = map->table[i];
		if(map->equals(pair.key, tempkey )){
			map->table[i].key = NULL;
			return entry;
		}
	}
	return NULL;
}
// checks if the map is empty
bool map_empty( MapADT map){
	if(map->size == 0)
		return true;
	return false;
}
