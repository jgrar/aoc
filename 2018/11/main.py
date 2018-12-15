#!/usr/bin/env python3

import sys
from PIL import Image

class FuelGrid:

	def __init__ (self, serial, size):

		self.serial = serial
		self.width, self.height = size

		self.__generate()

	def __powerlevel (self, x, y):
		rid = x + 10
		return (rid * y + self.serial) * rid // 100 % 10 - 5

	def __generate (self):
		self.__grid = {}

		for x in range(1, self.height + 1):
			for y in range(1, self.width + 1):
				self.__grid[x, y] = self.__powerlevel(x, y)

		self.__gensumtable()

	def __gensumtable (self):

		self.__sumtable = {}

		for x in range(1, self.height + 1):
			s = 0
			for y in range(1, self.width + 1):
				s += self.__grid[x, y]

				if x == 1:
					self.__sumtable[x, y] = s
				else:
					self.__sumtable[x, y] = s + self.__sumtable[x - 1, y]

	def __iter__ (self):
		yield from self.__grid.keys()

	def __getitem__ (self, key):
		return self.__grid[key]

	def get (self, key):
		return self[key]

	def minmax (self):
		k = sorted(self, key = self.get)
		return self.get(k[0]), self.get(k[-1])

	def sumsquare (self, x, y, size):

		mx = my = 0

		if x != 1:
			mx = 1
		if y != 1:
			my = 1

		return self.__sumtable[x + size - 1, y + size - 1] \
			- self.__sumtable[x + size - 1, y - my] \
			- self.__sumtable[x - mx, y + size - 1] \
			+ self.__sumtable[x - mx, y - my]

	def maxsubsquare (self, size = 0):

		maxsum = 0

		maxsquare = (0, 0, 0)

		if size == 0:
			for sz in range(1, min(self.height, self.width) + 1):
				square = self.maxsubsquare(sz)

				if maxsum < square[0]:
					maxsum = square[0]
					maxsquare = (*square, sz)

				# once sums start declining they don't seem to recover,
				# feel free to correct me
				elif square[0] < maxsum:
					break


			return maxsquare

		for x in range(1, self.height - size + 1):
			for y in range(1, self.width - size + 1):

				sqsum = self.sumsquare(x, y, size)

				if maxsum < sqsum:
					maxsum = sqsum
					maxsquare = (sqsum, x, y)

		return maxsquare

class ColorMap:

	def __init__ (self, nrange, palette):
		self.min, self.max = nrange
		self.palette = palette

		self.mult = self.max + 1 - self.min

	def __getitem__ (self, i):

		if self.min <= i <= self.max:
			k = int((i - self.min) *  self.mult / len(self.palette))
			return self.palette[k]

		raise IndexError


def createimage (grid, palette):
	img = Image.new('RGB', (grid.width, grid.height))

	pixels = img.load()

	colors = ColorMap(grid.minmax(), palette)

	for x, y in grid:
		pixels[x - 1, y - 1] = colors[grid[x, y]]

	return img

def saveimage (img, path, zoom):

	img.resize((sz * zoom for sz in img.size)).save(path)

def hilightarea (img, x, y, size, color):

	pixels = img.load()

	for i in range(size):
		pixels[x, y + i] = color
		pixels[x + i, y] = color

		pixels[x + size, y + i] = color
		pixels[x + i, y + size] = color

	pixels[x + size, y + size] = color


GRIDSIZE  = (300, 300)
ZOOMFACT  = 2
HILIGHT   = (0xC7, 0x1F, 0x1F)
VISPATH   = 'vis.png'

PALETTE = tuple(
	tuple((int(i * 0xDD / 10) for _ in range(3)))
		for i in range(10)
)

def main ():
	serial = int(sys.stdin.read())

	grid = FuelGrid(serial, GRIDSIZE)
	img = createimage(grid, PALETTE)

	_, x, y  = grid.maxsubsquare(3)
	print('Max 3x3 area: {},{}'.format(x, y))

	hilightarea(img, x, y, 3, HILIGHT)

	_, x, y, size = grid.maxsubsquare()
	print('Max sub-area: {},{},{}'.format(x, y, size))

	hilightarea(img, x, y, size, HILIGHT)

	saveimage(img, VISPATH, ZOOMFACT)

if __name__ == '__main__':
	main()
