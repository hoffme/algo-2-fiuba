#define _POSIX_C_SOURCE 200809L
#include "pila.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* -------------------------------- 		DEFINICIONES		-------------------------------- */

typedef struct nodo nodo_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);

typedef void (*abb_destruir_dato_t) (void *);

nodo_t* buscar_padre(nodo_t* nodo, abb_comparar_clave_t f_comp, const char* clave);

nodo_t* buscar_nodo(nodo_t* nodo, abb_comparar_clave_t f_comp, const char* clave);

void _nodo_in_order(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra, bool* estado);

/* -------------------------------- 		ESTRUCTURAS		-------------------------------- */

typedef struct abb {
	nodo_t* raiz;
	abb_comparar_clave_t f_comp;
	abb_destruir_dato_t f_dest;
	size_t cant;
} abb_t;

struct nodo {
	nodo_t* izq;
	nodo_t* der;
	char* clave;
	void* dato;
};

typedef struct abb_iter {
	pila_t* pila;
} abb_iter_t;

/* -------------------------------- 	PRIMITIVAS DE NODO 	-------------------------------- */

nodo_t* nodo_crear(const char* clave, void* dato) {
	/*	Dado una clave como parametro y un puntero a un dato, crea un nodo, con los hijos nulos
	 * 	y le asigna la clave y el dato y devuleve un puntero, si no se pudo crear devuelve null
	 */
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;

	char* str_clave = strdup(clave);
	if (!str_clave) {
		free(nodo);
		return NULL;	
	}

	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->clave = str_clave;
	nodo->dato = dato;

	return nodo;
}

nodo_t* nodo_hijo(const nodo_t* nodo, abb_comparar_clave_t f_comp, const char* clave) {
	/*	Dado un nodo, una funcion de comparacion y una clave del hijo, devuelve un puntero al
	 *	hijo, en caso de que no tenga o tenga pero la clave sea diferente devuelve null
	 */
	if (nodo->der && f_comp(nodo->der->clave, clave) == 0) return nodo->der;
	else if (nodo->izq && f_comp(nodo->izq->clave, clave) == 0) return nodo->izq;
	return NULL;
}

