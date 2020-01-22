/// Author: Nicholas Graca
#define _GNU_SOURCE
#include "jukebox.h"
#include <stdlib.h>
#include "hash.h"
#include <string.h>
#include <time.h>

// Hashes the song name and artist and adds them together.
long songHash(void* element){
	Song* s = (Song*) element;
	long hash = strHash(s->song) + strHash(s->artist);
	return hash;
}

// Checks if two Song structs are equal by seeing if the song names and artists
// are equal
bool songEquals(void* element1, void* element2){
	Song* s1 = (Song*) element1;
	Song* s2 = (Song*) element2;
	bool song = false;
	bool artist = false;
	if(strcmp(s1->song, s2->song) == 0)
		song = true;
	if(strcmp(s1->artist, s2->artist) == 0)
		artist = true;
	if(song)
		return artist;
	return false;
}
// Prints out the song struct to the way that the instructions specified
void songPrint(void* key, void* value){
	Song* s = (Song*) key;
	long v = (long)value;
	printf("%s by %s, %li times.", ((char*)s->song), ((char*)s->artist), v);
}

// Calculates all the statistics and prints them out according to the
// instructions
void printStatistics(Table* jukebox){
	long count = 0;
	long maxPlays = 0;
	Song* sMax = NULL;
	long minPlays = 9999999;
	Song* sMin = NULL;
	void** key = keys(jukebox);
	for(unsigned int i = 0; i < jukebox->size; i++){
		long temp = (long)get(jukebox, key[i]);
		count = count + temp;
		if(temp < minPlays){
			minPlays = temp;
			sMin = key[i];
		}
		if(temp > maxPlays){
			maxPlays = temp;
			sMax = key[i];
		}
	}

	printf("Jukebox statistics\n==================\n");
	printf("Number of songs in jukebox: %d\n", (int)jukebox->size);
	printf("Number of simulations run: %zu\n", NUM_SIMULATIONS);
	printf("Average number of plays to get duplicate: %lu", count/NUM_SIMULATIONS);
	printf("\nTotal number of songs played: %li\n", count);
	printf("Most played song: ");
	songPrint(sMax, (void*)maxPlays);
	printf("\nLeast played song: ");
	songPrint(sMin, (void*)minPlays);
	printf("\n");
	free(key);
}

// Goes through the file getting the artist and song name then it will create
// a new song struct and put it in the jukebox
Table* loadJukebox(FILE * fp){
	char buf[MAX_LINE_LENGTH];
	char* delimit = "<>\n";
	Table* jukebox = create(songHash, songEquals, songPrint);
	while(fgets(buf, (int)MAX_LINE_LENGTH, fp)){
		char* token = strtok(buf, delimit);
		int i = 0;
		char* song = NULL;
		char* artist = NULL;
		while(token != NULL){
			token = strtok(NULL, delimit);
			if(i == 3){
				song = token;
			}
			if(i == 5){
				artist = token;
			}
			i++;
		}
		Song* s = calloc(1, sizeof(Song));
		s->song = strdup(song);
		s->artist = strdup(artist);	
		if(has(jukebox, s) == false){
			long *v;
			v = 0;
			put(jukebox, s, (void*)v);
		}
		else{
			free(s->song);
			free(s->artist);
			free(s);
		}
	}
	fclose(fp);
	return jukebox;
}
// Will pick a random song and check to see it is already played, if it has,
// it will break out of the infinite loop, if it hasnt already been played it
// will add it to the new list. Either way the value is inremented by 1.
void runSimulation(Table* jukebox){
	int seed = time(0);
	srand(seed);
	void** songs = keys(jukebox);
	for(unsigned int i = 0; i < NUM_SIMULATIONS; i++){
		Table* t = create(songHash, songEquals, songPrint);
		while(true){
			int r = rand() % jukebox->size;
			Song* s = songs[r];
			long value = (long)get(jukebox, s);
			value = value + 1;
			put(jukebox, s, (void*)value);
			if(has(t, s)){	
				break;
			}
			else{
				put(t, s, get(jukebox, s));
			}
		}
		destroy(t);
	}
	free(songs);
}

// Will see if the file is a valid file and will then load, run and print
int main(int argc, char** argv){
	if(argc != 2){
		fprintf(stderr, "Usage: main filename\n");
		exit( EXIT_FAILURE);
	}
	
	FILE* fp = fopen(argv[1], "r");
	if(!fp){
		fprintf(stderr, "Error reading file: {%s}", argv[1]);
	}
	Table* jukebox = loadJukebox(fp);
	runSimulation(jukebox);
	printStatistics(jukebox);
	for(unsigned int i = 0; i < jukebox->capacity; i++){
		if(jukebox->table[i] != NULL){
			Song* s = (Song*)jukebox->table[i]->key;
			free(s->song);
			free(s->artist);
			free(s);
		}
	}
	destroy(jukebox);
	return EXIT_SUCCESS;
}
