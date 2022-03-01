#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

// Los structs deben llamarse "abb" y "abb_iter".
struct abb;

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

// tipos de función para destruir dato y comparar clave
typedef int (*abb_comparar_clave_t) (const char*, const char*);
typedef void (*abb_destruir_dato_t) (void*);


/* -------------------------------   PRIMITIVAS  ABB   ------------------------------- */
/* Crea y devuelve un abb. Se requiere una función de comparación de claves, y si es necesario,
 * una función de destrucción de datos.
 * Pre: comp es una función capaz de comparar dos cadenas, y devolver un entero menor, mayor
 * o igual a 0. dest es una función de destrucción de datos o NULL.
 * Post: el abb fue creado.
*/
abb_t* abb_crear(abb_comparar_clave_t comp, abb_destruir_dato_t dest);

/* Devuelve true si la cadena clave existe en el abb, false en caso contrario.
 * Pre: el abb fue creado.
*/
bool abb_pertenece(const abb_t* arbol, const char* clave);

/* Devuelve la cantidad de elementos insertados en el abb.
 * Pre: el abb fue creado.
*/
size_t abb_cantidad(abb_t* arbol);

/* Devuelve el dato asociado a la clave pasada por parámetro, o NULL si la clave no existe en el abb.
 * Pre: el abb fue creado.
*/
void* abb_obtener(const abb_t* arbol, const char* clave);

/* Se inserta una clave y un dato asociado a ella, en una de las hojas del abb. 
 * Si la clave ya existe en el abb, se modifica el dato asociado.
 * Pre: el abb fue creado.
 * Post: el abb contiene una clave nueva, o se modifica el dato asociado a una clave preexistente.
*/
bool abb_guardar(abb_t* arbol, const char* clave, void* dato);

/* Se elimina una clave del abb, y se devuelve el dato asociado a la clave, 
 * o NULL si la clave no existe en el abb.
 * Pre: el abb fue creado.
 * Post: el abb contiene una clave menos.
*/
void* abb_borrar(abb_t* arbol, const char* clave);

/* Destruye el abb y sus claves. 
 * Si el abb se creó con una función de destrucción, se le aplica a todos los datos.
 * Post: el abb fue destruido.
*/
void abb_destruir(abb_t* arbol);

/* --------------------------   PRIMITIVA ITERADOR INTERNO   ------------------------- */
/* Se recorren de forma in-order todos los elementos del abb. Puede recibir una función visitar,
 * la cual se le aplica a los elementos por cada iteración, mientras la función devuelva true.
 * También permite un parámetro extra aplicable para interactuar con los elementos.
 * Pre: el abb fue creado.
 * Post: se recorre el abb, y se aplica la función visitar a sus elementos, hasta que
 * se produzca la condición de corte.
*/ 
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* -------------------------   PRIMITIVAS ITERADOR EXTERNO   ------------------------- */
/* Crea un iterador para el abb, que lo recorre de forma in-order.
 * Pre: el abb fue creado.
 * Post: devuelve un iterador para el abb, inicializado en el menor valor del abb.
*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Si es posible, se avanza el iterador hacia la clave inmediatamente mayor.
 * Pre: el iterador fue creado.
 * Post: el iterador avanza un elemento.
*/
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Se devuelve el dato asociado a la clave a la cual apunta el iterador.
 * Pre: el iterador fue creado.
*/
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Devuelve true si el iterador ya recorrió todos los elementos del abb.
 * Pre: el iterador fue creado.
*/
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Se destruye el iterador del abb.
 * Pre: el iterador fue creado.
 * Post: el iterador fue destruido.
*/
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H
