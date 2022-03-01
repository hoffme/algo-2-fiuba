#define _POSIX_C_SOURCE 200809L

#include "lista.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* -------------------------------- 		DEFINICIONES		-------------------------------- */

typedef struct nodo nodo_t;

typedef int (*avl_comparar_clave_t) (const char *, const char *);

typedef void (*avl_destruir_dato_t) (void *);

/* -------------------------------- 		ESTRUCTURAS			-------------------------------- */

struct nodo {
	nodo_t* izq;
	nodo_t* der;
	char* clave;
	void* dato;
	size_t altura;
}

typedef struct avl {
	nodo_t* raiz;
	avl_comparar_clave_t f_comp;
	avl_destruir_dato_t f_dest;
	size_t cant;
} avl_t;

/* -------------------------------- 		DEFINICIONES		-------------------------------- */

bool _avl_guardar(avl_t* arbol, nodo_t* nodo, nodo_t* padre, int dir_nodo,const char* clave, void* dato);

void avl_rotar(avl_t* arbol, nodo_t* nodo, nodo_t* padre);

void* _avl_borrar(avl_t* arbol, nodo_t* nodo, nodo_t* padre, int dir_nodo, const char* clave);

/* -------------------------------- 			AUX 			-------------------------------- */

long max(long a, long b) {
	/*	Dado dos numeros devuelve el maximo entre los 2, si son iguales devuelve el segundo.
	 */
	if (a > b) return a;
	return b;
}

long mod(long a) {
	/*	Dado un numero devuelve su modulo. 
	 */
	if (a < 0) return (a * (-1));
	return a;
}

/* -------------------------------- 	PRIMITIVAS DE NODO 		-------------------------------- */

nodo_t* nodo_crear(const char* clave, void* dato) {
	/*	Dado una clave como parametro y un puntero a un dato, crea un nodo, con los hijos nulos
	 * 	y le asigna la clave y el dato y devuleve un puntero, si no se pudo crear devuelve null.
	 */
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;

	char* copia_clave = strdup(clave);
	if (!copia_clave) {
		free(nodo);
		return NULL;	
	}

	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->altura = 1;
	nodo->clave = copia_clave;
	nodo->dato = dato;

	return nodo;
}

void nodo_destruir(nodo_t* nodo, avl_destruir_dato_t f_dest, bool recursiva) {
	/*	Dado un nodo, una funcion de destruccion y un booleano destruye el nodo, si el booleano 
	 *	es true destruye a los hijos recursivamente, si la funcion de destruccion no es nula
	 *	destruye al dato, la clave y el nodo.
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

size_t nodo_altura(nodo_t* nodo) {
	/*	Devuelve la altura del nodo.
	 *	Pre: las alturas de los hijos estan bien o no existen los hijos.
	 */
	size_t altura;
	if (!nodo) altura = 0;
	else if (!nodo->der && !nodo->izq) altura = 1;
	else if (!nodo->der && nodo->izq) altura = nodo->izq->altura + 1;
	else if (!nodo->izq && nodo->der) altura = nodo->der->altura + 1;
	else altura = max(nodo->der->altura, nodo->izq->altura) + 1;
	return altura;
}

nodo_t* buscar_nodo(nodo_t* nodo, avl_comparar_clave_t f_comp, const char* clave) {
	/*	Busca la clave en el arbol y devuelve un puntero al nodo que contiene
	 *	dicha clave en caso de que no este la clave devuelve null.
	 */
	if (!nodo) return NULL;
	int dir = f_comp(nodo->clave, clave);
	if (dir > 0) return buscar_nodo(nodo->izq, f_comp, clave);
	else if (dir < 0) return buscar_nodo(nodo->der, f_comp, clave);
	return nodo;
}

/* -------------------------------- 	PRIMITIVAS DE AVL 		-------------------------------- */

avl_t* avl_crear(avl_comparar_clave_t cmp, avl_destruir_dato_t f_dest) {
	avl_t* avl = malloc(sizeof(avl_t));
	if (!avl) return NULL;

	avl->raiz = NULL;
	avl->f_comp = cmp;
	avl->f_dest = f_dest;
	avl->cant = 0;

	return avl;
}

void* avl_obtener(const avl_t *arbol, const char *clave) {
	nodo_t*	nodo = buscar_nodo(arbol->raiz, arbol->f_comp, clave);
	if (!nodo) return NULL;
	return nodo->dato;
}

bool avl_pertenece(const avl_t *arbol, const char *clave) {
	return buscar_nodo(arbol->raiz, arbol->f_comp, clave) != NULL;
}

size_t avl_cantidad(avl_t *arbol) {
	return arbol->cant;
}

