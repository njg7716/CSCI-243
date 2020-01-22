// Author: Nicholas Graca
// Date: 11/29/17

#define _BSD_SOURCE
#define _GNU_SOURCE
#include <stdlib.h>
#include "racer.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h> 
#include "display.h"

long maxDelay = 0;
pthread_mutex_t mymutex;

//set all the variables before the run function
void initRacers(long milliseconds){
	maxDelay = milliseconds;
	srand(time(NULL));
}

// Make a new Raacer struct
Racer *makeRacer(char *name, int position){
	Racer* r = malloc(sizeof(Racer));
	r->dist = 0;
	r->row = position + 1;
	char* pre = malloc(MAX_CAR_LEN+1);
	strcat(pre, "~O=");
	strcat(pre, name);
	//Puts the correct number of '-' depending on the length of the name
	for(unsigned int i = 0; i < (6 - strlen(name)); i++){
		strcat(pre, "-");
	}
	char* final = "o>";
	strcat(pre, final);
	r->graphic = pre;
	return r;
}

//free the racer's name and the racer struct
void destroyRacer(Racer *racer){
 	free(racer->graphic);
	free(racer);
}

//This is what makes everything move
void *run(void *racer){	
	Racer* r = racer;
	//Run until the racer is at the finish line
	while(r->dist != FINISH_LINE){
		long delay = rand() % maxDelay;
		usleep(delay);

		//only one thread can access this code at once
		pthread_mutex_lock(&mymutex);
		//if true, give the racer a flat and exit
		if(delay < 3){
			set_cur_pos(r->row, r->dist);
			for(unsigned int i = 0; i < strlen(r->graphic); i++){
				put((r->graphic)[i]);
			}
			int cur = (r->dist)+1;
			if((r->dist) == 0){
				cur = (r->dist)+2;
			}
			set_cur_pos(r->row, cur);
			put('X');
			pthread_mutex_unlock(&mymutex);
			return NULL;
		}
		else{
			//clears the line of any printed characters
			for(int i = 0; i < 80; i++){
				set_cur_pos(r->row,i);
				put(' ');
			}
			//moves the racer to the right one space and prints the graphic
			r->dist = r->dist+1;
			set_cur_pos(r->row, r->dist);
			for(unsigned int i = 0; i < strlen(r->graphic); i++){
				put((r->graphic)[i]);
			}
		}
		//sleep so that you can follow the movements
		usleep(70000);

		//now you can let any other thread go
		pthread_mutex_unlock(&mymutex);
	}
	return NULL;
}
