from grafo import Grafo, PesoArista
import grafo_lib as Glib
import os.path
import sys
from collections import defaultdict

VUE_POS = {
	'ID-AE-IN' : 0,
	'ID-AE-OUT' : 1,
	'TIEMPO' : 2,
	'PRECIO' : 3,
	'CANT' : 4
}

AE_POS = {
	'ID' : 1,
	'CIU' : 0,
	'LAT' : 2,
	'LON' : 3
}

def listar_operaciones(comandos, ciudades, mapa_vuelos, aeropuertos):
	print(*comandos.keys(), sep='\n')
	return True

def camino_mas(param, ciudades, mapa_vuelos, aeropuertos):
	if 'rapido' in param['tipo']: mapa_vuelos.arista_peso = 'tiempo'
	if 'barato' in param['tipo']: mapa_vuelos.arista_peso = 'precio'
	
	recorrido, costo = Glib.camino_minimo(mapa_vuelos, param['origen'], param['destino'], 1)
	
	if param['origen'] not in recorrido:
		return False

	text = param['origen']
	act = param['origen']
	while act != param['destino']:
		text += ' -> ' + recorrido[act]
		act = recorrido[act]

	print(text)
	print(f'costo: {costo}')

	return True

def camino_escalas(param, ciudades, mapa_vuelos, aeropuertos):
	mapa_vuelos.arista_peso = '_sin_peso'
	recorrido, costo = Glib.camino_minimo(mapa_vuelos, param['origen'], param['destino'], 0)
	
	if param['origen'] not in recorrido:
		return False

	text = param['origen']
	act = param['origen']
	while act != param['destino']:
		text += ' -> ' + recorrido[act]
		act = recorrido[act]

	print(text)
	print(f'costo: {costo}')

	return True

def centralidad(param, ciudades, mapa_vuelos, aeropuertos):
	mapa_vuelos.arista_peso = '_sin_peso'

	cent = list(Glib.centralidad(mapa_vuelos).items())
	cent.sort(key=lambda x: x[1], reverse=True)

	for i in range(int(param['cant'])):
		if i >= len(cent):
			break
		print(cent[i][0])

	return True

def centralidad_aprox(param, ciudades, mapa_vuelos, aeropuertos):
	mapa_vuelos.arista_peso = 'cant_vuelos'

	cent = list(Glib.randomWalks(mapa_vuelos, 5000, 100).items())
	cent.sort(key=lambda x: x[1], reverse=True)

	for i in range(int(param['cant'])):
		if i >= len(cent):
			break
		print(cent[i][0])

	return True

def pagerank(param, ciudades, mapa_vuelos, aeropuertos):
	mapa_vuelos.arista_peso = 'cant_vuelos'

	pr = list(Glib.pageRank(mapa_vuelos, 0.85, 100).items())
	pr.sort(key=lambda x: x[1], reverse=True)

	for i in range(int(param['cant'])):
		if i >= len(pr):
			break
		print(pr[i][0])

	return True

def nueva_aerolinea(param, ciudades, mapa_vuelos, aeropuertos):
	mapa_vuelos.arista_peso = 'precio'

	vuelos = Glib.kruskal(mapa_vuelos)

	with open(param['ruta'], 'w') as ruta:
		for ae_in, ae_out, peso in vuelos:
			linea = f'{ae_in},{ae_out},{peso}\n'
			ruta.write(linea)

	return True

def recorrer_mundo(param, ciudades, mapa_vuelos, aeropuertos):
	if param['origen'] not in ciudades: return False

	mapa_ciudades = Grafo(True, True)
	for ciu in ciudades.keys():
		mapa_ciudades.agregar_vertice(ciu)

	aristas = mapa_vuelos.todas_aristas_pesos()
	sorted(aristas, key=lambda x: x[2].obtener('precio'))

	for v, w, peso in aristas:

		if mapa_ciudades.existe_arista(v, w): continue
		peso_vuelo_ciudades = PesoArista(
			tiempo = 		peso.obtener('tiempo'),
			precio = 		peso.obtener('precio'),
			cant_vuelos = 	peso.obtener('cant_vuelos'),
			origen =		v,
			llegada = 		w
		)

		mapa_ciudades.agregar_arista(aeropuertos[v][0], aeropuertos[w][0], peso_vuelo_ciudades)

	mapa_ciudades.arista_peso = 'precio'
	print(Glib.lista_de_tendido_minimo(mapa_ciudades, param['origen']))
	
	return True

def recorrer_mundo_aprox(param, ciudades, mapa_vuelos, aeropuertos):
	pass

def vacaciones(param, ciudades, mapa_vuelos, aeropuertos):
	pass

def itinerario(param, ciudades, mapa_vuelos, aeropuertos):
	pass

def exportar_kml(param, ciudades, mapa_vuelos, aeropuertos):
	pass

