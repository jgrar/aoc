#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#define CONT 0
#define HALT 1

typedef struct {
	int mem[BUFSIZ];
	size_t len;

	int pos;
	int modes;
} Intcode;

enum Modes {
	POSITION,
	IMMEDIATE,
	RELATIVE
};

long mode (Intcode *ic) {
	long mode = ic->modes % 10;
	ic->modes /= 10;
	return mode;
}

int shift (Intcode *ic) {
	switch (mode(ic)) {
		case POSITION:
			return ic->mem[ic->mem[ic->pos++]];
		case IMMEDIATE:
			return ic->mem[ic->pos++];
	}
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

int read (Intcode *ic) {
	long val;
	scanf("%ld", &val);

	move(ic, ic->pos++, val);
	return CONT;
}

int write (Intcode *ic) {
	printf("%ld\n", shift(ic));
	return CONT;
}

int jnz (Intcode *ic) {
	long val = shift(ic), loc = shift(ic);

	if (val != 0) {
		ic->pos = loc;
	}
	return CONT;
}

int jz (Intcode *ic) {
	long val = shift(ic), loc = shift(ic);

	if (val == 0) {
		ic->pos = loc;
	}
	return CONT;
}

int lt (Intcode *ic) {
	long v1 = shift(ic), v2 = shift(ic);
	long res = 0;

	if (v1 < v2) {
		res = 1;
	}

	move(ic, ic->pos++, res);
	return CONT;
}

int eq (Intcode *ic) {
	long v1 = shift(ic), v2 = shift(ic);
	int res = 0;

	if (v1 == v2) {
		res = 1;
	}

	move(ic, ic->pos++, res);
	return CONT;
}

typedef int (*Instruction)(Intcode *);

Instruction instructions[] = {
	/* 0     1    2      3      4     5     6     7     8     9 */
	NULL,  add, mul,  read, write,  jnz,   jz,   lt,   eq, NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, halt,
};

int next (Intcode *ic) {
	long instr = ic->mem[ic->pos++];

	ic->modes = instr / 100;
	instr %= 100;

	return instr;
}

int eval (Intcode *ic, long instr) {
	return instructions[instr](ic);
}

void run (Intcode *ic) {
	while (eval(ic, next(ic)) != HALT);
}

void parse (const char *path, Intcode *ic) {
	FILE *fp = fopen(path, "r");

	if (!fp) {
		fprintf(stderr, "failed to open file '%s': %s\n", path, strerror(errno));
		exit(EXIT_FAILURE);
	}

	long val;
	while (fscanf(fp, "%ld,", &val) != EOF) {
		ic->mem[ic->len++] = val;
	}

	fclose(fp);
}

int main (int argc, char *argv[]) {
	char **p;

	Intcode ic;
	memset(&ic, 0, sizeof ic);

	for (p = argv + 1; *p; p++) {
		parse(*p, &ic);
		run(&ic);
	}

	exit(EXIT_SUCCESS);
}
