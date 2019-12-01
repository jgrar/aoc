#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main (int argc, char *argv[]) {

	int weight;
	int total = 0;

	while (scanf("%d", &weight) != EOF) {
		total += floor(weight / 3) - 2;
	}

	printf("%d\n", total);
	exit(EXIT_SUCCESS);
}