void avl_destruir(avl_t *arbol) {
	nodo_destruir(arbol->raiz, arbol->f_dest, true);
	free(arbol);
}

bool avl_guardar(avl_t* arbol, const char* clave, void* dato) {
	return _avl_guardar(arbol, arbol->raiz, NULL, 0, clave, dato);
}

void* avl_borrar(avl_t *arbol, const char* clave) {
	return _avl_borrar(arbol, arbol->raiz, NULL, 0, clave);
}

/* -------------------------------- 	AUXILIARES DE AVL 		-------------------------------- */

void borrar_dos_hijos(abb_t* arbol, nodo_t* nodo) {
	nodo_t* nodo_cambio = nodo->der;

	while (nodo_cambio->izq) nodo_cambio = nodo_cambio->izq;

	char* aux = strdup(nodo_cambio->clave);
	nodo->dato = _avl_borrar(arbol, nodo->der, nodo, -1, nodo_cambio->clave);

	free(nodo->clave);
	nodo->clave = aux;
}

void borrar_simple_hijos(abb_t* arbol, nodo_t* padre, nodo_t* nodo, int dir) {
	nodo_t* hijo_nodo = NULL;
	if (nodo->der) hijo_nodo = nodo->der;
	else if (nodo->izq) hijo_nodo = nodo->izq;

	if (!padre) arbol->raiz = hijo_nodo;
	else {
		if (dir > 0) padre->izq = hijo_nodo;
		else if (dir < 0) padre->der = hijo_nodo;
	}
	nodo_destruir(nodo, NULL, false);
	--arbol->cant;
}

bool _avl_guardar(avl_t* arbol, nodo_t* nodo, nodo_t* padre, int dir_nodo, const char* clave, void* dato) {
	if (!nodo) {
		nodo_t* n_nodo = nodo_crear(clave, dato);
		if (!n_nodo) return false;

		if (!padre) arbol->raiz = n_nodo;
		else if (dir_nodo > 0) padre->izq = n_nodo;
		else padre->der = n_nodo;

		++arbol->cant;
		return true;
	}

	bool ok = true;
	int dir_sig = arbol->f_comp(nodo->clave, clave);

	if (dir_sig > 0) ok = _avl_guardar(arbol, nodo->izq, nodo, dir_sig, clave, dato);
	else if (dir_sig < 0) ok = _avl_guardar(arbol, nodo->der, nodo, dir_sig, clave, dato);
	else {
		if (arbol->f_dest) arbol->f_dest(nodo->dato);
		nodo->dato = dato;
		return true;
	}

	nodo->altura = nodo_altura(nodo);
	if (mod(nodo_altura(nodo->der) - nodo_altura(nodo->izq)) > 1) avl_rotar(arbol, nodo, padre);

	return ok;
}

void* _avl_borrar(avl_t* arbol, nodo_t* nodo, nodo_t* padre, int dir_nodo, const char* clave) {
	if (!nodo) return NULL;

	void* dato = NULL;
	int dir_hijo = arbol->f_comp(nodo->clave, clave);

	if (dir_hijo > 0) dato = _avl_borrar(arbol, nodo->izq, nodo, dir_hijo, clave);
	else if (dir_hijo < 0) dato = _avl_borrar(arbol, nodo->der, nodo, dir_hijo, clave);
	else {
		dato = nodo->dato;
		if (nodo->izq && nodo->der) borrar_dos_hijos(arbol, nodo);
		else borrar_simple_hijos(arbol, padre, nodo, dir);
		return dato;
	}

	if (mod(nodo_altura(nodo->der) - nodo_altura(nodo->izq)) > 1) avl_rotar(arbol, nodo, padre);

	return dato;
}

/* -------------------------------- 		ROTAR DE AVL 		-------------------------------- */

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

/* -------------------------------- 	ITERADOR INTERNO 		-------------------------------- */

void nodo_pre_order(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra, bool* estado) {
	/* 	Recorre el arbol en pre order le aplica la funcion visitar, en caso de 
	 *	que visitar devuelva false se deja de ejecutar. El estado incialmente
	 *	tiene que ser true para que itere.
	 */
	if (!nodo || !*estado) return;
	if (*estado) *estado = visitar(nodo->clave, nodo->dato, extra);
	nodo_pre_order(nodo->izq, visitar, extra, estado);
	nodo_pre_order(nodo->der, visitar, extra, estado);
}

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

void nodo_post_order(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra, bool* estado) {
	/* 	Recorre el arbol en post order le aplica la funcion visitar, en caso de 
	 *	que visitar devuelva false se deja de ejecutar. El estado incialmente
	 *	tiene que ser true para que itere.
	 */
	if (!nodo || !*estado) return;
	nodo_post_order(nodo->izq, visitar, extra, estado);
	nodo_post_order(nodo->der, visitar, extra, estado);
	if (*estado) *estado = visitar(nodo->clave, nodo->dato, extra);
}

