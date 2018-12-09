#!/usr/bin/env python3

import sys
import random
from PIL import Image

class Coord:

	def __init__ (self, x, y):
		self.x = x
		self.y = y

	def __sub__ (self, other):
		return abs(self.x - other.x) + abs(self.y - other.y)


def randcolor ():
	return (
		random.randrange(256),
		random.randrange(256),
		random.randrange(256)
	)


def savevis (regions, grid, sz):

	img = Image.new('RGB', (sz, sz), 'black')

	pixels = img.load()

	for x in range(img.size[0]):
		for y in range(img.size[1]):
			pixels[x, y] = regions[grid[x, y]]['color']

	img.save('vis.png')


def tokens (lines):
	for line in lines:
		yield [ int(t) for t in line.rstrip().split(', ') ]


def parse (lines):

	regions = [{
		'center': Coord(0,0),
		'color': (0, 0, 0),
		'area': 0,
		'inf': True
	}]

	for x, y in tokens(lines):
		regions.append({
			'center': Coord(x, y),
			'color': randcolor(),
			'area': 0,
			'inf': False
		})

	return regions


def atedge (x, y, sz):

	return x == 0 or y == 0 or x == sz - 1 or y == sz - 1

def creategrid (regions, sz):

	grid = {}

	for x in range(sz):
		for y in range(sz):

			grid[x, y] = 0
			mdist = sz * 2
			regid = 0

			for i in range(1, len(regions)):

				region = regions[i]
				dist = region['center'] - Coord(x, y)

				if dist < mdist:
					mdist = dist
					regid = i

				elif dist == mdist:
					regid = 0

			if regid:
				grid[x, y] = regid
				region = regions[regid]
				region['area'] += 1

				if region['inf'] == False and atedge(x, y, sz):
					region['inf'] = True

	return grid

def bestregion (regions):

	marea = 0
	regid = 0

	for i in range(1, len(regions)):
		region = regions[i]

		if region['inf'] == False:

			if region['area'] > marea:
				marea = region['area']
				regid = i

	return regions[regid]

GRID_SZ = 400

def main ():

	regions = parse(sys.stdin.readlines())

	grid = creategrid(regions, GRID_SZ)
	savevis(regions, grid, GRID_SZ)

	print(bestregion(regions)['area'])


if __name__ == '__main__':
	main()
