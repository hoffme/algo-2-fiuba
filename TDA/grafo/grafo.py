import random

class Grafo:
	def __init__(self, **kwargs):
		self.dirigido = kwargs.get('dirigido', False)
		self.pesado = kwargs.get('pesado', False)
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

	def agregar_arista(self, vertice1, vertice2, informacion = None):
		if vertice1 not in self.grafo:
			self.agregar_vertice(vertice1)

		if vertice2 not in self.grafo:
			self.agregar_vertice(vertice2)

		self.grafo[vertice1][vertice2] = informacion;

		if not self.dirigido:
			self.grafo[vertice2][vertice1] = informacion

		self.cantidad_aristas += 1

	def eliminar_arista(self, vertice1, vertice2):
		if vertice1 not in self.grafo:
			raise Exception(f"El vertice {str(vertice1)} no se encuentra en el grafo")

		if vertice2 not in self.grafo:
			raise Exception(f"El vertice {str(vertice2)} no se encuentra en el grafo")

		del self.grafo[vertice1][vertice2]

		if not self.dirigido:
			del self.grafo[vertice2][vertice1]

		self.cantidad_aristas -= 1

	def existe_arista(self, vertice1, vertice2):
		return (vertice1 in self.grafo and vertice2 in self.grafo[vertice1])

	def cantidad_vertices(self):
		return len(self.grafo)

	def cantidad_artistas(self, vertice1):
		try:
			return len(self.grafo[vertice1])
		except:
			raise Exception(f"El vertice {str(vertice1)} no se encuentra en el grafo")

	def vertice_azar(self):
		return random.choice(list(self.grafo.keys()))

	def todos_vertices(self):
		return self.grafo.keys()

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