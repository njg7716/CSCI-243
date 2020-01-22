///Nicholas Graca
///njg7716@rit.edu
///Reads data from a file and finds the max population, min population, the zipcodes that corresponds to those, the total population, and the amount of zipcodes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// define all the variables needed for this program

long maxpop = 0;
long maxpopzip = 0;
long totpop = 0;
long totpopzip = 0;
long minpop = 1000000;
long minpopzip = 0;
long totzip = 0;
char file[50];
char line[256];
int linenum = 0;
char zip[12];
long ipop = 0;
long izip = 0;
int it = 0;
int z = 6;
char comma = ',';
char pop[12];

int main(void){

	///Loops through file and gets each line until there are no more lines to read
	while(fgets(line, 256, stdin)){

		///Skips the first line of data which is just labels
		if(linenum != 0){

			///Loops and gets the zipcode character by character
			for(int i = 0; i < 5; i++){
				zip[i] = line[i];
			}

			///Loops and gets the population character by character
			while(line[z] != comma){
				pop[it] = line[z];
				it++;
				z++;
			}

			///Converts the population and zipcode into Longs 
			ipop = strtol(pop, NULL, 10);
			izip = strtol(zip, NULL, 10);
			totpop += ipop;
			totzip++;

			///Finds the max population
			if(ipop > maxpop){
				maxpop = ipop;
				maxpopzip = izip;
			}

			///Finds the minimum population
			if(ipop < minpop){
				minpop = ipop;
				minpopzip = izip;
			}
		}
	linenum++;

	///Resets data
	it = 0;
	z = 6;
	memset(pop, '\0', 12);
	}

	printf("Total population across %ld zipcodes is %ld.\n", totzip, totpop);
	printf("The fewest people live in %ld, population %ld.\n", minpopzip, minpop);
	printf("The most people live in %ld, population %ld.\n", maxpopzip, maxpop);
	return EXIT_SUCCESS;
}