void nodo_por_niveles(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra, bool* estado) {
	/* 	Recorre el arbol por niveles y le aplica la funcion visitar, en caso de 
	 *	que visitar devuelva false se deja de ejecutar. El estado incialmente
	 *	tiene que ser true para que itere.
	 */
	if (!nodo) return;

	lista_t* nivel_nodo = lista_crear();
	if (!nivel_nodo) return;

	lista_insertar_ultimo(nivel_nodo, nodo);

	nodo_t* actual = NULL;
	while (!lista_esta_vacia(nivel_nodo)) {

		actual = (nodo_t*)lista_borrar_primero(nivel_nodo);

		if (actual->der && !lista_insertar_ultimo(nivel_nodo, actual->der)) break;
		if (actual->izq && !lista_insertar_ultimo(nivel_nodo, actual->izq)) break;

		if (!visitar(actual->clave, actual->dato, extra)) break;
	}

	lista_destruir(nivel_nodo, NULL);
}

void avl_iter_interno(avl_t* arbol, int recorrido, bool visitar(const char*, void*, void*), void* extra) {
	if (!arbol->raiz || recorrido < 0 || recorrido > 3) return;
	bool continuar = true;
	if (recorrido == 0) nodo_pre_order(arbol->raiz, visitar, extra, &continuar);
	else if (recorrido == 1) nodo_in_order(arbol->raiz, visitar, extra, &continuar);
	else if (recorrido == 2) nodo_post_order(arbol->raiz, visitar, extra, &continuar);
	else if (recorrido == 3) nodo_por_niveles(arbol->raiz, visitar, extra, &continuar);
}

/* -------------------------------- ESTRUCTURA ITERADOR EXTERNO	-------------------------------- */

typedef struct avl_iter {
	lista_t* lista;
	int recorrido;
} avl_iter_t;

/* -------------------------------- 	ITERADOR EXTERNO 		-------------------------------- */

void avl_iter_crear_pre(avl_t* arbol, lista_t* lista) {
	lista_insertar_primero(arbol->raiz);
}

void avl_iter_crear_in(avl_t* arbol, lista_t* lista) {
	nodo_t* act = arbol->raiz;
	while (act) {
		lista_insertar_primero(lista, act);
		act = act->izq;
	}
}

void avl_iter_crear_post(avl_t* arbol, lista_t* lista) {
	nodo_t* act = arbol->raiz;
	while (act) {
		lista_insertar_primero(lista, act);
		act = act->izq;
	}
}

void avl_iter_crear_niveles(avl_t* arbol, lista_t* lista) {
	lista_insertar_ultimo(arbol->raiz);
}

avl_iter_t* avl_iter_crear(const avl_t* arbol, int recorrido) {
	if (recorrido < 0 || recorrido > 3) return NULL;

	avl_iter_t* iter = malloc(sizeof(avl_iter_t));
	if (!iter) return NULL;

	lista_t* lista = lista_crear();
	if (!lista) {
		free(iter);
		return NULL;
	}

	iter->lista = lista;
	iter->recorrido = recorrido;

	if (recorrido == 0) avl_iter_crear_pre(arbol, lista);
	if (recorrido == 1) avl_iter_crear_in(arbol, lista);
	if (recorrido == 2) avl_iter_crear_post(arbol, lista);
	if (recorrido == 3) avl_iter_crear_niveles(arbol, lista);

	return iter;
}

bool avl_iter_avanzar_pre(avl_iter_t* iter) {
}

bool avl_iter_avanzar_in(avl_iter_t* iter) {
}

bool avl_iter_avanzar_post(avl_iter_t* iter) {
}

bool avl_iter_avanzar_niveles(avl_iter_t* iter) {
}

bool avl_iter_avanzar(avl_iter_t *iter) {
	if (iter->recorrido == 0) return avl_iter_avanzar_pre(iter);
	if (iter->recorrido == 1) return avl_iter_avanzar_in(iter);
	if (iter->recorrido == 2) return avl_iter_avanzar_post(iter);
	if (iter->recorrido == 3) return avl_iter_avanzar_niveles(iter);
}

bool avl_iter_al_final(const avl_iter_t* iter) {
	return lista_esta_vacia(iter->lista);
}

const char* avl_iter_ver_actual(const avl_iter_t* iter) {
	if (avl_iter_al_final(iter)) return NULL;
	return ((nodo_t*)lista_ver_primero(iter->lista))->clave;
}

void avl_iter_destruir(avl_iter_t* iter) {
	lista_destruir(iter->lista, NULL);
	free(iter);
}