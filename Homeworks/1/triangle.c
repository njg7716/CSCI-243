/// 
/// File: triangle.c 
/// 
/// A program to print out multiple "pyramid" triangles of the form:
/// <pre>
///   *
///  ***
/// *****
/// </pre>
/// 
/// @author TODO_AUTHOR_ID_ : TODO_AUTHOR_FULL_NAME_
/// 
// // // // // // // // // // // // // // // // // // // // // // // // 

// TODO_ADD_#INCLUDES_ HERE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 *  Function: drawTriangle
 *
 *  Description: Draw a 'pyramid' triangle whose base width is 'size'.
 *  Note: If size is even, the function makes a 'size + 1' pyramid triangle. 
 * 
 *  @param size  the width of the base of the triangle to draw
 */
void drawTriangle(int size){
	printf("\n");
	if(size%2==0){ //Handles if the size is a even number
		size++;
}
	int center = (size+1)/2; //Finds the center of the triangle where the first * in row 1 will go
	int row = (size+1)/2; // computes the number of rows based on the size given
	for(int i=1;i<=row;i++){ //This will loop through the whole triangle row by row
		int temp = center;
		int count = 1;
		while(temp>0){ // This loop prints the spaces before the *
			printf(" ");
			temp--;
}
		while(count<=2*i-1){ //This loop will print the correct number of *
			printf("*");
			count++;
}

	center--;
	printf("\n");
}
}

/**
 *  Function: main 
 *
 *  Description: draws pyramid triangles of size 1, 5, and '6'.
 *
 *  @returns errorCode  error Code; EXIT_SUCCESS if no error
 */
int main(){
	drawTriangle(1);
	drawTriangle(5);
	drawTriangle(6);
	printf("\n");	
	return EXIT_SUCCESS;
}

