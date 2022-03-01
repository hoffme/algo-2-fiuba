#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

/* *****************************************************************
 *                			TIPOS DE DATOS
 * *****************************************************************/

typedef struct abb abb_t;

typedef struct abb_iter abb_iter_t;

/* *****************************************************************
 *              		  TIPOS DE FUNCIONES
 * *****************************************************************/

typedef int (*abb_comparar_clave_t) (const void*, const void*, void*);

typedef void (*abb_destruir_dato_t) (void*);

typedef void (*abb_destruir_clave_t) (void*);

typedef void* (*abb_copiar_clave_t) (const void*);

typedef bool (*abb_visitar_t) (const void*, void*, void*);

/* *****************************************************************
 *                       PRIMITIVAS DEL ABB
 * *****************************************************************/

/* Crea y devuelve un abb. Se requiere una función de comparación de claves, y si es necesario,
 * una función de destrucción de datos.
 * Pre: comp es una función capaz de comparar dos cadenas, y devolver un entero menor, mayor
 * o igual a 0. dest es una función de destrucción de datos o NULL.
 * Post: el abb fue creado.
*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t f_dest_dato, abb_destruir_clave_t f_dest_clave, abb_copiar_clave_t f_copia_clave);

/* Se inserta una clave y un dato asociado a ella, en una de las hojas del abb. 
 * Si la clave ya existe en el abb, se modifica el dato asociado.
 * Pre: el abb fue creado.
 * Post: el abb contiene una clave nueva, o se modifica el dato asociado a una clave preexistente.
*/
bool abb_guardar(abb_t* arbol, const void* clave, void* dato);

/* Se elimina una clave del abb, y se devuelve el dato asociado a la clave, 
 * o NULL si la clave no existe en el abb.
 * Pre: el abb fue creado.
 * Post: el abb contiene una clave menos.
*/
void* abb_borrar(abb_t* arbol, const void* clave);

/* Devuelve el dato asociado a la clave pasada por parámetro, o NULL si la clave no existe en el abb.
 * Pre: el abb fue creado.
*/
void* abb_obtener(const abb_t* arbol, const void* clave);

/* Devuelve true si la cadena clave existe en el abb, false en caso contrario.
 * Pre: el abb fue creado.
*/
bool abb_pertenece(const abb_t* arbol, const void* clave);

/* Devuelve la cantidad de elementos insertados en el abb.
 * Pre: el abb fue creado.
*/
size_t abb_cantidad(abb_t* arbol);

/* Destruye el abb y sus claves. 
 * Si el abb se creó con una función de destrucción, se le aplica a todos los datos.
 * Post: el abb fue destruido.
*/
void abb_destruir(abb_t* arbol);

/* *****************************************************************
 *              PRIMITIVAS ITERADOR INTERNO DEL ABB
 * *****************************************************************/

/* Se recorren de forma in-order todos los elementos del abb. Puede recibir una función visitar,
 * la cual se le aplica a los elementos por cada iteración, mientras la función devuelva true.
 * También permite un parámetro extra aplicable para interactuar con los elementos.
 * Pre: el abb fue creado.
 * Post: se recorre el abb, y se aplica la función visitar a sus elementos, hasta que
 * se produzca la condición de corte.
*/ 
void abb_in_order(abb_t* arbol, abb_visitar_t f_visitar, void* extra);

void abb_in_order_rango(abb_t *arbol, const void* desde, const void* hasta, abb_visitar_t f_visitar, void* extra, abb_comparar_clave_t f_comp);

#endif // ABB_H