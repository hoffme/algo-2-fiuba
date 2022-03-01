#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/*-----------    ESTRUCTURA NODO    -----------*/

//	estructura de nodo solamente con un puntero a un nodo siguiente,
//	y un puntero void* a un dato.
typedef struct nodo {
	struct nodo* sig;
	void* dato;
} nodo_t;

/*-----------    PRIMITIVAS NODO    -----------*/

//Accion:	crea un nodo.
//Pre:		-
//Post:		devuelve un nuevo nodo con siguiente nulo.
nodo_t* nodo_crear(void){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;
	nodo->sig = NULL;
	return nodo;
}

/*-----------    ESTRUCTURA LISTA    -----------*/

typedef struct lista {
	struct nodo* prim;
	struct nodo* ulti;
	size_t largo;
} lista_t;


/*-----------    PRIMITIVAS LISTA    -----------*/

lista_t* lista_crear(void) {
	lista_t* lista = malloc(sizeof(lista_t));
	if (!lista) return NULL;
	lista->prim = NULL;
	lista->ulti = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
	return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
	nodo_t* nuevo = nodo_crear();
	if (!nuevo) return false;
	nuevo->dato = dato;
	nuevo->sig = lista->prim;

	lista->prim = nuevo;
	if (lista_esta_vacia(lista)) lista->ulti = nuevo;

	lista->largo++;

	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
	nodo_t* nuevo = nodo_crear();
	if (!nuevo) return false;
	nuevo->dato = dato;

	if (lista_esta_vacia(lista)) lista->prim = nuevo;
	else lista->ulti->sig = nuevo;

	lista->ulti = nuevo;

	lista->largo++;

	return true;
}

void* lista_borrar_primero(lista_t *lista) {
	if (lista_esta_vacia(lista)) return NULL;

	void* dato = lista->prim->dato;
	nodo_t* n_prim = lista->prim->sig;
	free(lista->prim);
	
	lista->largo--;
	if (lista_esta_vacia(lista)) lista->ulti = NULL;
	lista->prim = n_prim;
	
	return dato;
}

void *lista_ver_primero(const lista_t *lista) {
	if (lista_esta_vacia(lista)) return NULL;
	return lista->prim->dato;
}

void* lista_ver_ultimo(const lista_t* lista) {
	if (lista_esta_vacia(lista)) return NULL;
	return lista->ulti->dato;
}

size_t lista_largo(const lista_t *lista) {
	return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)) {
	while (!lista_esta_vacia(lista)) {
		void* dato = lista_borrar_primero(lista);
		if (destruir_dato) destruir_dato(dato);
	}
	free(lista);
}

/*-----------    ITERADOR LISTA INTERNO    -----------*/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
	nodo_t* nodo = lista->prim;
	while (nodo && visitar(nodo->dato,extra)) {
		nodo = nodo->sig;
	}
}

/*-----------    ESTRUCTURA ITERADOR LISTA EXTERNO    -----------*/

typedef struct lista_iter {
	lista_t* lista;
	struct nodo* act;
	struct nodo* ant;
} lista_iter_t;

/*-----------    PRIMITIVAS ITERADOR LISTA EXTERNO    -----------*/

lista_iter_t* lista_iter_crear(lista_t *lista) {
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (!iter) return NULL;
	iter->lista = lista;
	iter->act = lista->prim;
	iter->ant = NULL;
	return iter;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
	return iter->act == NULL;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
	if (lista_iter_al_final(iter)) return false;
	iter->ant = iter->act;
	iter->act = iter->act->sig;
	return true;
}

void* lista_iter_ver_actual(const lista_iter_t *iter) {
	if (lista_iter_al_final(iter)) return NULL;
	return iter->act->dato;
}

void lista_iter_destruir(lista_iter_t *iter) {
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
	nodo_t* n_act = nodo_crear();
	if (!n_act) return false;
	n_act->dato = dato;
	n_act->sig = iter->act;

	if (!iter->ant)	iter->lista->prim = n_act;
	else iter->ant->sig = n_act;

	if (lista_iter_al_final(iter)) iter->lista->ulti = n_act;

	iter->act = n_act;
	iter->lista->largo++;

	return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
	if (lista_iter_al_final(iter)) return NULL;

	void* dato = iter->act->dato;
	
	if (!iter->ant)	iter->lista->prim = iter->act->sig;
	else iter->ant->sig = iter->act->sig;

	if (!iter->act->sig) iter->lista->ulti = iter->ant; 
	
	nodo_t* sig = iter->act->sig;
	free(iter->act);
	iter->act = sig;

	iter->lista->largo--;
	
	return dato;
}