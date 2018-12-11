#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node {
	int value;
	struct node *next, *prev;
};

typedef struct node Node;

struct ring {
	Node *current;
};

typedef struct ring Ring;

void *xmalloc (size_t size) {
	void *p = malloc(size);

	if (!p) {
		abort();
	}

	memset(p, 0, size);
	return p;
}

void *xcalloc (size_t nmemb, size_t size) {
	void *p = calloc(nmemb, size);

	if (!p) {
		abort();
	}
	return p;
}

void init (Ring *ring) {

	Node *node = xmalloc(sizeof *node);

	node->prev = node;
	node->next = node;
	node->value = 0;

	ring->current = node;
}

void seek (Ring *ring, int n) {
	if (n < 0) {
		while (n) {
			ring->current = ring->current->prev;
			n++;
		}
	} else {
		while (n) {
			ring->current = ring->current->next;
			n--;
		}
	}
}

int pop (Ring *ring) {
	Node *node = ring->current;
	int value = node->value;

	node->next->prev = node->prev;
	node->prev->next = node->next;

	ring->current = node->next;

	free(node);

	return value;
}

void insert (Ring *ring, int v) {
	Node *node = xmalloc(sizeof *node);

	node->value = v;

	node->prev = ring->current;
	node->next = ring->current->next;

	ring->current->next->prev = node;
	ring->current->next = node;

	ring->current = node;
}

void destroy (Ring *ring) {
	Node *curr = ring->current->next, *next;

	while (curr != ring->current) {
		next = curr->next;
		free(curr);
		curr = next;
	}

	free(ring->current);
}

int play (int nplayers, int highest) {

	int *scores = xcalloc(nplayers, sizeof *scores);
	int score = 0;

	int i = 0;
	int player = 0;

	Ring ring;

	init(&ring);

	memset(scores, 0, sizeof scores);

	for (i = 1; i <= highest; i++) {

		if (i % 23 == 0) {
			seek(&ring, -7);
			scores[player] += i + pop(&ring);

		} else {
			seek(&ring, 1);
			insert(&ring, i);
		}

		player = (player + 1) % nplayers;
	}

	for (i = 0; i < nplayers; i++) {
		if (score < scores[i]) {
			score = scores[i];
		}
	}

	destroy(&ring);

	return score;
}

void parse (int *nplayers, int *highest) {
	scanf("%d players; last marble is worth %d points", nplayers, highest);
}

#include <assert.h>
int units[][3] = {
	{9,    25,     32},
	{10, 1618,   8317},
	{13, 7999, 146373},
	{17, 1104,   2764},
	{21, 6111,  54718},
	{30, 5807,  37305}
};

void test () {
	int i, result;
	for (i = 0; i < sizeof units; i++) {
		result = play(units[i][0], units[i][1]);
		if (result != units[i][2]) {
			printf("test #%d failed: play(%d, %d); expected %d, got %d\n",
				i, units[i][0], units[i][1], units[i][2],
				result
			);
			abort();
		}
	}
}

int main (int argc, char *argv[]) {

	int nplayers, highest;

	test();

	parse(&nplayers, &highest);

	printf("%d\n", play(nplayers, highest));

	exit(EXIT_SUCCESS);
}
