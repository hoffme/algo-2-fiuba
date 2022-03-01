from grafo import Grafo

def bfs(grafo, origen):
	visitados = set()
	padre = {}
	orden = {}
	cola = []
	
	visitados.add(origen)
	padre[origen] = None
	orden[origen] = 0
	cola.insert(0, origen)

	while len(cola) > 0:

		vertice = cola.pop()
		for vertice_ady in grafo.adyacentes(vertice):
			if vertice_ady not in visitados:

				visitados.add(vertice_ady)
				cola.insert(0, vertice_ady)
				padre[vertice_ady] = vertice
				orden[vertice_ady] = orden[vertice] + 1

	return padre, orden

def dfs(grafo, vertice, visitados, padre, orden):
	visitados.add(vertice)

	for vertice_ady in grafo.adyacentes(vertice):
		if vertice_ady not in visitados:

			padre[vertice_ady] = vertice
			orden[vertice_ady] = orden[vertice] + 1
			dfs(grafo, vertice_ady, visitados, padre, orden)

def recorrido_dfs(grafo):
	visitados = set()
	padre = {}
	orden = {}

	for vertice in grafo:
		if vertice not in visitados:
			orden[vertice] = 0
			padre[vertice] = None
			dfs(grafo, vertice, visitados, padre, orden)

	return padre, orden

def orden_topologico_bfs(grafo):
	grados = {}

	for v in grafo:
		grados[v] = 0
	
	for v in grafo:
		for w in grafo.adyacentes(v):
			grados[w] += 1

	cola = []

	for v in grafo:
		if grados[v] == 0:
			cola.insert(0, v)

	result = []

	while len(cola) > 0:
		v = cola.pop()
		result.append(v)

		for w in grafo.adyacentes(v):
			
			grados[w] -= 1
			if grados[w] == 0:
				cola.insert(0, w)

	return result

def orden_topologico_dfs_rec(grafo, vertice, pila, visitados):
	visitados.add(vertice)
	for w in grafo.adyacentes(vertice):
		if w not in visitados:
			orden_topologico_dfs_rec(grafo, w, pila, visitados)

	pila.insert(len(pila), vertice)

def orden_topologico_dfs(grafo):
	visitados = set()
	pila = []

	for v in grafo:
		if v not in visitados:
			orden_topologico_dfs_rec(grafo, v, pila, visitados)

	return pila

# g = Grafo(True)
# with open("grafo_grande.txt") as archivo:
# 	for linea in archivo.readlines():
# 		v1, v2, peso= linea.split()
	
# 		try:
# 			g.agregar_arista(v1, v2, peso)
# 		except Exception as e:
# 			print(f"Error {e}")