#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* -------------------------------- 		DEFINICIONES		-------------------------------- */

typedef struct nodo nodo_t;
typedef int (*abb_comparar_clave_t) (const void*, const void*, void*);
typedef void (*abb_destruir_dato_t) (void*);
typedef void (*abb_destruir_clave_t) (void*);
typedef void* (*abb_copiar_clave_t) (const void*);
typedef bool (*abb_visitar_iter_t) (const void*, void*, void*);

/* -------------------------------- 		ESTRUCTURAS		-------------------------------- */

typedef struct abb {
	nodo_t* raiz;
	abb_comparar_clave_t f_comp;
	abb_destruir_dato_t f_dest_dato;
	abb_destruir_clave_t f_dest_clave;
	abb_copiar_clave_t f_copia_clave;
	size_t cant;
} abb_t;

struct nodo {
	nodo_t* izq;
	nodo_t* der;
	void* clave;
	void* dato;
};

/* -------------------------------- 		DEFINICIONES		-------------------------------- */

void borrar_dos_hijos(abb_t* arbol, nodo_t* nodo);

void borrar_simple_hijos(abb_t* arbol, nodo_t* padre, nodo_t* nodo, void* cmp_extra);

/* -------------------------------- 	PRIMITIVAS DE NODO 	-------------------------------- */

nodo_t* nodo_crear_abb(const void* clave, void* dato, abb_copiar_clave_t f_copia_clave) {
	/*	Dado una clave como parametro y un puntero a un dato, crea un nodo, con los hijos nulos
	 * 	y le asigna la clave y el dato y devuleve un puntero, si no se pudo crear devuelve null
	 */
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;

	void* copia_clave = f_copia_clave(clave);
	if (!copia_clave) {
		free(nodo);
		return NULL;	
	}
	
	nodo->clave = copia_clave;
	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->dato = dato;

	return nodo;
}

nodo_t* nodo_hijo(const nodo_t* nodo, abb_comparar_clave_t f_comp, const void* clave, void* cmp_extra) {
	/*	Dado un nodo, una funcion de comparacion y una clave del hijo, devuelve un puntero al
	 *	hijo, en caso de que no tenga o tenga pero la clave sea diferente devuelve null
	 */
	if (nodo->der && f_comp(nodo->der->clave, clave, cmp_extra) == 0) return nodo->der;
	else if (nodo->izq && f_comp(nodo->izq->clave, clave, cmp_extra) == 0) return nodo->izq;
	return NULL;
}

void nodo_destruir(nodo_t* nodo, abb_destruir_dato_t f_dest, abb_destruir_clave_t f_dest_clave, bool recursiva) {
	/*	Dado un nodo, una funcion de destruccion y un booleano de recusividad, si el booleano 
	 *	es true destruye a los hijos recursivamente, destruye al dato y libera a la clave y al nodo
	 */
	if (!nodo) return;
	if (recursiva) {
		nodo_destruir(nodo->izq, f_dest, f_dest_clave, true);
		nodo_destruir(nodo->der, f_dest, f_dest_clave, true);
	}
	if (f_dest) f_dest(nodo->dato);
	f_dest_clave(nodo->clave);
	free(nodo);
}

nodo_t* buscar_nodo(nodo_t* nodo, abb_comparar_clave_t f_comp, const void* clave, void* cmp_extra) {
	/*	Busca la clave en el arbol y devuelve un puntero al nodo que contiene
	 *	dicha clave en caso de que no este la clave devuelve null.
	 */
	if (!nodo) return NULL;
	int dir = f_comp(nodo->clave, clave, cmp_extra) ;
	if (dir > 0) return buscar_nodo(nodo->izq, f_comp, clave, cmp_extra);
	else if (dir < 0) return buscar_nodo(nodo->der, f_comp, clave, cmp_extra);
	return nodo;
}

void _nodo_in_order(nodo_t* nodo, abb_visitar_iter_t f_visitar, void* extra, bool* estado) {
	/* 	Recorre el arbol en in order sobre un rango y le aplica la funcion visitar,  
	 *	en caso de que visitar devuelva false se deja de ejecutar. El estado incialmente
	 *	tiene que ser true para que itere.
	 */
	if (!nodo || !*estado) return;
	_nodo_in_order(nodo->izq, f_visitar, extra, estado);
	if (*estado) *estado = f_visitar(nodo->clave, nodo->dato, extra);
	_nodo_in_order(nodo->der, f_visitar, extra, estado);
}

void _nodo_in_order_rango(abb_comparar_clave_t f_comp, nodo_t* nodo, const void* desde, const void* hasta, abb_visitar_iter_t f_visitar, void* v_extra, bool* estado, void* cmp_extra) {
	/* 	Recorre el arbol en in order sobre un rango y le aplica la funcion visitar,  
	 *	en caso de que visitar devuelva false se deja de ejecutar. El estado incialmente
	 *	tiene que ser true para que itere.
	 */
	if (!nodo || !*estado) return;

	bool izq = f_comp(desde, nodo->clave, cmp_extra) <= 0;
	bool der = f_comp(hasta, nodo->clave, cmp_extra) >= 0;

	if (izq) _nodo_in_order_rango(f_comp, nodo->izq, desde, hasta, f_visitar, v_extra, estado, cmp_extra);
	if (*estado && izq && der) *estado = f_visitar(nodo->clave, nodo->dato, v_extra);
	if (der) _nodo_in_order_rango(f_comp, nodo->der, desde, hasta, f_visitar, v_extra, estado, cmp_extra);
}

