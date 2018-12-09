#!/usr/bin/env python3

import sys
import re

MAX_WORKERS = 5

def task2time (task):
	return 60 + ord(task) - (ord('A') - 1)


def process (graph):

	result = 0

	ready = [ (k, task2time(k)) for k, v in graph.items() if len(v) == 0 ]
	procs = { **(dict(ready[:MAX_WORKERS])) }
	ready = ready[MAX_WORKERS:]

	g = { k: v for k, v in graph.items() if len(v) > 0 }

	while procs:

		(task, n) = min(procs.items(), key = lambda p: (p[1], p[0]))

		if n == 0:
			del procs[task]

			for k in g.keys():
				g[k] = [ v for v in g[k] if v != task ]

				if not g[k]:
					ready.append((k, task2time(k)))

			g = { k: v for k, v, in g.items() if len(v) > 0 }

			avail = MAX_WORKERS - len(procs)
			if avail:
				procs = { **procs, **(dict(ready[:avail])) }
				ready = ready[avail:]

		else:
			result += n
			procs = { k: v - n for k, v in procs.items() }

	return result


def tokens (lines):

	tokensre = re.compile("Step ([A-Z]) must be finished before step ([A-Z]) can begin.")

	for line in lines:
		yield tokensre.match(line).group(1, 2)


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
	print(process(graph))


if __name__ == '__main__':
	main()
