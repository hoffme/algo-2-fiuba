#ifndef AVL_H
#define AVL_H

#include <stdbool.h>
#include <stddef.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct avl avl_t;

// typedef struct avl_iter avl_iter_t;

/* *****************************************************************
 *              DEFINICION DE LOS TIPOS DE FUNCIONES
 * *****************************************************************/

typedef int (*avl_comparar_clave_t) (const char*, const char*);
typedef void (*avl_destruir_dato_t) (void*);

/* *****************************************************************
 *                       PRIMITIVAS DEL AVL
 * *****************************************************************/

/* Crea y devuelve un avl. Se requiere una función de comparación de claves, y si es necesario,
 * una función de destrucción de datos.
 * Pre: comp es una función capaz de comparar dos cadenas, y devolver un entero menor, mayor
 * o igual a 0. dest es una función de destrucción de datos o NULL.
 * Post: el avl fue creado.
*/
avl_t* avl_crear(avl_comparar_clave_t cmp, avl_destruir_dato_t destruir_dato);

/* Se inserta una clave y un dato asociado a ella, en una de las hojas del avl. 
 * Si la clave ya existe en el avl, se modifica el dato asociado.
 * Pre: el avl fue creado.
 * Post: el avl contiene una clave nueva, o se modifica el dato asociado a una clave preexistente.
*/
bool avl_guardar(avl_t* arbol, const char* clave, void* dato);

/* Se elimina una clave del avl, y se devuelve el dato asociado a la clave, 
 * o NULL si la clave no existe en el avl.
 * Pre: el avl fue creado.
 * Post: el avl contiene una clave menos.
*/
void* avl_borrar(avl_t* arbol, const char* clave);

/* Devuelve el dato asociado a la clave pasada por parámetro, o NULL si la clave no existe en el avl.
 * Pre: el avl fue creado.
*/
void* avl_obtener(const avl_t* arbol, const char* clave);

/* Devuelve true si la cadena clave existe en el avl, false en caso contrario.
 * Pre: el avl fue creado.
*/
bool avl_pertenece(const avl_t* arbol, const char* clave);

/* Devuelve la cantidad de elementos insertados en el avl.
 * Pre: el avl fue creado.
*/
size_t avl_cantidad(avl_t* arbol);

/* Destruye el avl y sus claves. 
 * Si el avl se creó con una función de destrucción, se le aplica a todos los datos.
 * Post: el avl fue destruido.
*/
void avl_destruir(avl_t* arbol);

/* *****************************************************************
 *              PRIMITIVAS ITERADOR INTERNO DEL AVL
 * *****************************************************************/

/* Se recorren segun el recorrido todos los elementos del avl. Puede recibir una función visitar,
 * la cual se le aplica a los elementos por cada iteración, mientras la función devuelva true.
 * También permite un parámetro extra aplicable para interactuar con los elementos. TIPOS:
 *		0 -> preorder
 *		1 -> inorder
 * 		2 -> postorder
 * 		3 -> niveles
 * Pre: el avl fue creado.
 * Pre: el avl fue creado.
 * Post: se recorre el avl, y se aplica la función visitar a sus elementos, hasta que
 * se produzca la condición de corte.
*/ 
void avl_iter_interno(avl_t* arbol, int recorrido, bool visitar(const char*, void*, void*), void* extra);

/* *****************************************************************
 *              PRIMITIVAS ITERADOR EXTERNO DEL AVL
 * *****************************************************************/

/* Crea un iterador para el avl, que lo recorre segun el tipo:
 *		0 -> preorder
 *		1 -> inorder
 * 		2 -> postorder
 * 		3 -> niveles
 * Pre: el avl fue creado.
 * Post: devuelve un iterador para el avl, inicializado en el menor valor del avl.
*/
avl_iter_t* avl_iter_crear(const avl_t* arbol, int recorrido);

/* Si es posible, se avanza el iterador hacia la clave inmediatamente mayor.
 * Pre: el iterador fue creado.
 * Post: el iterador avanza un elemento.
*/
bool avl_iter_avanzar(avl_iter_t* iter);

/* Se devuelve el dato asociado a la clave a la cual apunta el iterador.
 * Pre: el iterador fue creado.
*/
const char* avl_iter_ver_actual(const avl_iter_t* iter);

/* Devuelve true si el iterador ya recorrió todos los elementos del avl.
 * Pre: el iterador fue creado.
*/
bool avl_iter_al_final(const avl_iter_t* iter);

/* Se destruye el iterador del avl.
 * Pre: el iterador fue creado.
 * Post: el iterador fue destruido.
*/
void avl_iter_destruir(avl_iter_t* iter);

#endif // AVL_H