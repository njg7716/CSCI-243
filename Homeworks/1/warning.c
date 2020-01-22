/// file: warning.c
/// description: faulty code example that needs fixing.
/// @author RIT CS Dept.
/// @co-author Nick Graca
#include <stdio.h>

/// compute a value.
int compute(int a) {
    int b = a * 5;
	return b;
}

/// main function should print 2017
int main(void) {
    int x=0;
    int y = compute(20);

    printf("%d\n", y+x);

    return 0;
}

