#ifndef _OPERACIONES_H
#define _OPERACIONES_H

#include <stdbool.h>

/*	Dado como parametro un entero devuelve un puntero a la posicion
 *	en memoria (heap) del numero, si no se puede devuelve NULL.
 */
long* nuevo_entero(long entero);

/*	Dado como parametro una pila apila la suma de los ultimos 2 
 *	elementos apilados, devolviendo true en caso de exito.
 *	Pre: 	la pila fue creada, y tiene 2 enteros.
 * 	Post: 	desapila los ultimos 2 numero y apila el resultado de la
 * 			suma, si hay algun error queda como estaba estaba antes 
 *			de la operaciony devuelve false.
 */
bool sumar_pila(pila_t* pila);

/*	Dado como parametro una pila apila la resta de los ultimos 2 
 *	elementos apilados, devolviendo true en caso de exito.
 *	Pre: 	la pila fue creada, y tiene 2 enteros.
 * 	Post: 	desapila los ultimos 2 numero y apila el resultado de la
 * 			resta, si hay algun error queda como estaba estaba antes 
 *			de la operaciony devuelve false.
 */
bool restar_pila(pila_t* pila);

/*	Dado como parametro una pila apila la multiplicacion de los ultimos 2 
 *	elementos apilados, devolviendo true en caso de exito.
 *	Pre: 	la pila fue creada, y tiene 2 enteros.
 * 	Post: 	desapila los ultimos 2 numero y apila el resultado de la
 * 			multiplicacion, si hay algun error queda como estaba estaba 
 *			antes de la operaciony devuelve false.
 */
bool multiplicar_pila(pila_t* pila);

/*	Dado como parametro una pila apila la division de los ultimos 2 
 *	elementos apilados, devolviendo true en caso de exito.
 *	Pre: 	la pila fue creada, y tiene 2 enteros.
 * 	Post: 	desapila los ultimos 2 numero y apila el resultado de la
 * 			division, si hay algun error queda como estaba estaba antes 
 *			de la operaciony devuelve false.
 */
bool dividir_pila(pila_t* pila);

/*	Dado como parametro una pila apila la raiz del ultimo elementos apilados, 
 *	devolviendo true en caso de exito.
 *	Pre: 	la pila fue creada, y tiene 1 entero.
 * 	Post: 	desapila el ultimo numero y apila el resultado de la raiz, si hay 
 *	algun error queda como estaba antes de la operacion y devuelve false.
 */
bool raiz_pila(pila_t* pila);

/*	Dado como parametro una pila apila la potencia de los ultimos 2 
 *	elementos apilados, devolviendo true en caso de exito.
 *	Pre: 	la pila fue creada, y tiene 2 enteros.
 * 	Post: 	desapila los ultimos 2 numero y apila el resultado de la
 * 			potencia, si hay algun error queda como estaba estaba antes 
 *			de la operaciony devuelve false.
 */
bool potencia_pila(pila_t* pila);

/*	Dado como parametro una pila apila el logaritmo de los ultimos 2 
 *	elementos apilados, devolviendo true en caso de exito.
 *	Pre: 	la pila fue creada, y tiene 2 enteros.
 * 	Post: 	desapila los ultimos 2 numero y apila el resultado de la
 * 			logritmo, si hay algun error queda como estaba estaba antes 
 *			de la operaciony devuelve false.
 */
bool logaritmo_pila(pila_t* pila);

/*	Dado como parametro una pila desapila 3 elementos y si el ultimo
 *	desapilado es igual a 0 apila el 2 elemento desapilado sino el primero.
 *	Pre: 	la pila fue creada, y tiene 3 enteros.
 * 	Post: 	desapila los ultimos 3 numero y apila el resultado de la operacion,
 * 			si hay algun error queda como estaba estaba antes de la operacion 
 *			y devuelve false.
 */
bool ternario_pila(pila_t* pila);

#endif //