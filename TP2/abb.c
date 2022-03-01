#define _POSIX_C_SOURCE 200809L

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

/* -------------------------------- 		DEFINICIONES		-------------------------------- */

typedef struct nodo nodo_t;
typedef int (*abb_cmp_t) (void*, void*);
typedef int (*abb_cmp_rango_t) (void*, void*);
typedef bool (*abb_visitar_t) (void*, void*);

/* -------------------------------- 		ESTRUCTURAS		-------------------------------- */

typedef struct abb {
	nodo_t* raiz;
	abb_cmp_t f_comp;
	size_t cant;
} abb_t;

struct nodo {
	nodo_t* izq;
	nodo_t* der;
	void* clave;
};

/* -------------------------------- 	PRIMITIVAS DE NODO 	-------------------------------- */

nodo_t* nodo_crear_abb(void* clave) {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;

	nodo->clave = clave;
	nodo->izq = NULL;
	nodo->der = NULL;

	return nodo;
}

void nodo_destruir(nodo_t* nodo, bool recursiva) {
	if (!nodo) return;
	if (recursiva) {
		nodo_destruir(nodo->izq, true);
		nodo_destruir(nodo->der, true);
	}
	free(nodo);
}

void _nodo_rango_insertar_en_lista(abb_cmp_rango_t f_comp, nodo_t* nodo, void* desde, void* hasta, lista_t* lista, bool asc, int cant) {
	if (!nodo || (cant >= 0 && lista_largo(lista) >= cant)) return;

	bool izq = f_comp(desde, nodo->clave) <= 0;
	bool der = f_comp(hasta, nodo->clave) >= 0;

	if (asc && izq) _nodo_rango_insertar_en_lista(f_comp, nodo->izq, desde, hasta, lista, asc, cant);
	else if (!asc && der) _nodo_rango_insertar_en_lista(f_comp, nodo->der, desde, hasta, lista, asc, cant);

	if (izq && der && (cant < 0 || cant > lista_largo(lista))) lista_insertar_ultimo(lista, nodo->clave);

	if (asc && der) _nodo_rango_insertar_en_lista(f_comp, nodo->der, desde, hasta, lista, asc, cant);
	else if (!asc && izq) _nodo_rango_insertar_en_lista(f_comp, nodo->izq, desde, hasta, lista, asc, cant);
}

/* -------------------------------- 	AUXILIARES DE ABB 	-------------------------------- */

void borrar_dos_hijos(abb_t* arbol, nodo_t* nodo) {
	nodo_t* nodo_padre_cambio = nodo;
	nodo_t* nodo_cambio = nodo->der;

	if (!nodo_cambio->izq) {
		nodo_padre_cambio->der = nodo_cambio->der;
	} else {
		while (nodo_cambio->izq) {
			nodo_padre_cambio = nodo_cambio;
			nodo_cambio = nodo_cambio->izq;
		}
		nodo_padre_cambio->izq = nodo_cambio->der;
	}

	nodo->clave = nodo_cambio->clave;
	free(nodo_cambio);
}

void borrar_simple_hijos(abb_t* arbol, nodo_t* padre, nodo_t* nodo) {
	nodo_t* hijo_nodo = NULL;
	if (nodo->der) hijo_nodo = nodo->der;
	else if (nodo->izq) hijo_nodo = nodo->izq;

	if (!padre) arbol->raiz = hijo_nodo;
	else {
		int dir = arbol->f_comp(padre->clave, nodo->clave);		
		if (dir > 0) padre->izq = hijo_nodo;
		else if (dir < 0) padre->der = hijo_nodo;
	}
	nodo_destruir(nodo, false);
}

/* -------------------------------- 	PRIMITIVAS DE ABB 	-------------------------------- */


abb_t* abb_crear(abb_cmp_t cmp) {
	abb_t* abb = malloc(sizeof(abb_t));
	if (!abb) return NULL;

	abb->raiz = NULL;
	abb->f_comp = cmp;
	abb->cant = 0;

	return abb;
}

bool abb_guardar(abb_t* arbol, void* clave) {
	nodo_t* actual = arbol->raiz;
	int dir = 0;

	while (actual){
		dir = arbol->f_comp(clave, actual->clave);

		if (dir == 0){
			actual->clave = clave;
			return true;
		}
		else if (dir > 0 && actual->der) actual = actual->der;
		else if (dir < 0 && actual->izq) actual = actual->izq;
		else break;
	}

	nodo_t* n_nodo = nodo_crear_abb(clave);
	if (!n_nodo) return false;

	if (dir > 0) actual->der = n_nodo;
	else if (dir < 0) actual->izq = n_nodo;
	else arbol->raiz = n_nodo;

	++arbol->cant;
	return true;
}

void* abb_borrar(abb_t *arbol, void* clave) {
	nodo_t* actual = arbol->raiz;
	nodo_t* padre = NULL;

	while (actual){
		int dir = arbol->f_comp(clave, actual->clave);

		if (dir == 0) {
			void* clave_borrada = actual->clave;
			if (actual->izq && actual->der) borrar_dos_hijos(arbol, actual);
			else  borrar_simple_hijos(arbol, padre, actual);
			--arbol->cant;
			return clave_borrada;
		}
		padre = actual;
		if (dir > 0) actual = actual->der;
		else actual = actual->izq;
	}
	return NULL;
}

void abb_destruir(abb_t *arbol) {
	nodo_destruir(arbol->raiz, true);
	free(arbol);
}

lista_t* abb_rango_lista(abb_t *arbol, abb_cmp_rango_t f_comp, void* desde, void* hasta, bool asc, int cant) {
	if (!arbol->raiz || !f_comp) return NULL;

	lista_t* lista = lista_crear();
	if (!lista) return NULL;

	_nodo_rango_insertar_en_lista(f_comp, arbol->raiz, desde, hasta, lista, asc, cant);

	return lista;
}