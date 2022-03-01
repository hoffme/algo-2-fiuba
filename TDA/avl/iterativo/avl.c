#define _POSIX_C_SOURCE 200809L
#include "pila.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* -------------------------------- 		DEFINICIONES		-------------------------------- */

typedef struct nodo nodo_t;

typedef int (*avl_comparar_clave_t) (const char *, const char *);

typedef void (*avl_destruir_dato_t) (void *);

/* -------------------------------- 		ESTRUCTURAS		-------------------------------- */

typedef struct avl {
	nodo_t* raiz;
	avl_comparar_clave_t f_comp;
	avl_destruir_dato_t f_dest;
	size_t cant;
} avl_t;

struct nodo {
	nodo_t* izq;
	nodo_t* der;
	char* clave;
	void* dato;
	size_t altura;
};

typedef struct avl_iter {
	pila_t* pila;
} avl_iter_t;

/* -------------------------------- 		DEFINICIONES		-------------------------------- */

nodo_t* buscar_nodo(nodo_t* nodo, avl_comparar_clave_t f_comp, const char* clave);

void nodo_in_order(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra, bool* estado);

void borrar_dos_hijos(avl_t* arbol, nodo_t* nodo);

void borrar_simple_hijos(avl_t* arbol, nodo_t* padre, nodo_t* nodo);

size_t actualizar_altura(avl_t* arbol, nodo_t* nodo, nodo_t* padre, nodo_t* buscado);

size_t max(size_t ent1, size_t ent2);

long mod(long ent1);

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
	nodo->altura = 0;
	nodo->clave = str_clave;
	nodo->dato = dato;

	return nodo;
}

