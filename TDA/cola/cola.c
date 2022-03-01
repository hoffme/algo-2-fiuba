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

/*-----------    PRIMITIVA NODO    -----------*/

nodo_t* nodo_crear(void){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) return NULL;
	nodo->sig = NULL;
	return nodo;
}

/*-----------    ESTRUCTURA COLA    -----------*/

typedef struct cola {
	nodo_t* prim;
	nodo_t* ulti;
} cola_t;

/*-----------    PRIMITIVAS COLA    -----------*/

cola_t* cola_crear(void){
	cola_t* cola = malloc(sizeof(cola_t));
	if (!cola) return NULL;
	cola->prim = NULL;
	cola->ulti = NULL;
	return cola;
}

bool cola_esta_vacia(const cola_t *cola) {
	return cola->prim == NULL;
}

bool cola_encolar(cola_t *cola, void* valor) {
	nodo_t* n_nodo = nodo_crear();
	if (!n_nodo) return false;

	n_nodo->dato = valor;

	if (cola_esta_vacia(cola)) {
		cola->prim = n_nodo;
	} else {
		cola->ulti->sig = n_nodo;
	}

	cola->ulti = n_nodo;
	return true;
}

void* cola_ver_primero(const cola_t *cola) {
	if (cola_esta_vacia(cola)) return NULL;

	return cola->prim->dato;
}

void* cola_desencolar(cola_t *cola) {
	if (cola_esta_vacia(cola)) return NULL;

	nodo_t* nuevo_prim = cola->prim->sig;
	void* valor = cola->prim->dato;
	free(cola->prim);

	cola->prim = nuevo_prim;
	if (!cola->prim) cola->ulti = NULL;

	return valor;
}

void cola_destruir(cola_t *cola, void destruir_dato(void*)) {
	while (!cola_esta_vacia(cola)) {
		void* dato = cola_desencolar(cola);
		if (destruir_dato) destruir_dato(dato);
	}
	free(cola);
}