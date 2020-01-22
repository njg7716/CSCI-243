/// http://www.dreamincode.net/forums/topic/55572-conways-game-of-life/
/// downloaded by copy-paste on 8/19/2015 and modified to use an infinite loop
/// and have different Rule functions.
///
/// This code needs serious work in these areas:
/// <ol>
/// <li>  syntax: there are a number of warnings that cause errors.
/// </li>
/// <li>  semantics: the game algorithm implementation is incorrect.
/// </li>
/// <li>  design: the implementation needs function and value refactoring.
/// </li>
/// <li>  style: formatting is poor; the mix of TAB and spaces indentation
/// needs correction, and spacing between tokens is inconsistent.
/// The course style puts the '{' at the end of the function header, not
/// on a line of its own, and function headers belong at the left margin.
/// </li>
/// <li>  documentation: documentation is non-existent except for this block.
/// </li>
/// </ol>
///

#include <stdio.h>
#include <stdlib.h>
int size = 20;
int arrayLen = 19;
void header(void) /*function for program header*/{
	printf("\n\t..Welcome to the Game of life..\n");
}

int checkMinus(int num){
	num--;
	if(num < 0){
		num = arrayLen;
		return num;
	}
	return num;
}

int checkPlus(int num){
	num++;
	if(num > arrayLen){
		num = 0;
		return num;
	}
	return num;
}

int neighborsCount(int size, char life[][size], int row, int col){
///Both survivalRule and birthRule had these if statements so I just made it a function they can both call 
///That way there is less duplicate code
	int neighbors = 0;
	if((life[checkMinus(row)][checkMinus(col)]) == '*')
		++neighbors;

	if((life[checkMinus(row)][col]) == '*')
		++neighbors;

	if((life[checkMinus(row)][checkPlus(col)]) == '*')
		++neighbors;

	if((life[row][checkMinus(col)]) == '*')
		++neighbors;

	if((life[row][checkPlus(col)]) == '*')
		++neighbors;

	if((life[checkPlus(row)][checkMinus(col)]) == '*')
		++neighbors;

	if((life[checkPlus(row)][col]) == '*')
		++neighbors;

	if((life[checkPlus(row)][checkPlus(col)]) == '*')
		++neighbors;

	return neighbors;
}

void survivalRule(int size, char life[][size]){
	//I put Parenthesis around the boolean statements and made sure the equals signs were double where they needed to be.
	int row, col;
	for(row = 0; row<=arrayLen; row++){
		for(col = 0; col<=arrayLen; col++){
			if((life[row][col])== '*'){
				int neighbors = neighborsCount(size, life, row, col);

				if(neighbors == 2 || neighbors == 3)
					life[row][col] = '*';

				else{
					life[row][col] = ' ';
				}
			}	
		}
	}
	return;
}
	
void birthRule(int size, char life[][size]){
	//I put parenthesis around in the if statements and made sure there were double equals signs
	int row, col;
	for(row = 0; row<=arrayLen; row++){
		for(col = 0; col<=arrayLen; col++){
			if((life[row][col])== ' '){
				int neighbors = neighborsCount(size, life, row, col);

				if(neighbors == 3)
					life[row][col] = '*';

				else{
					life[row][col] = ' ';
				}
			}
		}
	}

	return;
}
	
   	
int main(void){
	// I got rid of all the unused variables
	char life[size][size];
	int orgs;
	int i, row, col;
	int count = 0;
   	header();
	printf("\nPlease enter the initial number of organisms: ");
	scanf("%i", &orgs);
	srand( 31 );

	for(i = 0; i<orgs; i++){
		row = rand();
		row %= size;
		col = rand();
		col %= size;
		life[row][col] = '*';//fix2
	}
		 
	for(row = 0; row<size; row++){

		for(col = 0; col<size; col++){
			if(life[row][col] != '*')
			life[row][col] = ' ';//fix2
		}
	}
			  
	for(row = 0; row<size; row++){

		for(col = 0; col<size; col++){
			printf("%c", life[row][col]); //fix1 and fix3
		}
		puts(" ");
	}
			 
	while ( count < 100 ) {//I used the count variable to make sure it would stop after looping 100 times
		birthRule(size, life);
		survivalRule(size, life);
		for(row = 0; row<size; row++){
				  
			for(col = 0; col<size; col++){
				printf("%c", life[row][col]);//fix1 and fix3
			}
			puts(" ");
		}
		printf("\ngeneration: %d\n", count);
		count++;
	}
		  
   	
	return 0;
}			
