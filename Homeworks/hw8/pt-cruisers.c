// Author: Nicholas Graca
// 11/27/17
/* This program reads in the command line arguments, set the delay, get the
names, make the racers and make the threads after the threads run, free all
memory and join the threads
*/

#define _GNU_SOURCE
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "racer.h"
#include <ctype.h>
#include "display.h"
#include <string.h>

int main(int argc, char** argv){
	if(argc < 2){
		fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
		return EXIT_FAILURE;
	}
	char names[argc-1][7];
	long delay = DEFAULT_WAIT;
	int i = 0;
	int numNames = 0;
	//check if the first command line argument is a digit
	if(isdigit(argv[1][0]) || argv[1][0] == '.'){
		//if there is only the delay and a name throw an error
		if(argc < 4){
			fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
			return EXIT_FAILURE;
		}
		//set the speed delay to that
		int delay = atoi(argv[1]);
		delay = delay + 0;
		numNames = argc - 2;
		initRacers(delay);
	}

	else{
		//if there are not two names throw an error
		if(argc < 3){
			fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
			return EXIT_FAILURE;
		}
		//If the name is longer than 6 characters throw an error
		if(strlen(argv[1]) > 6){
			fprintf(stderr, "Error: racer names must not exceed lenth 6\n");
			return EXIT_FAILURE;
		}
		numNames = argc-1;
		i = 1;
		// put the first command line arugment that is a name in an array
		char* name = argv[1];
		strcpy((names[0]), name);
		initRacers(delay);
	}
	//get the rest of the names and check that they are a good length
	for(int x = 2; x < argc; x++){
		if(strlen(argv[x]) > 6){
			fprintf(stderr, "Error: racer names must not exceed length 6\n");
			return EXIT_FAILURE;
		}
		if(argv[x][0] == ' '){
			fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
			return EXIT_FAILURE;
		}
		// copy the rest of the names into the array
		strcpy((char*)names[i], (char*)argv[x]);
		i++;
	}
	//create an array of racers
	Racer* racers[numNames];
	//make all the racer structs with the names and put them in the array
	for(int y = 0; y < numNames; y++){
		if(names[y] != NULL){
			char * name = (char*)names[y];
			racers[y] = makeRacer(name, y);
		}
	}
	clear();
	set_cur_pos(0,0);
	pthread_t threads[numNames];
	//create threads for the amount of names given
	for(int t = 0; t < numNames; t++){
		int j = pthread_create(&threads[t], NULL, run, (void*)racers[t]);
		if(j){
			fprintf(stderr, "Error: pthread_create() returned %d\n", j);
		}
	}
	//free all the memory
	for(int t = 0; t < numNames; t++){
		pthread_join(threads[t], NULL);
		destroyRacer((Racer*)racers[t]);
	}
	//move cursor down
	set_cur_pos(argc, 0);
}
