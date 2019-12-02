#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TBLSIZ 256 // hash table size
#define GROWSIZ 256 // array growth size

/** rudimentary variable length array:
 */
struct array {
	int *base;
	int len, cap;
};

typedef struct array Array;

#define INIT(array) memset(&(array), 0, sizeof (array))

void grow (Array *array) {

	int size = array->cap + GROWSIZ;
	int *base = realloc(array->base, sizeof *array->base * size);

	if (!base) {
		abort();
	}

	array->base = base;
	array->cap = size;
}

int append (Array *array, int item) {

	if (array->len >= array->cap) {
		grow(array);
	}

	array->base[array->len++] = item;
	return array->len - 1;
}

int exists (Array array, int term) {

	int i;

	for (i = 0; i < array.len; i++) {
		if (array.base[i] == term) {
			return 1;
		}
	}

	return 0;
}

/** rudimentary hash table:
 */
struct table {
	Array *table;
	int size;
};

typedef struct table Table;

Table create (int size) {

	Table table;

	table.table = calloc(size, sizeof *table.table);
	table.size = size;
	return table;
}

unsigned hash (int value, int size) {
	return value & (size - 1);
}

void *insert(Table table, int value) {

	unsigned i = hash(value, table.size);
	Array array = table.table[i];

	if (exists(array, value)) {
		return NULL;
	}

	append(&table.table[i], value);
	return &table.table[i];
}

void destroy (Table table) {
	int i;
	for (i = 0; i < table.size; i++) {
		if (table.table[i].base) {
			free(table.table[i].base);
		}
	}
	free(table.table);
}

/** program code:
 */
Array read () {

	Array array;
	int value;

	INIT(array);

	while (scanf("%d", &value) != EOF) {
		append(&array, value);
	}

	return array;
}

int main (int argc, char *argv[]) {

	Array deltas;
	Table freqs;

	deltas = read();
	freqs = create(TBLSIZ);

	int freq = 0;
	int delta = 0;
	int i = 0;

	do {

		delta = deltas.base[i++];
		if (i >= deltas.len) {
			i = 0;
		}

		freq += delta;

	} while (insert(freqs, freq) != NULL);

	printf("%d\n", freq);

	free(deltas.base);
	destroy(freqs);

	exit(EXIT_SUCCESS);
}

