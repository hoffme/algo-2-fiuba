class DisjointSet:
	def __init__(self, elementos):
		self.elementos = {}
		self.padres = {}

		for x in elementos:
			self.elementos[x] = set()
			self.elementos[x].add(x)
			self.padres[x] = x

	def buscar(self, x):
		return self.elementos[self.padres[x]]

	def unir(self, x, y):
		padre_x = self.padres[x]
		padre_y = self.padres[y]

		if padre_x == padre_y or padre_x == None or padre_y == None: return

		if len(self.elementos[padre_x]) > len([padre_y]):
			for e in self.elementos[padre_x]:
				self.padres[e] = padre_y

			self.elementos[padre_y] = self.elementos[padre_y] | self.elementos[padre_x]

			del self.elementos[padre_x]
		else:
			for e in self.elementos[padre_y]:
				self.padres[e] = padre_x

			self.elementos[padre_x] = self.elementos[padre_y] | self.elementos[padre_x]

			del self.elementos[padre_y]