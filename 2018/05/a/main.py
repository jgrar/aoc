#!/usr/bin/env python3

import sys

def reacts (a, b):
	return a != b and a.casefold() == b.casefold()

def process (units):

	result = []
	for u in units:
		if result and reacts(result[-1], u):
			result.pop()
		else:
			result.append(u)

	return result

def main ():

	units = sys.stdin.read().rstrip()
	print(len(process(units)))

if __name__ == '__main__':
	main()
