/// Author: Nicholas Graca
/// Mechanics Project 1: Brace-Topia 


#define _BSD_SOURCE 
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Initialize all default values
float average = 0;
int delay = 900000;
int count = -1;
int size = 15;
double vacancies = .20;
double end = .60;
double threshold = .50;
int cycle = 0;
int move = 1;
float total = 0;

//Finds the number of neighbors that are endline people (the right way)
double getENeighbors(int row, int col, int size, char neighborhood[][size]){
	double e = 0;
	if(neighborhood[row-1][col-1] == 'e')
			e++;
	if(neighborhood[row-1][col] == 'e')
			e++;
	if(neighborhood[row-1][col+1] == 'e')
			e++;
	if(neighborhood[row][col-1] == 'e')
			e++;
	if(neighborhood[row][col+1] == 'e')
			e++;
	if(neighborhood[row+1][col-1] == 'e')
			e++;
	if(neighborhood[row+1][col] == 'e')
			e++;
	if(neighborhood[row+1][col+1] == 'e')
			e++;
	return e;
}

//Finds the number of neighbors that are newline people (the cancerous way)
double getNNeighbors(int row, int col, int size, char neighborhood[][size]){
	double n = 0;
	if(neighborhood[row-1][col-1] == 'n')
		n++;
	if(neighborhood[row-1][col] == 'n')
		n++;
	if(neighborhood[row-1][col+1] == 'n')
		n++;
	if(neighborhood[row][col-1] == 'n')
		n++;
	if(neighborhood[row][col+1] == 'n')
		n++;
	if(neighborhood[row+1][col-1] == 'n')
		n++;
	if(neighborhood[row+1][col] == 'n')
		n++;
	if(neighborhood[row+1][col+1] == 'n')
		n++;
	return n;
}

//prints the grid depending on if it is in infinite mode or not
void printNeighborhood(int size, char neighborhood[][size]){
	//If Infinite mode, dont print multiple grids
	if(count == -1)
		set_cur_pos(1,1);
	for(int i = 0; i < size; i++){
		for(int x = 0; x < size; x++){
			printf("%c", neighborhood[i][x]);
		}
		printf("\n");
	}
	printf("\n");
	printf("Cycle: %d\n", cycle);
	printf("Moves this cycle: %d\n", move);
	move++;
	printf("teams' \"happiness\": %3.0f%%\n", average*100);
	printf("dim: %d, strength of preference: %3.0f%%, vacancy: %3.0f%%, end: %3.0f%%\n", size, threshold*100, vacancies*100, end*100);
	if(count == -1){
		printf("Use Control-C to quit.\n");
		usleep(delay);
		return;
	}
	printf("\n");
	usleep(delay);
}

//Moves the unhappy agent and prints the change to the grid
void relocate(int size, char newNeighborhood[][size], char copy[][size], int row, int col){
	char tenant = copy[row][col];
	for(int i = 0; i < size; i++){
		for(int x = 0; x < size; x++){
			if(copy[i][x] == '.'){
				copy[i][x] = tenant;
				newNeighborhood[i][x] = tenant;
				newNeighborhood[row][col] = '.';
				return;
			}
		}
		
	}
} 

//Calls the other functions to find if the agent is happy and if not,
// it wall relocate the agent. Also calculates the average happiness
void getHappy(int size, char neighborhood[][size]){
	float e = 0;
	float n = 0;
	average = 0;
	char newNeighborhood[size][size];
	char copy[size][size];
	//Makes a copy of the neighborhood
	for(int r = 0; r < size; r++){
		for( int c = 0; c < size; c++){
			newNeighborhood[r][c] = neighborhood[r][c];
			copy[r][c] = neighborhood[r][c];
		}
	}
	for(int r = 0; r < size; r++){
		for( int c = 0; c < size; c++){
			double percent = 0;
			n = getNNeighbors(r, c, size, neighborhood);
			e = getENeighbors(r, c, size, neighborhood);
			if(neighborhood[r][c] == 'n'){
				if(n == 0 && e == 0)
					percent = 1;
				else
					percent = n/(e+n);
				average = average + percent;
				if(percent < threshold){
				relocate(size, newNeighborhood, copy, r, c);
				}
			}
			if(neighborhood[r][c] == 'e'){
				if(n == 0 && e == 0)
					percent = 1;
				else
					percent = e/(n+e);
				average = average + percent;
				if(percent < threshold){
				relocate(size, newNeighborhood, copy, r, c);
				}
			}
			
		}
	}
	average =  average / total;
	//Makes the changes to the original from the copy
	for(int r = 0; r < size; r++){
		for( int c = 0; c < size; c++){
			neighborhood[r][c] = newNeighborhood[r][c];
		}
	}
	printNeighborhood(size, neighborhood);
}