/* -------------------------------- 	PRIMITIVAS DE ABB 	-------------------------------- */


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t f_dest_dato, abb_destruir_clave_t f_dest_clave, abb_copiar_clave_t f_copia_clave) {
	abb_t* abb = malloc(sizeof(abb_t));
	if (!abb) return NULL;

	abb->raiz = NULL;
	abb->f_comp = cmp;
	abb->f_dest_dato = f_dest_dato;
	abb->f_dest_clave = f_dest_clave;
	abb->f_copia_clave = f_copia_clave;
	abb->cant = 0;

	return abb;
}

bool abb_guardar(abb_t* arbol, const void* clave, void* dato, void* cmp_extra) {
	nodo_t* actual = arbol->raiz;
	int dir = 0;

	while (actual){
		dir = arbol->f_comp(clave, actual->clave, cmp_extra);

		if (dir == 0){
			void* copia_clave = arbol->f_copia_clave(clave);
			if (!copia_clave) return false;

			if (arbol->f_dest_dato) arbol->f_dest_dato(actual->dato);
			arbol->f_dest_clave(actual->clave);

			actual->clave = copia_clave;
			actual->dato = dato;
			return true;
		}
		else if (dir > 0 && actual->der) actual = actual->der; 
		else if (dir < 0 && actual->izq) actual = actual->izq;
		else break;
	}

	nodo_t* n_nodo = nodo_crear_abb(clave, dato, arbol->f_copia_clave);
	if (!n_nodo) return false;

	if (dir > 0) actual->der = n_nodo;
	else if (dir < 0) actual->izq = n_nodo;
	else arbol->raiz = n_nodo;

	++arbol->cant;
	return true;
}

void* abb_borrar(abb_t *arbol, const void* clave, void* cmp_extra) {
	nodo_t* actual = arbol->raiz;
	nodo_t* padre = NULL;

	while (actual){
		int dir = arbol->f_comp(clave, actual->clave, cmp_extra);

		if (dir == 0) {
			void* dato = actual->dato;
			if (actual->izq && actual->der) borrar_dos_hijos(arbol, actual);
			else  borrar_simple_hijos(arbol, padre, actual, cmp_extra);
			--arbol->cant;
			return dato;
		}
		padre = actual;
		if (dir > 0) actual = actual->der;
		else actual = actual->izq;
	}
	return NULL;
}

void* abb_obtener(const abb_t *arbol, const void* clave, void* cmp_extra) {
	if (!arbol->raiz) return NULL;
	nodo_t*	nodo = buscar_nodo(arbol->raiz, arbol->f_comp, clave, cmp_extra);
	if (!nodo) return NULL;
	return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const void* clave, void* cmp_extra) {
	return buscar_nodo(arbol->raiz, arbol->f_comp, clave, cmp_extra) != NULL;
}

size_t abb_cantidad(abb_t *arbol) {
	return arbol->cant;
}

void abb_destruir(abb_t *arbol) {
	nodo_destruir(arbol->raiz, arbol->f_dest_dato, arbol->f_dest_clave, true);
	free(arbol);
}

void abb_in_order(abb_t *arbol, abb_visitar_iter_t f_visitar, void *extra) {
	if (!arbol->raiz) return;
	bool continuar = true;
	_nodo_in_order(arbol->raiz, f_visitar, extra, &continuar);
}

void abb_in_order_rango(abb_t *arbol, const void* desde, const void* hasta, abb_visitar_iter_t f_visitar, void* extra, void* cmp_extra) {
	if (!arbol->raiz) return;
	bool continuar = true;
	_nodo_in_order_rango(arbol->f_comp, arbol->raiz, desde, hasta, f_visitar, extra, &continuar, cmp_extra);
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

	arbol->f_dest_clave(nodo->clave);
	nodo->clave = nodo_cambio->clave;
	nodo->dato = nodo_cambio->dato;
	free(nodo_cambio);
}

void borrar_simple_hijos(abb_t* arbol, nodo_t* padre, nodo_t* nodo, void* cmp_extra) {
	nodo_t* hijo_nodo = NULL;
	if (nodo->der) hijo_nodo = nodo->der;
	else if (nodo->izq) hijo_nodo = nodo->izq;

	if (!padre) arbol->raiz = hijo_nodo;
	else {
		int dir = arbol->f_comp(padre->clave, nodo->clave, cmp_extra);		
		if (dir > 0) padre->izq = hijo_nodo;
		else if (dir < 0) padre->der = hijo_nodo;
	}
	nodo_destruir(nodo, NULL, arbol->f_dest_clave, false);
}