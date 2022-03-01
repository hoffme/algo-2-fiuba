#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>
#include "lista.h"

/* *****************************************************************
 *                			TIPOS DE DATOS
 * *****************************************************************/

typedef struct abb abb_t;

typedef struct abb_iter abb_iter_t;

/* *****************************************************************
 *              		  TIPOS DE FUNCIONES
 * *****************************************************************/

typedef int (*abb_cmp_t) (void*, void*);

/*	funcion de comparacion cuando se utiliza el iterador por rango, como primer,
	como primer parametro recibe el parametro, desde o hasta y el segundo es con 
	el que se compara.
*/
typedef int (*abb_cmp_rango_t) (void*, void*);

typedef bool (*abb_visitar_t) (void*, void*);

/* *****************************************************************
 *                       PRIMITIVAS DEL ABB
 * *****************************************************************/

abb_t* abb_crear(abb_cmp_t cmp);

bool abb_guardar(abb_t* arbol, void* clave);

void* abb_borrar(abb_t* arbol, void* clave);

void abb_destruir(abb_t* arbol);

/* *****************************************************************
 *              PRIMITIVAS ITERADOR INTERNO
 * *****************************************************************/

lista_t* abb_rango_lista(abb_t *arbol, abb_cmp_rango_t f_comp, void* desde, void* hasta, bool asc, int cant);

#endif // ABB_H