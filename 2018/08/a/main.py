#!/usr/bin/env python3

import sys

def treesum (tree):

	s = 0
	n = tree.pop(0)
	z = tree.pop(0)

	for _ in range(n):
		s += treesum(tree)

	for _ in range(z):
		s += tree.pop(0)

	return s


def parse (s):

	return [ int(x) for x in s.split(' ') ]


def main ():

	tree = parse(sys.stdin.read())

	print(treesum(tree))

if __name__ == '__main__':
	main()
