
#include <stdio.h>

int a = 1;
int b = 2;
int c = 3;

void main(){
	int z = a = b = c = 0;
	printf( "%d\n", z);
}
