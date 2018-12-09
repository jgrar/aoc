#!/usr/bin/env python3

import sys
from PIL import Image

class Coord:

	def __init__ (self, x, y):
		self.x = x
		self.y = y

	def __sub__ (self, other):
		return abs(self.x - other.x) + abs(self.y - other.y)


def savevis (grid, sz):

	img = Image.new('RGB', (sz, sz), 'black')

	pixels = img.load()

	for x in range(img.size[0]):
		for y in range(img.size[1]):
			if (x, y) in grid:
				if grid[x, y]:
					pixels[x, y] = (255, 255, 255)

	img.save("vis.png")


def tokens (lines):
	for line in lines:
		yield [ int(t) for t in line.rstrip().split(', ') ]


def parse (lines):

	coords = []

	for x, y in tokens(lines):
		coords.append(Coord(x, y))

	return coords

GRID_SZ = 400
THRESHOLD = 10000

def main ():

	coords = parse(sys.stdin)

	grid = {}

	area = 0

	for x in range(GRID_SZ):
		for y in range(GRID_SZ):

			dist = 0
			grid[x, y] = 0

			for coord in coords:
				dist += coord - Coord(x, y)

			if dist < THRESHOLD:
				grid[x, y] = 1
				area += 1

	savevis(grid, GRID_SZ)

	print(area)

if __name__ == '__main__':
	main()
