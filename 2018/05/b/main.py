#!/usr/bin/env python3

import sys
import string

def reacts (a, b):
	return a != b and a.casefold() == b.casefold()

def process (units, ignore):

	result = []
	for u in units:
		if u.casefold() == ignore.casefold():
			continue

		if result and reacts(result[-1], u):
			result.pop()
		else:
			result.append(u)

	return result

def main ():

	units = sys.stdin.read().rstrip()
	mlen = len(units)

	for z in string.ascii_lowercase:
		n = len(process(units, z))

		if n < mlen:
			mlen = n

	print(mlen)

if __name__ == '__main__':
	main()
