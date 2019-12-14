#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#define CONT 0
#define HALT 1
#define PAUS 2

typedef struct {
	long mem[BUFSIZ];
	size_t len;

	long pos;
	long rbi;
	long modes;

	int in, out;
	int status;
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

long shift (Intcode *ic) {
	switch (mode(ic)) {
		case POSITION:
			return ic->mem[ic->mem[ic->pos++]];
		case IMMEDIATE:
			return ic->mem[ic->pos++];
		case RELATIVE:
			return ic->mem[ic->rbi + ic->mem[ic->pos++]];
	}
}

long move (Intcode *ic, long loc, long val) {
	switch (mode(ic)) {
		case POSITION:
			return ic->mem[ic->mem[loc]] = val;
		case RELATIVE:
			return ic->mem[ic->rbi + ic->mem[loc]] = val;
	}
}

int halt (Intcode *ic) {
	return ic->status = HALT;
}

int add (Intcode *ic) {
	long v1 = shift(ic), v2 = shift(ic);
	move(ic, ic->pos++, v1 + v2);
	return ic->status = CONT;
}

int mul (Intcode *ic) {
	long v1 = shift(ic), v2 = shift(ic);
	move(ic, ic->pos++, v1 * v2);
	return ic->status = CONT;
}

int scan (Intcode *ic) {
	move(ic, ic->pos++, ic->in);
	return ic->status = CONT;
}

int print (Intcode *ic) {
	ic->out = shift(ic);
	return ic->status = PAUS;
}

int jnz (Intcode *ic) {
	long val = shift(ic), loc = shift(ic);

	if (val != 0) {
		ic->pos = loc;
	}
	return ic->status = CONT;
}

int jz (Intcode *ic) {
	long val = shift(ic), loc = shift(ic);

	if (val == 0) {
		ic->pos = loc;
	}
	return ic->status = CONT;
}

int lt (Intcode *ic) {
	long v1 = shift(ic), v2 = shift(ic);
	long res = 0;

	if (v1 < v2) {
		res = 1;
	}

	move(ic, ic->pos++, res);
	return ic->status = CONT;
}

int eq (Intcode *ic) {
	long v1 = shift(ic), v2 = shift(ic);
	int res = 0;

	if (v1 == v2) {
		res = 1;
	}

	move(ic, ic->pos++, res);
	return ic->status = CONT;
}

int adjrb (Intcode *ic) {
	ic->rbi += shift(ic);
	return ic->status = CONT;
}

typedef int (*Instruction)(Intcode *);

Instruction instructions[] = {
	/* 0     1    2      3      4     5     6     7     8      9 */
	NULL,  add, mul,  scan, print,  jnz,   jz,   lt,   eq, adjrb,
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

long next (Intcode *ic) {
	long instr = ic->mem[ic->pos++];

	ic->modes = instr / 100;
	instr %= 100;

	return instr;
}

int eval (Intcode *ic, long instr) {
	return instructions[instr](ic);
}

int run (Intcode *ic) {
	while (eval(ic, next(ic)) == CONT);
	return ic->status;
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

#define LEN(a) (sizeof (a) / sizeof (a)[0])

int main (int argc, char *argv[]) {
	int inputs[] = {1, 2};

	int i;

	Intcode ic;
	memset(&ic, 0, sizeof ic);
	parse(argv[1], &ic);

	for (i = 0; i < LEN(inputs); i++) {
		Intcode tc;
		memcpy(&tc, &ic, sizeof tc);

		tc.in = inputs[i];
		while (tc.status != HALT) {
			run(&tc);
		}
		printf("%ld\n", tc.out);
	}

	exit(EXIT_SUCCESS);
}
