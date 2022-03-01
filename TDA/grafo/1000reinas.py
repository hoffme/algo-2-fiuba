def se_come(tablero, n, i, j):
	for col in range(n):
		if tablero[i][col]:
			return True

	for fil in range(n):
		if tablero[fil][j]:
			return True

	for x in range(j):
		if tablero[i + j - x][x] or tablero[i - j + x][x]:
			return True

	for x in range(j + 1, n):
		if tablero[i + j - x][x] or tablero[i - j + x][x]:
			return True

def es_compatible(tablero, n, sol):
	for x, y in sol:
		if se_come(tablero, n, x, y):
			return False
	return True

def reinas(tablero, fila, n, sol):
	if fila == n:
		return es_compatible(tablero, n, sol)

	if not es_compatible(tablero, n, sol):
		return False

	for columna in range(n):
		tablero[fila][columna] = True
		sol.append((fila,columna))
		if reinas(tablero, fila + 1, n, sol):
			return True
		else:
			sol.remove((fila,columna))
			tablero[fila][columna] = False

	return True

def generar_tablero(tamaño):
	tablero = {}
	for fila in range(tamaño):
		tablero[fila] = {}
		for columna in range(tamaño):
			tablero[fila][columna] = 0
	return tablero