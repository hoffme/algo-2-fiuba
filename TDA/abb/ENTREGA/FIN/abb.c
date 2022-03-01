#define _POSIX_C_SOURCE 200809L

#include "abb.h"
#include "pila.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ESTRUCTURAS */
typedef struct nodo{
	struct nodo* izq;
	struct nodo* der;
	char* clave;
	void* dato;
} nodo_t;

struct abb{
	nodo_t* raiz;
	size_t cantidad;
	abb_comparar_clave_t f_comp;
	abb_destruir_dato_t f_dest;
};

struct abb_iter {
	pila_t* pila;
};

/* FUNCIONES AUXILIARES */
void abb_destruir_nodos(abb_t* arbol, nodo_t* nodo);
void _nodo_in_order(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra);

/* -------------------------------   PRIMITIVAS  ABB   ------------------------------- */
/* CREAR */
abb_t* abb_crear(abb_comparar_clave_t comp, abb_destruir_dato_t dest){
	if (!comp) return NULL;

	abb_t* nuevo_abb = malloc(sizeof(abb_t));
	if (!nuevo_abb) return NULL;

	nuevo_abb->raiz = NULL;
	nuevo_abb->cantidad = 0;
	nuevo_abb->f_comp = comp;
	nuevo_abb->f_dest = dest;

	return nuevo_abb;
}

/* PERTENECE */
bool abb_pertenece(const abb_t* arbol, const char* clave){
	nodo_t* buscar = arbol->raiz;

	while (buscar != NULL){
		if (arbol->f_comp(clave, buscar->clave) == 0) return true;

		if (arbol->f_comp(clave, buscar->clave) > 0){
			buscar = buscar->der;
		} else {
			buscar = buscar->izq;
		}
	}
	return false;
}

/* CANTIDAD */
size_t abb_cantidad(abb_t* arbol){
	return arbol->cantidad;
}

/* OBTENER */
void* abb_obtener(const abb_t* arbol, const char* clave){
	nodo_t* buscar = arbol->raiz;

	while (buscar != NULL){
		if (arbol->f_comp(clave, buscar->clave) == 0) return buscar->dato;

		if (arbol->f_comp(clave, buscar->clave) > 0){
			buscar = buscar->der;
		} else {
			buscar = buscar->izq;
		}
	}
	return NULL;
}

/* GUARDAR */
bool abb_guardar(abb_t* arbol, const char* clave, void* dato){
	if (abb_cantidad(arbol) == 0){
		nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
		if (!nuevo_nodo) return false;

		nuevo_nodo->clave = strdup(clave);
		nuevo_nodo->dato = dato;
		nuevo_nodo->izq = NULL;
		nuevo_nodo->der = NULL;

		arbol->raiz = nuevo_nodo;
		arbol->cantidad++;
		return true;
	}

	nodo_t* actual = arbol->raiz;
	nodo_t* anterior = NULL;

	while (actual != NULL){
		anterior = actual;

		if (arbol->f_comp(clave, actual->clave) == 0){
			if (arbol->f_dest != NULL) arbol->f_dest(actual->dato);
			actual->dato = dato;
			return true;
		}

		if (arbol->f_comp(clave, actual->clave) > 0){
			actual = actual->der;
		} else {
			actual = actual->izq;
		}
	}

	nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo) return false;

	nuevo_nodo->clave = strdup(clave);
	nuevo_nodo->dato = dato;
	nuevo_nodo->izq = NULL;
	nuevo_nodo->der = NULL;

	if (arbol->f_comp(nuevo_nodo->clave, anterior->clave) > 0){
		anterior->der = nuevo_nodo;
	} else {
		anterior->izq = nuevo_nodo;
	}
	arbol->cantidad++;
	return true;
}

