import heapq
import random
from collections import defaultdict
from disjoin import DisjointSet

def bfs(grafo, origen, visitados = set()):
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

def dfs(grafo, origen, visitados = set()):
	padre = {}
	orden = {}
	pila = []
	
	visitados.add(origen)
	padre[origen] = None
	orden[origen] = 0
	pila.append(origen)

	while len(pila) > 0 and len(visitados) < grafo.cantidad_vertices():
		vertice = pila.pop()

		for vertice_ady in grafo.adyacentes(vertice):
			if vertice_ady not in visitados:
				visitados.add(vertice_ady)
				pila.append(vertice_ady)
				padre[vertice_ady] = vertice
				orden[vertice_ady] = orden[vertice] + 1

	return padre, orden

def wrp_noconexo(grafo, f):
	visitados = set()
	padre = {}
	orden = {}

	for vertice in grafo:
		if vertice not in visitados:
			n_padre, n_orden = f(grafo, vertice, visitados)
			padre += n_padre
			orden += n_orden

	return padre, orden

def recorrer(grafo, origen, tipo = 'ancho'):
	if tipo == 'ancho': return wrp_noconexo(grafo, bfs)
	elif tipo == 'profundo': return wrp_noconexo(grafo, dfs)

def dijkstra(grafo, origen):
	distancia = defaultdict(lambda: float('inf'), {origen : 0})
	padre = {origen : None}
	
	heap_min = [(0, origen)]

	while len(heap_min) > 0:
		dist_v, v = heapq.heappop(heap_min)

		for w in grafo.adyacentes(v):
			dist_w = dist_v + grafo.peso(v, w)
			
			if dist_w < distancia[w]:
				distancia[w] = dist_w
				padre[w] = v
				heapq.heappush(heap_min, (dist_w, w))

	return padre, distancia

def bellmanForm(grafo, origen):
	distancia = defaultdict(lambda: float('inf'), {origen : 0})
	padre = {origen : None}

	aristas = grafo.todas_aristas()

	for i in range(len(grafo)):
		for v, w, peso in aristas:
			if distancia[v] + peso < distancia[w]:
				padre[w] = v
				distancia[w] = distancia[v] + peso

	for v, w, peso in aristas:
		if distancia[v] + peso < distancia[w]:
			return None, None

	return padre, distancia

def camino_minimo(grafo, origen, destino, peso = 0):
	if peso > 0: recorrido, distancia = dijkstra(grafo, origen)
	elif peso < 0: recorrido, distancia = bellmanForm(grafo, origen)
	else: recorrido, distancia = bfs(grafo, origen)

	recorrido = {}
	act = destino
	while act != origen:
		if act not in padre: return {}, 0
		recorrido[padre[act]] = act
		act = padre[act]

	return recorrido, distancia[destino]

def floydWarshall(grafo):
	dist = defaultdict(lambda: {}, {})

	for v in grafo:
		for w in grafo:
			if v == w: dist[v][w] = 0
			elif grafo.existe_arista(v, w): dist[v][w] = grafo.arista(v, w)
			else: dist[v][w] = float('inf')

	for k in grafo:
		for i in grafo:
			for j in grafo:
				if dist[i][j] > dist[i][k] + dist[k][j]:
					dist[i][j] = dist[i][k] + dist[k][j]

	return dist

def filtrar_y_ordenar(distancia):
	res = []
	for v, dist in distancia.items():
		if dist != float('inf'): res.append((v, dist))

	return sorted(res, key=lambda x: x[1], reverse=True)

def centralidad(grafo):
	centralidad = defaultdict(lambda: 0, {})
	
	for v in grafo:
		padre, distancia = dijkstra(grafo, v)
		cent_aux = defaultdict(lambda: 0, {})

		list_dist = filtrar_y_ordenar(distancia)

		for w, peso in list_dist:
			cent_aux[padre[w]] += 1 + cent_aux[w]

		for w in grafo:
			if w == v: continue
			centralidad[w] += cent_aux[w]

	return centralidad

def vert_sig_aleat(grafo, vert):
	pesos = {}
	for w in grafo.adyacentes(vert):
		pesos[w] = grafo.peso(vert, w)

	total = sum(pesos.values())
	rand = random.uniform(0, total)
	acum = 0
	for vertice, peso_arista in pesos.items():
		if acum + peso_arista >= rand:
			return vertice
		acum += peso_arista

def randomWalks(grafo, largo, cant_rec, sig_vert = vert_sig_aleat):
	rw = defaultdict(lambda: 0, {})

	for i in range(cant_rec):
		v = grafo.vertice_aleatorio()

		for j in range(largo):			
			if len(grafo.adyacentes(v)) == 0: break
			v = sig_vert(grafo, v)
			rw[v] += 1

	return rw

def pageRank(grafo, const_amort = 0.85, iteraciones = 60):
	const_ini = (1 - const_amort) / grafo.cantidad_vertices()

	aristas = defaultdict(lambda: [], {})

	for v in grafo:
		for w in grafo.adyacentes(v):
			aristas[w].append(v)

	pag_rank = defaultdict(lambda: const_ini, {})

	for i in range(iteraciones):
		for v, padres in aristas.items():
			suma = 0
			for padre in padres:
				suma += pag_rank[padre] / grafo.cantidad_artistas(padre)

			pag_rank[v] = const_ini + const_amort * suma

	return pag_rank

def kruskal(grafo):
	cdij = DisjointSet(grafo.todos_vertices())
	aristas = sorted(grafo.todas_aristas(), key=lambda x: x[2])

	res = []
	for v, w, peso in aristas:
		if cdij.buscar(v) == cdij.buscar(w): continue
		res.append((v, w, peso))
		cdij.unir(v, w)

	return res

