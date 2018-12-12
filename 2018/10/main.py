#!/usr/bin/env python3

import sys
import re

from PIL import Image

ZOOM   =  4
BORDER = 10

def plot (points):

	points = [ (int((x / 2) * ZOOM), int((y / 2) * ZOOM)) for x, y, _, _ in points ]

	minx = min(points, key = lambda p: p[0])[0]
	miny = min(points, key = lambda p: p[1])[1]

	points = [ (x - minx + BORDER, y - miny + BORDER) for x, y in points ]

	maxx = max(points, key = lambda p: p[0])[0]
	maxy = max(points, key = lambda p: p[1])[1]

	img = Image.new('RGB', (maxx + BORDER, maxy + BORDER), 'black')
	pixels = img.load()

	for x, y in points:
		pixels[x, y] = (255, 255, 255)

	img.save('vis.png')


def tokens (lines):
	pointre = re.compile('position=<([- ]\d+), ([- ]\d+)> velocity=<([- ]\d+), ([- ]\d+)>')

	for line in lines:
		yield map(int, pointre.match(line).group(1, 2, 3, 4))


def parse (lines):
	positions = []

	for posx, posy, velx, vely in tokens(lines):
		entry = (posx, posy, velx, vely)
		positions.append(entry)

	return positions


def distance (p1, p2):
	return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])

def minmax (points):

	minx = sys.maxsize
	miny = sys.maxsize

	maxx = 0
	maxy = 0

	for x, y, _, _ in points:
		if x < minx:
			minx = x
		elif maxx < x:
			maxx = x

		if y < miny:
			miny = y
		elif maxy < y:
			maxy = y

	return (minx, miny), (maxx, maxy)


def main ():

	points = parse(sys.stdin.readlines())

	s = 0
	prevdist = sys.maxsize

	while True:

		newpoints = [
			(px + vx, py + vy, vx, vy) for px, py, vx, vy in points
		]

		dist = distance(*minmax(newpoints))

		if prevdist < dist:
			break

		prevdist = dist

		points = newpoints
		s += 1

	print(s, "seconds")
	plot(points)


if __name__ == '__main__':
	main()
