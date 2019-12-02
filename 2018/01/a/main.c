#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]) {

	int delta;
	int frequency = 0;

	while (scanf("%d", &delta) != EOF) {
		frequency += delta;
	}

	printf("%d\n", frequency);
}