void nodo_destruir(nodo_t* nodo, avl_destruir_dato_t f_dest, bool recursiva) {
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

nodo_t* nodo_hijo(const nodo_t* nodo, avl_comparar_clave_t f_comp, const char* clave) {
	/*	Dado un nodo, una funcion de comparacion y una clave del hijo, devuelve un puntero al
	 *	hijo, en caso de que no tenga o tenga pero la clave sea diferente devuelve null
	 */
	if (nodo->der && f_comp(nodo->der->clave, clave) == 0) return nodo->der;
	else if (nodo->izq && f_comp(nodo->izq->clave, clave) == 0) return nodo->izq;
	return NULL;
}

size_t nodo_altura(nodo_t* nodo) {
	/*	Devuelve la altura del nodo, siempre y cuando las alturas de los hijos esten bien.
	 */
	size_t altura;
	if (!nodo) altura = 0;
	else if (!nodo->der && !nodo->izq) altura = 1;
	else if (!nodo->der && nodo->izq) altura = nodo->izq->altura + 1;
	else if (!nodo->izq && nodo->der) altura = nodo->der->altura + 1;
	else altura = max(nodo->der->altura, nodo->izq->altura) + 1;
	return altura;
}

/* -------------------------------- 	PRIMITIVAS DE avl 	-------------------------------- */


avl_t* avl_crear(avl_comparar_clave_t cmp, avl_destruir_dato_t destruir_dato) {
	avl_t* avl = malloc(sizeof(avl_t));
	if (!avl) return NULL;

	avl->raiz = NULL;
	avl->f_comp = cmp;
	avl->f_dest = destruir_dato;
	avl->cant = 0;

	return avl;
}

bool avl_guardar(avl_t* arbol, const char* clave, void* dato) {
	nodo_t* actual = arbol->raiz;
	int dir = 0;

	while (actual){
		dir = arbol->f_comp(clave, actual->clave);

		if (dir == 0){
			if (arbol->f_dest) arbol->f_dest(actual->dato);
			actual->dato = dato;
			return true;
		}
		else if (dir > 0 && actual->der) actual = actual->der; 
		else if (dir < 0 && actual->izq) actual = actual->izq;
		else break;
	}

	nodo_t* n_nodo = nodo_crear(clave, dato);
	if (!n_nodo) return false;

	if (dir > 0) actual->der = n_nodo;
	else if (dir < 0) actual->izq = n_nodo;
	else arbol->raiz = n_nodo;

	arbol->raiz->altura = actualizar_altura(arbol, arbol->raiz, NULL, n_nodo);

	++arbol->cant;
	return true;
}

void* avl_borrar(avl_t *arbol, const char *clave) {
	nodo_t* actual = arbol->raiz;
	nodo_t* padre = NULL;

	while (actual){
		int dir = arbol->f_comp(clave, actual->clave);

		if (dir == 0) {
			void* dato = actual->dato;
			if (actual->izq && actual->der) borrar_dos_hijos(arbol, actual);
			else  borrar_simple_hijos(arbol, padre, actual);
			--arbol->cant;

			if (padre) arbol->raiz->altura = actualizar_altura(arbol, arbol->raiz, NULL, padre);

			return dato;
		}
		padre = actual;
		if (dir > 0) actual = actual->der;
		else actual = actual->izq;
	}
	return NULL;
}

void* avl_obtener(const avl_t *arbol, const char *clave) {
	if (!arbol->raiz) return NULL;
	nodo_t*	nodo = buscar_nodo(arbol->raiz, arbol->f_comp, clave);
	if (!nodo) return NULL;
	return nodo->dato;
}

bool avl_pertenece(const avl_t *arbol, const char *clave) {
	if (!arbol->raiz) return NULL;
	return buscar_nodo(arbol->raiz, arbol->f_comp, clave) != NULL;
}

size_t avl_cantidad(avl_t *arbol) {
	return arbol->cant;
}

void avl_destruir(avl_t *arbol) {
	nodo_destruir(arbol->raiz, arbol->f_dest, true);
	free(arbol);
}

void avl_in_order(avl_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	if (!arbol->raiz) return;
	bool continuar = true;
	nodo_in_order(arbol->raiz, visitar, extra, &continuar);
}

/* -------------------------------- 	AUXILIARES DE avl 	-------------------------------- */

void nodo_in_order(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra, bool* estado) {
	/* 	Recorre el arbol en in order le aplica la funcion visitar, en caso de 
	 *	que visitar devuelva false se deja de ejecutar. El estado incialmente
	 *	tiene que ser true para que itere.
	 */
	if (!nodo || !*estado) return;
	nodo_in_order(nodo->izq, visitar, extra, estado);
	if (*estado) *estado = visitar(nodo->clave, nodo->dato, extra);
	nodo_in_order(nodo->der, visitar, extra, estado);
}

void borrar_dos_hijos(avl_t* arbol, nodo_t* nodo) {
	/*	Dado un arbol y un nodo con 2 hijos borra el nodo buscando un remplazante.
	 *	no destruye el dato.
	 */
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

	free(nodo->clave);
	nodo->clave = nodo_cambio->clave;
	nodo->dato = nodo_cambio->dato;
	free(nodo_cambio);
}

void borrar_simple_hijos(avl_t* arbol, nodo_t* padre, nodo_t* nodo) {
	/*	Dado un arbol, un padre y un nodo a borrar con 0 o 1 hijos borra el nodo y le 
	 * 	asigna el sigguiente correspondiente al nodo. no destruye el dato.
	 */
	nodo_t* hijo_nodo = NULL;
	if (nodo->der) hijo_nodo = nodo->der;
	else if (nodo->izq) hijo_nodo = nodo->izq;

	if (!padre) arbol->raiz = hijo_nodo;
	else {
		int dir = arbol->f_comp(padre->clave, nodo->clave);		
		if (dir > 0) padre->izq = hijo_nodo;
		else if (dir < 0) padre->der = hijo_nodo;
	}
	nodo_destruir(nodo, NULL, false);
}

nodo_t* buscar_nodo(nodo_t* nodo, avl_comparar_clave_t f_comp, const char* clave) {
	/*	Busca la clave en el arbol y devuelve un puntero al nodo que contiene
	 *	dicha clave en caso de que no este la clave devuelve null.
	 */
	if (!nodo) return NULL;
	int dir = f_comp(nodo->clave, clave) ;
	if (dir > 0) return buscar_nodo(nodo->izq, f_comp, clave);
	else if (dir < 0) return buscar_nodo(nodo->der, f_comp, clave);
	return nodo;
}

void rot_der_der(avl_t* arbol, nodo_t* padre, nodo_t* z, nodo_t* y, nodo_t* x) {
	/*	Aplioca una rotacion derecha derecha y actualiza las alturas.
	 */
	nodo_t* aux = y->izq;
	y->izq = z;
	z->der = aux;

	if (padre && padre->der == z) padre->der = y;
	else if (padre && padre->izq == z) padre->izq = y;
	else arbol->raiz = y;

	z->altura = nodo_altura(z);
	x->altura = nodo_altura(x);
	y->altura = nodo_altura(y);
}

void rot_izq_izq(avl_t* arbol, nodo_t* padre, nodo_t* z, nodo_t* y, nodo_t* x) {
	/*	Aplioca una rotacion izquierda izquierda y actualiza las alturas.
	 */
	nodo_t* aux = y->der;
	y->der = z;
	z->izq = aux;

	if (padre && padre->der == z) padre->der = y;
	else if (padre && padre->izq == z) padre->izq = y;
	else arbol->raiz = y;

	z->altura = nodo_altura(z);
	x->altura = nodo_altura(x);
	y->altura = nodo_altura(y);
}

void rot_der_izq(avl_t* arbol, nodo_t* padre, nodo_t* z, nodo_t* y, nodo_t* x) {
	/*	Aplica una rotacion derecha izquierda, luego derecha derecha y actualiza las alturas.
	 */
	nodo_t* aux = x->der;
	x->der = y;
	z->der = x;
	y->izq = aux;

	y->altura = nodo_altura(y);
	x->altura = nodo_altura(x);
	z->altura = nodo_altura(z);
	
	rot_der_der(arbol, padre, z, x, y);
}

void rot_izq_der(avl_t* arbol, nodo_t* padre, nodo_t* z, nodo_t* y, nodo_t* x) {
	/*	Aplica una rotacion izquierda derecha, luego izquierda izquierda y actualiza las alturas.
	 */
	nodo_t* aux = x->izq;
	x->izq = y;
	z->izq = x;
	y->der = aux;

	y->altura = nodo_altura(y);
	x->altura = nodo_altura(x);
	z->altura = nodo_altura(z);

	rot_izq_izq(arbol, padre, z, x, y);
}

void avl_rotar(avl_t* arbol, nodo_t* nodo, nodo_t* padre) {
	/*	Dada un arbol un nodo y el padre, aplica la rotacion correspondiente y
	 *	actualiza las alturas.
	 */
	nodo_t* z = nodo;
	nodo_t* y;
	nodo_t* x;
	bool pri_der = false;
	bool seg_der = false;

	if (nodo_altura(z->der) > nodo_altura(z->izq)) {
		y = z->der;
		pri_der = true;
	} else y = z->izq;

	if (nodo_altura(y->der) > nodo_altura(y->izq)) {
		x = y->der;
		seg_der = true;
	} else x = y->izq;

	if (pri_der && seg_der) rot_der_der(arbol, padre, z, y, x);
	else if (!pri_der && !seg_der) rot_izq_izq(arbol, padre, z, y, x);
	else if (pri_der && !seg_der) rot_der_izq(arbol, padre, z, y, x);
	else rot_izq_der(arbol, padre, z, y, x);
}

size_t actualizar_altura(avl_t* arbol, nodo_t* nodo, nodo_t* padre, nodo_t* buscado) {
	/*	Dado un arbol, el nodo incial con su padre y el nodo buscado como direccion
	 *	actualiza sus alturas hasta llegar al nodo buscado, si la altura esta mal,
	 *	aplica la rotacion correspondiente.
	 *	devuelve la altura del nodo, pasado como parametro.
	 */
	if (!nodo) return 0;

	int dir = arbol->f_comp(nodo->clave, buscado->clave);
	if (dir > 0) nodo->altura = max(actualizar_altura(arbol, nodo->izq, nodo, buscado), nodo_altura(nodo->der)) + 1;
	else if (dir < 0) nodo->altura = max(actualizar_altura(arbol, nodo->der, nodo, buscado), nodo_altura(nodo->izq)) + 1;
	else nodo->altura = nodo_altura(nodo);

	if (mod(nodo_altura(nodo->der) - nodo_altura(nodo->izq)) > 1) avl_rotar(arbol, nodo, padre);

	return nodo->altura;
}

size_t max(size_t ent1, size_t ent2) {
	/*	Dado don numero devuelve el maximo entre los 2, si son iguales devuelve ent2.
	 */
	if (ent1 > ent2) return ent1;
	return ent2;
}

long mod(long ent1) {
	/*	Dado un numero devuelve su modulo. 
	 */
	if (ent1 < 0) return (ent1 * (-1));
	return ent1;
}

/* -------------------------------- 	PRIMITIVAS ITERADOR 	-------------------------------- */

avl_iter_t* avl_iter_in_crear(const avl_t* arbol) {
	avl_iter_t* iter = malloc(sizeof(avl_iter_t));
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

bool avl_iter_in_avanzar(avl_iter_t *iter) {
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

bool avl_iter_in_al_final(const avl_iter_t* iter) {
	return pila_ver_tope(iter->pila) == NULL;
}

const char* avl_iter_in_ver_actual(const avl_iter_t* iter) {
	if (avl_iter_in_al_final(iter)) return NULL;
	return ((nodo_t*)pila_ver_tope(iter->pila))->clave;
}

void avl_iter_in_destruir(avl_iter_t* iter) {
	pila_destruir(iter->pila);
	free(iter);
}