/* BORRAR */
void* abb_borrar(abb_t* arbol, const char* clave){
	if (!abb_pertenece(arbol, clave)) return NULL;

	nodo_t* actual = arbol->raiz;
	nodo_t* anterior = NULL;

	while (arbol->f_comp(clave, actual->clave) != 0){
		anterior = actual;

		if (arbol->f_comp(clave, actual->clave) > 0){
			actual = actual->der;
		} else {
			actual = actual->izq;
		}
	}

	void* devolver = actual->dato;

	// caso 1: sin hijos
	if (actual->izq == NULL && actual->der == NULL){
		if (anterior != NULL){
			if (arbol->f_comp(actual->clave, anterior->clave) > 0){
				anterior->der = NULL;
			} else {
				anterior->izq = NULL;
			}
		}	

		//if (arbol->f_dest != NULL) arbol->f_dest(actual->dato);
		free(actual->clave);
		free(actual);

	// caso 2: dos hijos
	} else if (actual->izq != NULL && actual->der != NULL){
		anterior = actual;
		nodo_t* reemplazo = actual->der;

		while (reemplazo && reemplazo->izq != NULL){
			anterior = reemplazo;
			reemplazo = reemplazo->izq;
		}

		//if (arbol->f_dest != NULL) arbol->f_dest(actual->dato);
		free(actual->clave);

		actual->clave = reemplazo->clave;
		actual->dato = reemplazo->dato;

		if (anterior != actual){
			anterior->izq = reemplazo->der;
		}else{
			anterior->der = NULL;
		}

		free(reemplazo);
	
	// caso 3: solo un hijo
	} else {
		nodo_t* reemplazo = NULL;
		if (actual->izq != NULL){
			reemplazo = actual->izq;
		} else if (actual->der != NULL) {
			reemplazo = actual->der;
		}

		if (anterior != NULL){
			if (arbol->f_comp(actual->clave, anterior->clave) > 0){
				anterior->der = reemplazo;
			} else {
				anterior->izq = reemplazo;
			}
		} else {
			arbol->raiz = reemplazo;
		}

		//if (arbol->f_dest != NULL) arbol->f_dest(actual->dato);
		free(actual->clave);
		free(actual);
	}

	arbol->cantidad--;
	if (abb_cantidad(arbol) == 0) arbol->raiz = NULL;

	return devolver;
}

/* DESTRUIR */
void abb_destruir(abb_t* arbol){
	abb_destruir_nodos(arbol, arbol->raiz);
	free(arbol);
}

/* AUX: destruir_nodos */
void abb_destruir_nodos(abb_t* arbol, nodo_t* nodo){
	if (!nodo) return;

	abb_destruir_nodos(arbol, nodo->izq);
	abb_destruir_nodos(arbol, nodo->der);

	if (arbol->f_dest != NULL) arbol->f_dest(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

/* --------------------------   PRIMITIVA ITERADOR INTERNO   ------------------------- */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	if (!arbol->raiz) return;
	_nodo_in_order(arbol->raiz, visitar, extra);
}

/* AUX: nodo_in_order */
void _nodo_in_order(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra) {
	if (!nodo) return;
	_nodo_in_order(nodo->izq, visitar, extra);
	visitar(nodo->clave, nodo->dato, extra);
	_nodo_in_order(nodo->der, visitar, extra);
}

/* -------------------------   PRIMITIVAS ITERADOR EXTERNO   ------------------------- */
/* CREAR */
abb_iter_t* abb_iter_in_crear(const abb_t* arbol){
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if (!iter) return NULL;

	pila_t* pila = pila_crear();
	if (!pila){
		free(iter);
		return NULL;
	}

	nodo_t* act = arbol->raiz;
	while (act){
		if (!pila_apilar(pila, act)){
			pila_destruir(pila);
			free(iter);
		}
		act = act->izq;
	}

	iter->pila = pila;
	return iter;
}

/* AVANZAR */
bool abb_iter_in_avanzar(abb_iter_t *iter){
	nodo_t* act = pila_desapilar(iter->pila);
	if (!act) return false;

	if (act->der){
		nodo_t* sig = act->der;
		while (sig){
			if (!pila_apilar(iter->pila, sig)){
				pila_destruir(iter->pila);
				free(iter);
			}
			sig = sig->izq;
		}
	}
	return true;
}

/* AL FINAL */
bool abb_iter_in_al_final(const abb_iter_t* iter){
	return pila_ver_tope(iter->pila) == NULL;
}

/* VER ACTUAL */
const char* abb_iter_in_ver_actual(const abb_iter_t* iter){
	if (abb_iter_in_al_final(iter)) return NULL;
	return ((nodo_t*)pila_ver_tope(iter->pila))->clave;
}

/* DESTRUIR */
void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}
