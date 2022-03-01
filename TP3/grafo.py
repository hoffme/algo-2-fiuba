import random
from grafo_lib import *

class PesoArista:
	def __init__(self, **info):
		self.info = info
		self.info['_sin_peso'] = 0

	def obtener(self, clave):
		return self.info[clave]

class Grafo:
	def __init__(self, dirigido, pesado):
		self.dirigido = dirigido
		self.pesado = pesado
		self.arista_peso = '_sin_peso'
		self.cantidad_aristas = 0
		self.grafo = {}

	def agregar_vertice(self, vertice):
		if vertice in self.grafo:
			raise Exception(f"El vertice {str(vertice)} ya se encuentra en el grafo")

		self.grafo[vertice] = {}

	def eliminar_vertice(self, vertice):
		if vertice not in self.grafo:
			raise Exception(f"El vertice {str(vertice)} no se encuentra en el grafo")

		if self.dirigido:
			for artistas_vertice in self.grafo.Values():
				if vertice in artistas_vertice:
					del artistas_vertice[vertice]
		else:
			for vertice2 in self.grafo[vertice].keys():
				del self.grafo[vertice2][vertice]

		del self.grafo[vertice]

	def agregar_arista(self, vertice1, vertice2, peso = PesoArista()):
		if vertice1 not in self.grafo:
			raise Exception(f"El vertice {str(vertice1)} no se encuentra en el grafo")

		if vertice2 not in self.grafo:
			raise Exception(f"El vertice {str(vertice2)} no se encuentra en el grafo")

		self.grafo[vertice1][vertice2] = peso;

		if not self.dirigido:
			self.grafo[vertice2][vertice1] = peso

		self.cantidad_aristas += 1

	def eliminar_arista(self, vertice1, vertice2):
		if vertice1 not in self.grafo:
			raise Exception(f"El vertice {vertice1} no se encuentra en el grafo")

		if vertice2 not in self.grafo:
			raise Exception(f"El vertice {vertice2} no se encuentra en el grafo")

		del self.grafo[vertice1][vertice2]

		if not self.dirigido:
			del self.grafo[vertice2][vertice1]

		self.cantidad_aristas -= 1

	def existe_arista(self, vertice1, vertice2):
		return (vertice1 in self.grafo and vertice2 in self.grafo[vertice1])

	def peso(self, vertice1, vertice2):
		return self.grafo[vertice1][vertice2].obtener(self.arista_peso)

	def pesos(self, vertice1, vertice2):
		return self.grafo[vertice1][vertice2]

	def cantidad_vertices(self):
		return len(self.grafo)

	def cantidad_artistas(self, vertice1):
		try:
			return len(self.grafo[vertice1])
		except:
			raise Exception(f"El vertice {vertice1} no se encuentra en el grafo")

	def vertice_aleatorio(self):
		return random.choice(list(self.grafo.keys()))

	def todos_vertices(self):
		return self.grafo.keys()

	def todas_aristas(self):
		res = []

		for v in self:
			for w in self.adyacentes(v):
				res.append((v, w, self.peso(v,w)))

		return res

	def todas_aristas_pesos(self):
		res = []

		for v in self:
			for w in self.adyacentes(v):
				res.append((v, w, self.pesos(v,w)))

		return res

	def adyacentes(self, vertice):
		try:
			return self.grafo[vertice].keys()
		except:
			raise Exception(f"El vertice {str(vertice)} no se encuentra en el grafo")

	def __iter__(self):
		return self.grafo.__iter__()

	def __contains__(self, vertice):
		return vertice in self.grafo

	def __len__(self):
		return self.cantidad_aristas
