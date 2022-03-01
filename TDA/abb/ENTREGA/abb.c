#include "abb.h"
#include "pila.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct nodo{
	struct nodo* izq;
	struct nodo* der;
	char* clave;
	void* dato;
}nodo_t;

struct abb{
	nodo_t* raiz;
	size_t cantidad;
	abb_comparar_clave_t f_comp;
	abb_destruir_dato_t f_dest;
};

/*AUXILIARES*/
char *strdup(const char *src);
void abb_destruir_nodos(abb_t* arbol, nodo_t* nodo);
void _nodo_in_order(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra);


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

bool abb_pertenece(const abb_t* arbol, const char* clave){
	nodo_t* buscar = arbol->raiz;

	while (buscar != NULL){
		if (arbol->f_comp(clave, buscar->clave) == 0) return true;

		if (arbol->f_comp(clave, buscar->clave) > 0){
			buscar = buscar->der;
		}else{
			buscar = buscar->izq;
		}
	}
	return false;
}

size_t abb_cantidad(abb_t* arbol){
	return arbol->cantidad;
}

void* abb_obtener(const abb_t* arbol, const char* clave){
	nodo_t* buscar = arbol->raiz;

	while (buscar != NULL){
		if (arbol->f_comp(clave, buscar->clave) == 0) return buscar->dato;

		if (arbol->f_comp(clave, buscar->clave) > 0){
			buscar = buscar->der;
		}else{
			buscar = buscar->izq;
		}
	}
	return NULL;
}

/* AUX: STRDUP */
// Duplica una cadena, asignándole al duplicado una posición en memoria dinámica.
char *strdup(const char *src){
	char *dst = malloc(strlen(src) + 1);  
	if (dst == NULL) return NULL;         
	strcpy(dst, src);                     
	return dst;                           
}


bool abb_guardar(abb_t* arbol, const char* clave, void* dato){
	// en arbol vacio
	if (abb_cantidad(arbol) == 0){
		nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
		if (!nuevo_nodo) return false;

		nuevo_nodo->clave = strdup(clave);
		nuevo_nodo->dato = dato;
		nuevo_nodo->izq = NULL;
		nuevo_nodo->der = NULL;

		arbol->raiz = nuevo_nodo;
		arbol->cantidad ++;
		return true;
	}

	//recorriendo el arbol
	nodo_t* actual = arbol->raiz;
	nodo_t* anterior = NULL;

	while (actual != NULL){
		anterior = actual;
		//la clave ya existe?
		if (arbol->f_comp(clave, actual->clave) == 0){
			if (arbol->f_dest != NULL) arbol->f_dest(actual->dato);
			actual->dato = dato;
			return true;
		}

		if (arbol->f_comp(clave, actual->clave) > 0){
			actual = actual->der;
		}else{
			actual = actual->izq;
		}
	}

	//agregando una hoja
	nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo) return false;

	nuevo_nodo->clave = strdup(clave);
	nuevo_nodo->dato = dato;
	nuevo_nodo->izq = NULL;
	nuevo_nodo->der = NULL;

	actual = nuevo_nodo;
	if (arbol->f_comp(actual->clave, anterior->clave) > 0){
		anterior->der = actual;
	}else{
		anterior->izq = actual;
	}
	arbol->cantidad ++;
	return true;
}

void* abb_borrar(abb_t* arbol, const char* clave){
	//la clave esta?
	if (!abb_pertenece(arbol, clave)) return NULL;

	//recorriendo el arbol
	nodo_t* actual = arbol->raiz;
	nodo_t* anterior = NULL;

	while (arbol->f_comp(clave, actual->clave) != 0){
		anterior = actual;

		if (arbol->f_comp(clave, actual->clave) > 0){
			actual = actual->der;
		}else{
			actual = actual->izq;
		}
	}

	void* devolver = actual->dato;
	//preparando para borrar
	//caso 1: sin hijos
	if (actual->izq == NULL && actual->der == NULL){
		// actualizar anterior, a menos que se trate de la raiz
		if (anterior != NULL){
			if (arbol->f_comp(actual->clave, anterior->clave) > 0){
				anterior->der = NULL;
			}else{
				anterior->izq = NULL;
			}
		}	

		if (arbol->f_dest != NULL) arbol->f_dest(actual->dato);
		free(actual->clave);
		free(actual);

	//caso 2: dos hijos
	} else if (actual->izq != NULL && actual->der != NULL){
		anterior = actual;
		nodo_t* reemplazo = actual->der;

		while (reemplazo && reemplazo->izq != NULL){
			anterior = reemplazo;
			reemplazo = reemplazo->izq;
		}

		// libero clave y dato del nodo a borrar
		if (arbol->f_dest != NULL) arbol->f_dest(actual->dato);
		free(actual->clave);

		// actualizo por los del reemplazante
		actual->clave = reemplazo->clave;
		actual->dato = reemplazo->dato;

		// actualizo los vinculos del nodo reemplazante
		if (anterior != actual){
			anterior->izq = reemplazo->der;
		}

		// libero nodo reemplazante
		free(reemplazo);
	
	//caso 3: solo un hijo
	} else {
		//identifico de cuál se trata
		nodo_t* reemplazo;
		if (actual->izq == NULL){
			//derecho
			reemplazo = actual->der;
		} else {
			//izquierdo
			reemplazo = actual->izq;
		}

		// actualizar anterior, a menos que se trate de la raiz
		if (anterior != NULL){
			if (arbol->f_comp(actual->clave, anterior->clave) > 0){
				anterior->der = reemplazo;
			} else {
				anterior->izq = reemplazo;
			}
		} else {
			// actualizar raiz
			arbol->raiz = reemplazo;
		}

		if (arbol->f_dest != NULL) arbol->f_dest(actual->dato);
		free(actual->clave);
		free(actual);
	}

	arbol->cantidad --;
	if (abb_cantidad(arbol) == 0) arbol->raiz = NULL;

	return devolver;
}

void abb_destruir(abb_t* arbol){
	abb_destruir_nodos(arbol, arbol->raiz);
	free(arbol);
}

void abb_destruir_nodos(abb_t* arbol, nodo_t* nodo){
	if (!nodo) return;

	abb_destruir_nodos(arbol, nodo->izq);
	abb_destruir_nodos(arbol, nodo->der);

	if (arbol->f_dest != NULL) arbol->f_dest(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	if (!arbol->raiz) return;
	_nodo_in_order(arbol->raiz, visitar, extra);
}

void _nodo_in_order(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra) {
	if (!nodo) return;
	_nodo_in_order(nodo->izq, visitar, extra);
	visitar(nodo->clave, nodo->dato, extra);
	_nodo_in_order(nodo->der, visitar, extra);
}

/* -------------------------------- 	STRUCT ITERADOR 	-------------------------------- */

struct abb_iter {
	pila_t* pila;
};

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