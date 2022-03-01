#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#define TAM_PILA_DEFECTO 16
#define CANT_MAX 0.75
#define CANT_MIN 0.25
#define CANT_REDUCIR 0.5
#define CANT_AUMENTAR 2

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void) {
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL) return NULL;

	pila->datos = malloc(sizeof(void*) * TAM_PILA_DEFECTO);
	if (pila->datos == NULL) {
		free(pila);
		return NULL;
	}

	pila->cantidad = 0;
	pila->capacidad = TAM_PILA_DEFECTO;

	return pila;
}

void pila_destruir(pila_t *pila) {
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
	return pila->cantidad == 0;
}

bool pila_redimensionar(pila_t *pila, size_t tam_nuevo) {
	void* nuevos_datos = realloc(pila->datos, sizeof(void*) * tam_nuevo);
	if (nuevos_datos == NULL) return false;

	pila->datos = nuevos_datos;
	pila->capacidad = tam_nuevo;

	return true;
}

bool pila_apilar(pila_t *pila, void* valor) {

	if (pila->cantidad > ((double)pila->capacidad * CANT_MAX)){
		size_t nueva_capacidad = (int)(pila->capacidad * CANT_AUMENTAR);
		bool redimensionado = pila_redimensionar(pila, nueva_capacidad);

		if (redimensionado == false && pila->capacidad == pila->cantidad) {
			return false;
		}
	}

	pila->datos[pila->cantidad] = valor;
	pila->cantidad += 1;

	return true;
}

void* pila_ver_tope(const pila_t *pila) {
	if (pila_esta_vacia(pila)) return NULL;
	return pila->datos[pila->cantidad - 1];
}

void* pila_desapilar(pila_t *pila) {
	if (pila_esta_vacia(pila)) return NULL;

	if (pila->cantidad > TAM_PILA_DEFECTO && pila->cantidad < ((double)pila->capacidad * CANT_MIN)){
		size_t nueva_capacidad = (long)((double)pila->capacidad * CANT_REDUCIR);
		pila_redimensionar(pila, nueva_capacidad);
	}

	void* tope = pila_ver_tope(pila);
	pila->cantidad -= 1;

	return tope;
}