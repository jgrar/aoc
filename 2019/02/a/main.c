#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum Positions {
	NOUN = 1,
	VERB,
};

enum Opcode {
	ADD = 1,
	MUL,
	HALT = 99,
};

struct intcode {
	int *code;
	int len;
	int ip;
};

typedef struct intcode Intcode;

#define BOUNDS_CHECK(x,m) if ((x) > (m)) abort()

int shift (Intcode *intcode) {
	BOUNDS_CHECK(intcode->ip, intcode->len);
	return intcode->code[intcode->ip++];
}

int load (Intcode *intcode, int addr) {
	BOUNDS_CHECK(addr, intcode->len);
	return intcode->code[addr];
}

int write (Intcode *intcode, int addr, int value) {
	BOUNDS_CHECK(addr, intcode->len);
	return intcode->code[addr] = value;
}

int halt (Intcode *intcode) {
	return 1;
}

int add (Intcode *intcode) {

	int v1 = load(intcode, shift(intcode));
	int v2 = load(intcode, shift(intcode));

	write(intcode, shift(intcode), v1 + v2);
	return 0;
}

int mul (Intcode *intcode) {

	int v1 = load(intcode, shift(intcode));
	int v2 = load(intcode, shift(intcode));

	write(intcode, shift(intcode), v1 * v2);
	return 0;
}

typedef int (*Operation)(Intcode *intcode);

Operation optab[100] = { NULL };

void init () {

	optab[ADD] = &add;
	optab[MUL] = &mul;

	optab[HALT] = &halt;
}

int run (int *_code, int len, int noun, int verb) {

	int code[len + 1];
	Intcode intcode;

	memcpy(code, _code, len * sizeof code[0]);
	code[NOUN] = noun;
	code[VERB] = verb;

	intcode = (Intcode) {code, len, 0};

	while (1) {

		int opcode = shift(&intcode);
		int status = optab[opcode](&intcode);

		if (status != 0) {
			break;
		}
	}

	return intcode.code[0];
}

#define REALLOCN 256

int read (int **code, int *size) {

	int len = 0;
	int val;

	while (scanf("%d,", &val) != EOF) {

		if (len >= *size) {

			int n = *size + REALLOCN;
			int *p = realloc(*code, n * sizeof **code);

			if (!p) {
				abort();
			}

			*code = p;
			*size = n;
		}

		(*code)[len++] = val;
	}

	return len;
}

int main (int argc, char *argv[]) {

	int *code = NULL, size = 0;
	int len = 0;

	init();
	len = read(&code, &size);

	int retval = run(code, len, 12, 2);

	free(code);
	printf("%d\n", retval);

	exit(EXIT_SUCCESS);
}

