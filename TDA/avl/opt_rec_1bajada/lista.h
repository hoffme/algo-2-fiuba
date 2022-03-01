#ifndef LISTA_H
#define LISTA_H
#include <stddef.h>
#include <stdbool.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

//definicion de la lista
typedef struct lista lista_t;

//definicion de la cola
typedef struct lista_iter lista_iter_t;


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

//Accion:	crea una lista vacia
//Pre:		-
//Post:		creo una lista vacia y la devolvio, en caso de error devolvio null
lista_t *lista_crear(void);

//Accion:	devuelve true si la lista esta vacia, sino false
//Pre:		la lista fue creada
//Post:		devolvio true si la lista estaba vacia, sino devolvio false
bool lista_esta_vacia(const lista_t *lista);

//Accion:	se inserta un dato en la primer posicion
//Pre:		la lista fue creada
//Post:		se inserto un dato en la primer posicion, en caso de exito
//			devuelve true, sino false
bool lista_insertar_primero(lista_t *lista, void *dato);

//Accion:	se inserta un dato en la ultima posicion
//Pre:		la lista fue creada
//Post:		se inserto un dato en la ultima posicion, en caso de exito
//			devuelve true, sino false
bool lista_insertar_ultimo(lista_t *lista, void *dato);

//Accion:	se borra el dato de la primer posicion y lo devuelve
//Pre:		la lista fue creada
//Post:		se borro el dato en la primer posicion, en caso de que no
//			estuviese vacia devuelve el dato, sino devuelve null
void *lista_borrar_primero(lista_t *lista);

//Accion:	devuelve el primer elemento de la lista, si esta vacia null
//Pre:		la lista fue creada
//Post:		se devolvio el dato del primer elemento de la lista, en caso
//			de que la lista estubiese vacia devuelve null
void *lista_ver_primero(const lista_t *lista);

//Accion:	devuelve el ultimo elemento de la lista, si esta vacia null
//Pre:		la lista fue creada
//Post:		se devolvio el dato del ultimo elemento de la lista, en caso
//			de que la lista estubiese vacia devuelve null
void *lista_ver_ultimo(const lista_t* lista);

//Accion:	devuelve el largo de la lista
//Pre:		la lista fue creada
//Post:		devolvio el largo (cantidad de elementos) de la lista
size_t lista_largo(const lista_t *lista);

//Accion:	destruye la lista, si se le pasa una funcion por parametro, se le
//			aplica al dato guardado
//Pre:		la lista fue creada
//Post:		se desruyo la lista, y en el caso de que la funcion sea diferente
//			de null se le aplico a cada uno de los datos
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/* ******************************************************************
 *           PRIMITIVAS DEL ITERADOR INTERNO DE LISTA
 * *****************************************************************/

//Accion:	itera la lista hasta el final o que la funcion le pase false, 
//			se ejecita la funcion pasada por parametro con el primero parametro
//			el dato y el segundo el parametro extra.
//Pre:		la lista fue cradada y se pasa una funcion como parametro != null
//Post: 	se ejecuto la funcion hasta el final o hasta que la funcion devolvio
//			null, se le paso a visitar el dato y el extra.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/* ******************************************************************
 *           PRIMITIVAS DEL ITERADOR EXTERNO DE LISTA
 * *****************************************************************/

//Accion:	crea una iterador para una lista
//Pre:		la lista fue creada
//Post:		creo un iterador para una lista, en caso de error devuelve null
lista_iter_t *lista_iter_crear(lista_t *lista);

//Accion:	devuelve true si se avanzo un elemento, y false si no se puede
//			avanzar mas
//Pre:		el iterador de la lista fue creado
//Post:		devolvio true si se avanzo un elemento y false si no se pudo
//			avanzar
bool lista_iter_avanzar(lista_iter_t *iter);

//Accion:	devuelve el dato en el que esta el iterador, si esta al final null
//Pre:		el iterador de la lista fue creado
//Post:		devolvio el dato en el que esta el iterador, si estava al final 
//			devuelve null
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Accion:	devuelve true si el iterador esta al final, sino false
//Pre:		el iterador de la lista fue creado
//Post:		se devolvio true si esta al final, sino false
bool lista_iter_al_final(const lista_iter_t *iter);

//Accion:	destruye el iterador de la lista
//Pre:		el iterador de la lista fue creado
//Post:		se destruyo el iterador de la lista
void lista_iter_destruir(lista_iter_t *iter);

//Accion:	se incerta un dato en la posicion en la que esta el iterador,
//			en caso de exito se devuelve true, sino false
//Pre:		el iterador de la lista fue creado
//Post:		se incerto un dato en la posicion del iterador y el dato en el
//			que estaba antes de insertar el nuevo dato, es el siguiente a 
//			iterar y devolvio true, sino se puedo insertar devuelve false
//			y queda la lista igual que antes
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Accion:	se borra el elemento de la lista en el que esta el iterador y 
//			lo devuelve, si la lista esta vacia o al final, devuelve null
//Pre:		el iterador de la lista fue creada
//Post:		se borro el dato de la posicion del iterador de la lista, se devolvio
//			el dato y la posicion actual del iterador, es la del dato 
//			sigueinte antes de que se elimine, si la lista estaba vacia 
//			o al final, devuelve null
void *lista_iter_borrar(lista_iter_t *iter);

void pruebas_lista_alumno(void);

#endif // LISTA_H