//Author: Nicholas Graca
//Date: 12/3/17

#include "interpret.h"
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int opt;
	//if there is an even number of command line arguments or if there are more than
	//5 command line arguments
	if((argc % 2) == 0 || argc > 5){
		fprintf(stderr, "Usage:  fred [ -s symbol-table-file ] [ -f fred-program-file ]\n");
		return EXIT_FAILURE;
	}
	// makes sure that you done do -s twice
	bool s = true;
	// runs the command line if no fred file was used
	bool command = true;
	Table* t = create();
		char* file;
		while ( (opt = getopt( argc, argv, "s:f:") ) != -1 ) {
			//if they use anything but -s or -f throw an error
			if(opt != 's'){
				if(opt != 'f'){
					fprintf(stderr, "Usage:  fred [ -s symbol-table-file ] [ -f fred-program-file ]\n");
					return EXIT_FAILURE;
				}
			}
			switch ( opt ) {
				case 's':
					if(s){
						//try to open the file and call the read Symbol Table function
						s = false;
						file = optarg;
						FILE* symbolFile = fopen(file, "r");
						if(symbolFile == NULL){
							fprintf(stderr, "You did not enter a symbol file when you said you were");
							fprintf(stderr, "going to. Shame on you.\n");
							return EXIT_FAILURE;
						}
						t = readSymbolTable(symbolFile, t);
						break;
					}
					else{
						fprintf(stderr, "Usage:  fred [ -s symbol-table-file ] [ -f fred-program-file ]\n");
						return EXIT_FAILURE;
					}
				case 'f':
					//try to open the fred file and call the function to parse through it
					command = false;
					file = optarg;
					FILE* fredFile = fopen(file, "r");
					if(fredFile == NULL){
						fprintf(stderr, "You did not enter a fred file when you said you were");
						fprintf(stderr, "going to. Shame on you.\n");
						return EXIT_FAILURE;
					}
					readFredFile(fredFile, t);
					break;
				default:
					readFredCommand(t);
			}
		}
		//if there is no fred file, open the command line
		if(command){
			readFredCommand(t);
		}
		//dump the table and free everything
		dumpSymbolTable(t);
		destroy(t);
}
