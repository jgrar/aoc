#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#define CONT 0
#define HALT 1
#define PAUS 2

typedef struct {
	int mem[BUFSIZ];
	size_t len;

	int pos;
	int modes;

	int in, out;

	int status;
} Intcode;

enum Modes {
	POSITION,
	IMMEDIATE,
	RELATIVE
};

int mode (Intcode *ic) {
	int mode = ic->modes % 10;
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
	return ic->status = HALT;
}

int add (Intcode *ic) {
	int v1 = shift(ic), v2 = shift(ic);
	move(ic, ic->pos++, v1 + v2);
	return ic->status = CONT;
}

int mul (Intcode *ic) {
	int v1 = shift(ic), v2 = shift(ic);
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
	int val = shift(ic), loc = shift(ic);

	if (val != 0) {
		ic->pos = loc;
	}
	return ic->status = CONT;
}

int jz (Intcode *ic) {
	int val = shift(ic), loc = shift(ic);

	if (val == 0) {
		ic->pos = loc;
	}
	return ic->status = CONT;
}

int lt (Intcode *ic) {
	int v1 = shift(ic), v2 = shift(ic);
	int res = 0;

	if (v1 < v2) {
		res = 1;
	}

	move(ic, ic->pos++, res);
	return ic->status = CONT;
}

int eq (Intcode *ic) {
	int v1 = shift(ic), v2 = shift(ic);
	int res = 0;

	if (v1 == v2) {
		res = 1;
	}

	move(ic, ic->pos++, res);
	return ic->status = CONT;
}

typedef int (*Instruction)(Intcode *);

Instruction instructions[] = {
	/* 0     1    2      3      4     5     6     7     8     9 */
	NULL,  add, mul,  scan, print,  jnz,   jz,   lt,   eq, NULL,
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
	int instr = ic->mem[ic->pos++];

	ic->modes = instr / 100;
	instr %= 100;

	return instr;
}

int eval (Intcode *ic, int instr) {
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

	int val;
	while (fscanf(fp, "%d,", &val) != EOF) {
		ic->mem[ic->len++] = val;
	}

	fclose(fp);
}

int permute (int *a, size_t size) {

	int k;
	for (k = size - 2; k >= 0; k--) {
		if (a[k] < a[k + 1]) {

			int l;
			for (l = size - 1; a[l] < a[k]; l--) ;

			int temp;

			temp = a[k];
			a[k] = a[l];
			a[l] = temp;

			int p, q;
			for (p = k + 1, q = size - 1; p < q; p++, q--) {
				temp = a[p];
				a[p] = a[q];
				a[q] = temp;
			}

			break;
		}
	}

	return k;
}

#define LEN(a) (sizeof (a) / sizeof (a)[0])
#define MAX(a,b) (((a) > (b)) ? (a): (b))

int main (int argc, char *argv[]) {

	Intcode ic;

	int phases[][5] = {{0, 1, 2, 3, 4}, {5, 6, 7, 8, 9}};
	int j;

	memset(&ic, 0, sizeof ic);
	parse(argv[1], &ic);

	for (j = 0; j < LEN(phases); j++) {
		int mval = 0;
		do {
			int i;
			Intcode amps[LEN(phases[j])];

			memset(amps, 0, sizeof amps);

			for (i = 0; i < LEN(amps); i++) {
				memcpy(&amps[i], &ic, sizeof amps[i]);

				amps[i].in = phases[j][i];
				eval(&amps[i], next(&amps[i]));
			}

			int val = 0;
			while (amps[0].status != HALT) {
				for (i = 0; i < LEN(amps); i++) {
					if (amps[i].status != HALT) {
						amps[i].in = val;
						run(&amps[i]);
						val = amps[i].out;
					}
				}
			}
			mval = MAX(mval, val);

		} while (permute(phases[j], LEN(phases[j])) >= 0);

		printf("%d\n", mval);
	}

	exit(EXIT_SUCCESS);
}