COMM = {
	'listar_operaciones' : 		{'func' : listar_operaciones, 	'param_ok' : lambda x: True, 							'param' : None},
	'camino_mas' : 				{'func' : camino_mas, 			'param_ok' : lambda x: x[0] in ('rapido','barato'),  	'param' : {'tipo' : 1, 'origen' : 2, 'destino' : 3}},
	'camino_escalas' : 			{'func' : camino_escalas, 		'param_ok' : lambda x: True, 						 	'param' : {'origen' : 1, 'destino' : 2,}},
	'centralidad' : 			{'func' : centralidad,			'param_ok' : lambda x: int(x[0]) > 0,				 	'param' : {'cant' : 1}},
	'centralidad_aprox' : 		{'func' : centralidad_aprox,	'param_ok' : lambda x: int(x[0]) > 0,				 	'param' : {'cant' : 1}},
	'pagerank' : 				{'func' : pagerank,				'param_ok' : lambda x: int(x[0]) > 0,				 	'param' : {'cant' : 1}},
	'nueva_aerolinea' : 		{'func' : nueva_aerolinea,		'param_ok' : lambda x: not os.path.exists(x[0]),		 	'param' : {'ruta' : 1}},
	'recorrer_mundo' : 			{'func' : recorrer_mundo,		'param_ok' : lambda x: True,						 	'param' : {'origen' : 1}},
	'recorrer_mundo_aprox' :	{'func' : recorrer_mundo_aprox,	'param_ok' : lambda x: True,						 	'param' : {'origen' : 1}},
	'vacaciones' : 				{'func' : vacaciones,			'param_ok' : lambda x: int(x[1]) > 0,				 	'param' : {'origen' : 1, 'cant' : 2}},
	'itinerario' : 				{'func' : itinerario,			'param_ok' : lambda x: os.path.exists(x[0]),		 	'param' : {'ruta' : 1}},
	'exportar_kml' : 			{'func' : exportar_kml,			'param_ok' : lambda x: os.path.exists(x[0]),		 	'param' : {'archivo' : 1}},
	'salir' : 					{'func' : None,					'param_ok' : lambda x: True,						 	'param' : None}
}

def nuevo_comando():
	ent = input().split(' ')
	salir = True
	funcion = None
	parametros = {}

	if ent[0] not in COMM: 
		return funcion, parametros, not salir
	com_conf = COMM[ent[0]]

	if ent[0] == 'salir': 
		return funcion, parametros, salir

	if ent[0] == 'listar_operaciones':
		return com_conf['func'], COMM, not salir

	if len(ent) == len(com_conf['param']) + 1 and com_conf['param_ok'](ent[1:]):		
		funcion = com_conf['func']

		for nom_param, pos in com_conf['param'].items():
			parametros[nom_param] = ent[pos]

	return funcion, parametros, not salir

def agregar_vuelos(ruta_archivo, ciudades, mapa_vuelos):
	with open(ruta_archivo) as archivo:
		for linea in archivo.readlines():
			vuelo = linea.rstrip('\n').split(',')

			peso = PesoArista(
				tiempo = 		int(vuelo[VUE_POS['TIEMPO']]),
				precio = 		int(vuelo[VUE_POS['PRECIO']]),
				cant_vuelos = 	int(vuelo[VUE_POS['CANT']])
			)

			mapa_vuelos.agregar_arista(vuelo[VUE_POS['ID-AE-IN']], vuelo[VUE_POS['ID-AE-OUT']], peso)

	return True

def agregar_aeropuertos(ruta_archivo, ciudades, mapa_vuelos, aeropuertos):
	with open(ruta_archivo) as arch:
		for linea in arch.readlines():
			aeport = linea.rstrip('\n').split(',')

			ciudades[aeport[AE_POS['CIU']]].append(aeport[AE_POS['ID']])
			aeropuertos[aeport[AE_POS['ID']]] = [aeport[AE_POS['CIU']], aeport[AE_POS['LAT']], aeport[AE_POS['LON']]]

			mapa_vuelos.agregar_vertice(aeport[AE_POS['ID']])

	return True

def cargar_datos(ciudades, mapa_vuelos, aeropuertos):
	if len(sys.argv) != 3: return False
	if not os.path.exists(sys.argv[1]): return False
	if not os.path.exists(sys.argv[2]): return False

	aer_ok = agregar_aeropuertos(sys.argv[1], ciudades, mapa_vuelos, aeropuertos)
	vue_ok = agregar_vuelos(sys.argv[2], ciudades, mapa_vuelos)

	return aer_ok and vue_ok

def flycombi():
	ciudades = defaultdict(lambda: [], {})
	aeropuertos = {}
	mapa_vuelos = Grafo(True, True)
	ultimo_comando = None
	ok = False

	if not cargar_datos(ciudades, mapa_vuelos, aeropuertos):
		print('Error al cargar datos')
		return

	while True:
		funcion, parametros, salir = nuevo_comando()
		if salir:
			break

		if funcion:
			ok, ultimo_comando = funcion(parametros, ciudades, mapa_vuelos, aeropuertos)
		
		if funcion and ok: print('OK')
		else: print('ERROR')

if __name__ == "__main__":
    flycombi()