void nodo_destruir(nodo_t* nodo, abb_destruir_dato_t f_dest, bool recursiva) {
	/*	Dado un nodo, una funcion de destruccion y un booleano de recusividad, si el booleano 
	 *	es true destruye a los hijos recursivamente, destruye al dato y libera a la clave y al nodo
	 */
	if (!nodo) return;
	if (recursiva) {
		nodo_destruir(nodo->izq, f_dest, true);
		nodo_destruir(nodo->der, f_dest, true);
	}
	if (f_dest) f_dest(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

/* -------------------------------- 	PRIMITIVAS DE ABB 	-------------------------------- */


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	abb_t* abb = malloc(sizeof(abb_t));
	if (!abb) return NULL;

	abb->raiz = NULL;
	abb->f_comp = cmp;
	abb->f_dest = destruir_dato;
	abb->cant = 0;

	return abb;
}

bool abb_guardar(abb_t* arbol, const char* clave, void* dato) {
	nodo_t* padre = buscar_padre(arbol->raiz, arbol->f_comp, clave);
	
	int dir_hijo = 0;
	if (padre) dir_hijo = arbol->f_comp(padre->clave, clave);
		
	if (dir_hijo > 0 && padre->izq) {

		if (arbol->f_dest) arbol->f_dest(padre->izq->dato);
		padre->izq->dato = dato;

	} else if (dir_hijo < 0 && padre->der) {

		if (arbol->f_dest) arbol->f_dest(padre->der->dato);
		padre->der->dato = dato;

	} else if (dir_hijo > 0 && !padre->izq) {

		nodo_t* n_nodo = nodo_crear(clave, dato);
		if (!n_nodo) return false;
		padre->izq = n_nodo;
		++arbol->cant;

	} else if (dir_hijo < 0 && !padre->der) {

		nodo_t* n_nodo = nodo_crear(clave, dato);
		if (!n_nodo) return false;
		padre->der = n_nodo;
		++arbol->cant;

	} else if (arbol->raiz){

		if (arbol->f_dest) arbol->f_dest(arbol->raiz->dato);
		arbol->raiz->dato = dato;

	} else {

		nodo_t* n_nodo = nodo_crear(clave, dato);
		if (!n_nodo) return false;
		arbol->raiz = n_nodo;
		++arbol->cant;

	}

	return true;
}

void* abb_borrar(abb_t *arbol, const char *clave) {
	nodo_t* padre = buscar_padre(arbol->raiz, arbol->f_comp, clave);
	nodo_t* nodo;

	if (!padre && !arbol->raiz) return NULL;
	else if (!padre && arbol->raiz) nodo = arbol->raiz;
	else nodo = nodo_hijo(padre, arbol->f_comp, clave);
	
	if (!nodo) return NULL;

	void* dato = nodo->dato;

	if (nodo->der && nodo->izq) {		//	caso con 2 hijos

		nodo_t* nodo_padre_cambio = nodo;
		nodo_t* nodo_cambio = nodo->der;

		if (!nodo_padre_cambio->der->izq) {

			if (!nodo_padre_cambio->der->der) nodo_padre_cambio->der = NULL;
			else nodo_padre_cambio->der = nodo_padre_cambio->der->der;

		} else {

			while (nodo_cambio->izq) {
				nodo_padre_cambio = nodo_cambio;
				nodo_cambio = nodo_cambio->izq;
			}
			if (!nodo_padre_cambio->izq->der) nodo_padre_cambio->izq = NULL;
			else nodo_padre_cambio->izq = nodo_padre_cambio->izq->der;

		}
		
		free(nodo->clave);
		nodo->clave = nodo_cambio->clave;
		nodo->dato = nodo_cambio->dato;
		free(nodo_cambio);

	} else {	//	caso con 1 o 0 hijos

		nodo_t* hijo_nodo = NULL;
		if (nodo->der) hijo_nodo = nodo->der;
		else if (nodo->izq) hijo_nodo = nodo->izq;

		if (!padre) arbol->raiz = hijo_nodo;
		else {
			int dir_nodo = arbol->f_comp(padre->clave, nodo->clave);		
			if (dir_nodo > 0) padre->izq = hijo_nodo;
			else if (dir_nodo < 0) padre->der = hijo_nodo;
		}
		nodo_destruir(nodo, NULL, false);
	}
	
	--arbol->cant;
	return dato;
}

void* abb_obtener(const abb_t *arbol, const char *clave) {
	if (!arbol->raiz) return NULL;
	nodo_t*	nodo = buscar_nodo(arbol->raiz, arbol->f_comp, clave);
	if (!nodo) return NULL;
	return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	if (!arbol->raiz) return NULL;
	return buscar_nodo(arbol->raiz, arbol->f_comp, clave) != NULL;
}

size_t abb_cantidad(abb_t *arbol) {
	return arbol->cant;
}

void abb_destruir(abb_t *arbol) {
	nodo_destruir(arbol->raiz, arbol->f_dest, true);
	free(arbol);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	if (!arbol->raiz) return;
	bool continuar = true;
	_nodo_in_order(arbol->raiz, visitar, extra, &continuar);
}

/* -------------------------------- 	AUXILIARES DE ABB 	-------------------------------- */

void _nodo_in_order(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra, bool* estado) {
	/* 	Recorre el arbol en in order le aplica la funcion visitar, en caso de 
	 *	que visitar devuelva false se deja de ejecutar. El estado incialmente
	 *	tiene que ser true para que itere.
	 */
	if (!nodo || !*estado) return;
	_nodo_in_order(nodo->izq, visitar, extra, estado);
	if (*estado) *estado = visitar(nodo->clave, nodo->dato, extra);
	_nodo_in_order(nodo->der, visitar, extra, estado);
}

nodo_t* buscar_padre(nodo_t* nodo, abb_comparar_clave_t f_comp, const char* clave) {
	/*	Si el hijo esta en el arbol devuelve el padre, sino devuelve el padre de la posicion 
	 *	donde se tendira que insertar, si no hay raiz o la raiz es el hijo devuelve null.
	 */
	if (!nodo) return NULL;
	
	int dir = f_comp(nodo->clave, clave);
	if (dir == 0) return  NULL;

	if ((nodo->izq && f_comp(nodo->izq->clave, clave) == 0) || (nodo->der && f_comp(nodo->der->clave, clave) == 0)) return nodo;

	if (dir > 0 && nodo->izq) return buscar_padre(nodo->izq, f_comp, clave);
	else if (dir < 0 && nodo->der) return buscar_padre(nodo->der, f_comp, clave);

	return nodo;
}

nodo_t* buscar_nodo(nodo_t* nodo, abb_comparar_clave_t f_comp, const char* clave) {
	/*	Busca la clave en el arbol y devuelve un puntero al nodo que contiene
	 *	dicha clave en caso de que no este la clave devuelve null.
	 */
	if (!nodo) return NULL;
	int dir = f_comp(nodo->clave, clave) ;
	if (dir > 0) return buscar_nodo(nodo->izq, f_comp, clave);
	else if (dir < 0) return buscar_nodo(nodo->der, f_comp, clave);
	return nodo;
}

/* -------------------------------- 	PRIMITIVAS ITERADOR 	-------------------------------- */

abb_iter_t* abb_iter_in_crear(const abb_t* arbol) {
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if (!iter) return NULL;

	pila_t* pila = pila_crear();
	if (!pila) {
		free(iter);
		return NULL;
	}

	nodo_t* act = arbol->raiz;
	while (act) {
		if (!pila_apilar(pila, act)) {
			pila_destruir(pila);
			free(iter);
		}
		act = act->izq;
	}

	iter->pila = pila;

	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
	nodo_t* act = pila_desapilar(iter->pila);
	if (!act) return false;

	if (act->der) {
		nodo_t* sig = act->der;
		while (sig) {
			if (!pila_apilar(iter->pila, sig)) {
				pila_destruir(iter->pila);
				free(iter);
			}
			sig = sig->izq;
		}
	}
	return true;
}

bool abb_iter_in_al_final(const abb_iter_t* iter) {
	return pila_ver_tope(iter->pila) == NULL;
}

const char* abb_iter_in_ver_actual(const abb_iter_t* iter) {
	if (abb_iter_in_al_final(iter)) return NULL;
	return ((nodo_t*)pila_ver_tope(iter->pila))->clave;
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	pila_destruir(iter->pila);
	free(iter);
}