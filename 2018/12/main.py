#!/usr/bin/env python

import sys

class State:

	def __init__ (self):
		self.state = {}

	def __setitem__ (self, key, value):
		if value != 0:
			self.state[key] = value

	def __getitem__ (self, key):
		if key not in self.state:
			return 0
		return self.state[key]

	def __iter__ (self):
		yield from self.state

	def neighbours (self, key):
		return tuple([self[key + i] for i in range(-2, 3)])

	def __repr__ (self):
		return repr(self.state)

class Rules:

	def __init__ (self):
		self.rules = {}

	def __setitem__ (self, key, value):
		self.rules[key] = value

	def __getitem__ (self, key):
		return self.rules[key]

	def apply (self, state):

		newstate = State()

		for i in range(min(state) - 1, max(state) + 2):
			newstate[i] = self.rules[state.neighbours(i)]

		return newstate


def statesrange (states):

	minimum = 0
	maximum = 0

	for state in states:
		k = sorted(state)

		if k[0] < minimum:
			minimum = k[0]

		if maximum < k[-1]:
			maximum = k[-1]
	return minimum, maximum

def savevis (states, path):

	mn, mx = statesrange(states)

	cw = 6
	ch = 6

	with open(path, 'w') as vis:
		vis.write('<svg xmlns="http://www.w3.org/2000/svg">\n')
		for y in range(len(states)):
			for x in states[y]:
				vis.write('\t<rect width="{}" height="{}" x="{}" y="{}" stroke="white"/>\n'.format(
					cw, ch, (x - mn) * cw, y * ch, vis
				))
		vis.write("</svg>")

SYMTAB = { '#': 1, '.': 0 }
def tr (t):
	return SYMTAB[t]

def parse (lines):

	state = State()

	_, line = lines.pop(0).split(': ')
	for i in range(len(line)):
		state[i] = tr(line[i])

	rules = Rules()
	for line in lines:
		if not line:
			continue

		tokens = line.split(' => ')

		pattern = [ tr(t) for t in tokens[0] ]

		rules[tuple(pattern)] = tr(tokens[1])

	return state, rules

def solution1 (state, rules):
	for _ in range(20):
		state = rules.apply(state)

	print('Living cell position sum for 20 gens: ', sum(state))

def solution2 (state, rules):

	csum, psum = 0, 0
	cdiff, pdiff = 0, 0
	step = 0
	while True:
		state = rules.apply(state)
		csum = sum(state)

		cdiff = csum - psum
		if cdiff == pdiff:
			break
		pdiff = cdiff
		psum = csum
		step += 1

	csum += cdiff * (50000000000 - step - 1)

	print("Living cell position sum for 5b gens: ", csum)

def main ():
	lines = [ line.rstrip() for line in sys.stdin.readlines() ]

	start, rules = parse(lines)

	solution1(start, rules)

	solution2(start, rules)

	states = [start]
	for _ in range(1, 100):
		states.append(rules.apply(states[-1]))

	savevis(states, 'vis.svg')

if __name__ == '__main__':
	main()
