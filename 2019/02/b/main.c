#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CONT 0
#define HALT 1

typedef struct {
	int mem[BUFSIZ];
	size_t len;

	int pos;
} Intcode;

int shift (Intcode *ic) {
	return ic->mem[ic->mem[ic->pos++]];
}

int move (Intcode *ic, int loc, int val) {
	return ic->mem[ic->mem[loc]] = val;
}

int halt (Intcode *ic) {
	return HALT;
}

int add (Intcode *ic) {
	int v1 = shift(ic), v2 = shift(ic);
	move(ic, ic->pos++, v1 + v2);
	return CONT;
}

int mul (Intcode *ic) {
	int v1 = shift(ic), v2 = shift(ic);
	move(ic, ic->pos++, v1 * v2);
	return CONT;
}

typedef int (*Instruction)(Intcode *);

Instruction instructions[] = {
	/* 0     1    2      3      4     5     6     7     8      9 */
	NULL,  add, mul,  NULL,  NULL, NULL, NULL, NULL, NULL,  NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL,  NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL,  NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL,  NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL,  NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL,  NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL,  NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL,  NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL,  NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL,  halt,
};

int next (Intcode *ic) {
	return ic->mem[ic->pos++];
}

int run (Intcode *ic) {
	while (instructions[next(ic)](ic) != HALT);
	return ic->mem[0];
}

void parse (Intcode *ic) {
	int val;
	while (scanf("%d,", &val) != EOF) {
		ic->mem[ic->len++] = val;
	}
}

int main (int argc, char *argv[]) {
	Intcode ic;
	memset(&ic, 0, sizeof ic);

	parse(&ic);

	int x, y;
	for (x = 0; x < 100; x++) {
		for (y = 0; y < 100; y++) {

			Intcode tmp;
			memcpy(&tmp, &ic, sizeof tmp);

			tmp.mem[1] = x;
			tmp.mem[2] = y;

			int res = run (&tmp);

			if (res == 19690720) {
				printf("%02d%02d\n", x, y);
				break;
			}

		}
	}

	exit(EXIT_SUCCESS);
}

