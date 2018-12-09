#!/usr/bin/env python3

import sys


def treeval (tree):

	v = 0
	n = tree.pop(0)
	z = tree.pop(0)

	if n:
		s = []
		for _ in range(n):
			s.append(treeval(tree))

		for _ in range(z):
			x = tree.pop(0) - 1

			if 0 <= x < len(s):
				v += s[x]

	else:
		for _ in range(z):
			v += tree.pop(0)

	return v


def parse (s):

	return [ int(x) for x in s.split(' ') ]


def main ():

	tree = parse(sys.stdin.read())

	print(treeval(tree))

if __name__ == '__main__':
	main()