// Handles all the command line arguments and will adjust the deafult values arcording to what
// the user defines. This will also call getHappy to start the whole process and depending on
// if it is infinite mode or not, it will change how the process is run.
int main(int argc, char* argv[]){
	srand(41);
	argv[0] = "brace-topia";
	int opt;
    int tmpsize = 0;
	//Handles command line arguments
    while ( (opt = getopt( argc, argv, "ht:c:d:s:v:e:") ) != -1 ) {
        switch ( opt ) {
			case 'h':
				fprintf(stderr, "usage:\n");
				fprintf(stderr, "brace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v %%vac] [-e %%end]\n");
				fprintf(stderr, "Option        Default Example Description\n");
				fprintf(stderr, "'-h'          NA      -h      print this usage message.\n");
				fprintf(stderr, "'-t N'        900000  -t 5000 microseconds cycle delay.\n");
				fprintf(stderr, "'-c N'        -1      -c4     count cycle maximum value.\n");
				fprintf(stderr, "'-d dim'      15      -d 7    width and height dimension.\n");
				fprintf(stderr, "'-s %%str'     50      -s 30   strength of preference.\n");
				fprintf(stderr, "'-v %%vac'     20      -v30    percent vacancies.\n");
				fprintf(stderr, "'-e %%end'     60      -e75    percent Endline braces. Others want Newline.\n");
				return EXIT_SUCCESS;
			case 't':
				tmpsize = (int)strtol( optarg, NULL, 10 );
				if(tmpsize < 0)
					break;
				delay = tmpsize;
				break;
			case 'c':
				tmpsize = (int)strtol( optarg, NULL, 10 );
				if(tmpsize <= 0){
					fprintf(stderr, "count (%d) must be a non-negative integer.\n", tmpsize);
					fprintf(stderr, "usage:\nbrace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v %%vac] [-e %%end]\n");
					exit( EXIT_FAILURE );
				}
				count = tmpsize;
				break;
			case 'd':
				tmpsize = (int)strtol( optarg, NULL, 10 );
				if(tmpsize < 5 || tmpsize > 39){
					fprintf(stderr, "dimension (%d) must be a value in [5...39]\n", tmpsize);
					fprintf(stderr, "usage:\nbrace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v %%vac] [-e %%end]");
					exit( EXIT_FAILURE );
				}
			
				size = tmpsize;
				break;
			case 's':
				tmpsize = (double)strtol( optarg, NULL, 10 );
				if(tmpsize < 1 || tmpsize > 99){
					fprintf(stderr, "prefrence strength (%d) must be a value in [1...99]", tmpsize);
					fprintf(stderr, "usage:\nbrace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v %%vac] [-e %%end]");
					exit( EXIT_FAILURE );
				}
				threshold = tmpsize;
				break;
			case 'v':
				tmpsize = (double)strtol( optarg, NULL, 10 );
				if(tmpsize < 1 || tmpsize > 99){
					fprintf(stderr, "vacancy (%d) must be a value in [1...99]", tmpsize);
					fprintf(stderr, "usage:\nbrace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v %%vac] [-e %%end]");
					exit( EXIT_FAILURE );
				}
				vacancies = tmpsize;
				break;
			case 'e':
				tmpsize = (double)strtol( optarg, NULL, 10 );
				if(tmpsize < 1 || tmpsize > 99){
					fprintf(stderr, "endline proportion (%d) must be a value in [1...99]", tmpsize);
					fprintf(stderr, "usage:\nbrace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v %%vac] [-e %%end]");
					exit( EXIT_FAILURE );
				}
				end = tmpsize;
				break;
			case 1:
				fprintf(stderr, "usage:\n brace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v %%vac] [-e %%end]");
				break;
			default:
				//some unknown, possibly unacceptable option flag
				fprintf(stderr, "usage:\n");
				fprintf(stderr, "brace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v %%vac] [-e %%end]\n");
				
				exit( EXIT_FAILURE );
        }
    }
	char neighborhood[size][size];
	int vacant = size*size*vacancies;
	int e = size*size*end;
	int n = size*size - e - vacant;
	total = n + e;
	for(int row = 0; row<=size; row++){
		for(int col = 0; col<=size; col++){
			neighborhood[row][col] = '.';
		}
	}
	for(int i = 0; i < e; i++){
		int row = rand();
		row = row % size;
		int col = rand();
		col = col % size;
		if(neighborhood[row][col] == '.'){
			neighborhood[row][col] = 'e';
		}
		else{
			i--;
		}
	}
	for(int x = 0; x < n; x++){
		int row = rand();
		row = row % size;
		int col = rand();
		col = col % size;
		if(neighborhood[row][col] == '.'){
			neighborhood[row][col] = 'n';
		}
		else{
			x--;
		}
	}
	
	clear();
	printNeighborhood(size, neighborhood);
	cycle++;
	//Infinite mode
	if(count == -1){
		while ( 1 ) {
			getHappy(size, neighborhood);
			cycle++;
			move = 1;
		}	
	}
	// Non-Infinite Mode
	else{
		int y = 0;
		while(y < count){
			getHappy(size, neighborhood);
			cycle++;
			move = 1;
			y++;
		}
	}
	return 0;	
}
