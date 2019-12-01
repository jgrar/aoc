#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int fuel (int weight) {

	int fw = floor(weight / 3) - 2;

	if (fw <= 0) {
		return 0;
	}

	return fw + fuel(fw);
}

int main (int argc, char *argv[]) {

	int weight;
	int total = 0;

	while (scanf("%d", &weight) != EOF) {
		total += fuel(weight);
	}

	printf("%d\n", total);
	exit(EXIT_SUCCESS);
}

