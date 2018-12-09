#!/usr/bin/env python3

import sys
import re

# Kahn's algorithm
def topsort (graph):

	L = []
	S = [ k for k, v in graph.items() if len(v) == 0 ]
	g = { k: v for k, v in graph.items() if len(v) > 0 }

	while S:
		n = S.pop(0)
		L.append(n)

		for m in g.keys():
			g[m] = [ e for e in g[m] if e != n ]

			if len(g[m]) == 0:
				S.append(m)

		g = { k: v for k, v in g.items() if len(v) > 0 }

		S.sort()

	return L


def tokens (lines):

	tokensre = re.compile("Step ([A-Z]) must be finished before step ([A-Z]) can begin.")

	for line in lines:
		m = tokensre.match(line).group(1, 2)
		yield m


def parse (lines):

	graph = {}

	for d, s in tokens(lines):

		if s not in graph:
			graph[s] = []
		if d not in graph:
			graph[d] = []

		graph[s].append(d)

	return graph


def main ():

	graph = parse(sys.stdin.readlines())
	print(''.join(topsort(graph)))


if __name__ == '__main__':
	main()
