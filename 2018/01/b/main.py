#!/usr/bin/env python3

import sys

def main ():

	deltas = getdeltas();

	freq = 0
	seen = {}

	while True:

		for delta in deltas:

			freq += delta
			if freq in seen:
				print(freq)
				quit()

			seen[freq] = 1


def getdeltas ():

	deltas = [];

	for line in sys.stdin:
		deltas.append(int(line))

	return deltas

if __name__ == "__main__":
	main()
