#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*avl_comparar_clave_t) (const char *, const char *);
typedef void (*avl_destruir_dato_t) (void *);

typedef struct nodo nodo_t;

/* -------------------------------- 		ESTRUCTURAS		-------------------------------- */

typedef struct avl {
	nodo_t* raiz;
	avl_comparar_clave_t f_comp;
	avl_destruir_dato_t f_dest;
	size_t cant;
} avl_t;

struct nodo {
	nodo_t* izq;
	nodo_t* der;
	char* clave;
	void* dato;
	size_t altura;
};

nodo_t* buscar_padre(nodo_t* nodo, avl_comparar_clave_t f_comp, const char* clave);

nodo_t* buscar_nodo(nodo_t* nodo, avl_comparar_clave_t f_comp, const char* clave);

size_t actualizar_altura(avl_t* arbol, nodo_t* nodo, const char* clave);

size_t max(size_t ent1, size_t ent2) {
	if (ent1 > ent2) return ent1;
	return ent2;
}

long mod(long ent1) {
	if (ent1 <= 0) return (ent1 * (-1));
	return ent1;
}

/* -------------------------------- 	PRIMITIVAS DE NODO 	-------------------------------- */

nodo_t* nodo_crear(const char* clave, void* dato, size_t altura) {
	/*	Dado una clave como parametro y un puntero a un dato, crea un nodo, con los hijos nulos
	 * 	y le asigna la clave y el dato y devuleve un puntero, si no se pudo crear devuelve null
	 */
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;

	char* str_clave = strdup(clave);
	if (!str_clave) {
		free(nodo);
		return NULL;	
	}

	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->clave = str_clave;
	nodo->dato = dato;
	nodo->altura = altura;

	return nodo;
}

nodo_t* nodo_hijo(const nodo_t* nodo, avl_comparar_clave_t f_comp, const char* clave) {
	/*	Dado un nodo, una funcion de comparacion y una clave del hijo, devuelve un puntero al
	 *	hijo, en caso de que no tenga o tenga pero la clave sea diferente devuelve null
	 */
	if (nodo->der && f_comp(nodo->der->clave, clave) == 0) return nodo->der;
	else if (nodo->izq && f_comp(nodo->izq->clave, clave) == 0) return nodo->izq;
	return NULL;
}

void nodo_destruir(nodo_t* nodo, avl_destruir_dato_t f_dest, bool recursiva) {
	/*	Dado un nodo, una funcion de destruccion y un booleadno de recusividad, si el booleano 
	 *	es true destruye a los hijos, destruye al dato y libera a la clave y al nodo
	 */
	if (!nodo) return;
	if (recursiva) {
		nodo_destruir(nodo->izq, f_dest, true);
		nodo_destruir(nodo->der, f_dest, true);
	}
	if (f_dest) f_dest(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

size_t nodo_altura(nodo_t* nodo) {
	/*	Devuelve la altura del nodo, siempre y cuando las alturas de los hijos esten bien.
	 */
	size_t altura;
	if (!nodo) altura = 0;
	else if (!nodo->der && !nodo->izq) altura = 1;
	else if (!nodo->der && nodo->izq) altura = nodo->izq->altura + 1;
	else if (!nodo->izq && nodo->der) altura = nodo->der->altura + 1;
	else altura = max(nodo->der->altura, nodo->izq->altura) + 1;
	return altura;
}

/* -------------------------------- 	PRIMITIVAS DE AVL 	-------------------------------- */


avl_t* avl_crear(avl_comparar_clave_t cmp, avl_destruir_dato_t destruir_dato) {
	/*	Dado una funcion de comparacion de las claves y una funcion de destruccion del, crea 
	 *	un avl y devuelve un puntero, si no se puede crear devuelve null.
	 */
	avl_t* avl = malloc(sizeof(avl_t));
	if (!avl) return NULL;

	avl->raiz = NULL;
	avl->f_comp = cmp;
	avl->f_dest = destruir_dato;
	avl->cant = 0;

	return avl;
}

bool avl_guardar(avl_t* arbol, const char* clave, void* dato) {
	/*	Dado un arbol, una clave y un dato, inserta la clave y el dato en el arbol, si la clave
	 *	ya existe remplaza eldato o si hay un error al guardar el dato devuelve null.
	 */

	// printf("GUARDADANDO %s\n", clave);

	nodo_t* padre = buscar_padre(arbol->raiz, arbol->f_comp, clave);

	// if (padre) printf("HAY PADRE: %s\n", padre->clave);
	
	int dir_hijo = 0;
	if (padre) dir_hijo = arbol->f_comp(padre->clave, clave);
		
	if (dir_hijo > 0 && padre->izq) {
		// printf("	HIJO IZQ ACTUALIZANDO\n");
		if (arbol->f_dest) arbol->f_dest(padre->izq->dato);
		padre->izq->dato = dato;
	} else if (dir_hijo < 0 && padre->der) {
		// printf("	HIJO DER ACTUALIZANDO\n");
		if (arbol->f_dest) arbol->f_dest(padre->der->dato);
		padre->der->dato = dato;
	} else if (dir_hijo > 0 && !padre->izq) {
		// printf("	HIJO IZQ CREANDO\n");
		nodo_t* n_nodo = nodo_crear(clave, dato, 1);
		if (!n_nodo) return false;
		padre->izq = n_nodo;
		++arbol->cant;
		// printf("	HIJO IZQ ACTUALIZANDO ALTURA\n");
		arbol->raiz->altura = actualizar_altura(arbol, arbol->raiz, clave);
	} else if (dir_hijo < 0 && !padre->der) {
		// printf("	HIJO DER CREANDO\n");
		nodo_t* n_nodo = nodo_crear(clave, dato, 1);
		if (!n_nodo) return false;
		padre->der = n_nodo;
		++arbol->cant;
		// printf("	HIJO DER ACTUALIZANDO ALTURA\n");
		arbol->raiz->altura = actualizar_altura(arbol, arbol->raiz, clave);
	} else if (arbol->raiz){
		// printf("	ACTUALIZANDO RAIZ\n");
		if (arbol->f_dest) arbol->f_dest(arbol->raiz->dato);
		arbol->raiz->dato = dato;
	} else {
		// printf("	CREANDO RAIZ\n");
		nodo_t* n_nodo = nodo_crear(clave, dato, 1);
		if (!n_nodo) return false;
		arbol->raiz = n_nodo;
		++arbol->cant;
		arbol->raiz->altura = actualizar_altura(arbol, arbol->raiz, clave);
	}

	// printf("	SALIENDO\n");

	return true;
}

void* avl_borrar(avl_t *arbol, const char *clave) {

	// printf("BORRANDO CLAVE %s\n", clave);

	nodo_t* padre = buscar_padre(arbol->raiz, arbol->f_comp, clave);
	nodo_t* nodo;

	// if (padre) printf("	TIENE PADRE: %s\n", padre->clave);
	// else if (!padre && !arbol->raiz) printf("	NO TIENE PADRE NI NODO\n");
	// else if (!padre) printf("	NO HAY PADRE\n");

	if (!padre && !arbol->raiz) return NULL;
	else if (!padre && arbol->raiz) nodo = arbol->raiz;
	else nodo = nodo_hijo(padre, arbol->f_comp, clave);
	if (!nodo) return NULL;

	// if (nodo) printf("	NODO ENCONTRADO\n");

	void* dato = nodo->dato;

	if (nodo->der && nodo->izq) {

		// printf("	TIENE 2 HIJOS\n	BUSCANDO REMPLAZO\n");

		nodo_t* nodo_padre_cambio = nodo;
		nodo_t* nodo_cambio = nodo->der;

		if (!nodo_padre_cambio->der->izq) {
			if (!nodo_padre_cambio->der->der) nodo_padre_cambio->der = NULL;
			else nodo_padre_cambio->der = nodo_padre_cambio->der->der;
		} else {
			while (nodo_cambio->izq) {
				nodo_padre_cambio = nodo_cambio;
				nodo_cambio = nodo_cambio->izq;
			}
			if (!nodo_padre_cambio->izq->der) nodo_padre_cambio->izq = NULL;
			else nodo_padre_cambio->izq = nodo_padre_cambio->izq->der;
		}

		// printf("	REMPLAZO == %s\n", nodo_cambio->clave);
		
		free(nodo->clave);
		nodo->clave = nodo_cambio->clave;
		nodo->dato = nodo_cambio->dato;
		free(nodo_cambio);

		// printf("	CAMBIADO\n	ACTUALIZANDO ALTURA\n");

		nodo_padre_cambio->altura = actualizar_altura(arbol, arbol->raiz, nodo_padre_cambio->clave);

		// printf("	ALTURA ACTUALIZADA");

	} else {
		// printf("	TIENE 1 o 0 HIJOS\n	BUSCANDO HIJO\n");
		nodo_t* hijo_nodo = NULL;
		if (nodo->der) hijo_nodo = nodo->der;
		else if (nodo->izq) hijo_nodo = nodo->izq;
		// else printf("	NO HIJO\n");

		if (!padre) {
			// printf("	ES LA RAIZ, NO TIENE PADRE\n");
			arbol->raiz = hijo_nodo;
		} else {
			// if (hijo_nodo) printf("	NUEVO EN SU LUGAR %s\n", hijo_nodo->clave);
			int dir_nodo = arbol->f_comp(padre->clave, nodo->clave);		
			if (dir_nodo > 0) padre->izq = hijo_nodo;
			else if (dir_nodo < 0) padre->der = hijo_nodo;
		}
		nodo_destruir(nodo, NULL, false);

		if (padre) padre->altura = actualizar_altura(arbol, arbol->raiz, padre->clave);
	}

	// printf("	RESTANDO CANTIDAD\n");
	
	--arbol->cant;
	return dato;
}

void* avl_obtener(const avl_t *arbol, const char *clave) {
	if (!arbol->raiz) return NULL;
	nodo_t*	nodo = buscar_nodo(arbol->raiz, arbol->f_comp, clave);
	if (!nodo) return NULL;
	return nodo->dato;
}

bool avl_pertenece(const avl_t *arbol, const char *clave) {
	if (!arbol->raiz) return NULL;
	return buscar_nodo(arbol->raiz, arbol->f_comp, clave) != NULL;
}

size_t avl_cantidad(avl_t *arbol) {
	return arbol->cant;
}

void avl_destruir(avl_t *arbol) {
	nodo_destruir(arbol->raiz, arbol->f_dest, true);
	free(arbol);
}

void _nodo_in_order(nodo_t* nodo, bool visitar(const char* , void* , void* ), void* extra, bool* estado) {
	/* 	Recorre el arbol en in order le aplica la funcion visitar, en caso de 
	 *	que visitar devuelva false se deja de ejecutar. El estado incialmente
	 *	tiene que ser true para que itere.
	 */
	if (!nodo || !*estado) return;
	_nodo_in_order(nodo->izq, visitar, extra, estado);
	if (*estado) *estado = visitar(nodo->clave, nodo->dato, extra);
	_nodo_in_order(nodo->der, visitar, extra, estado);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	if (!arbol->raiz) return;
	bool continuar = true;
	_nodo_in_order(arbol->raiz, visitar, extra, &continuar);
}

/* -------------------------------- 	AUXILIARES DE avl 	-------------------------------- */

nodo_t* buscar_padre(nodo_t* nodo, avl_comparar_clave_t f_comp, const char* clave) {
	/*	Si el hijo esta en el arbol devuelve el padre original, sino devuelve el padre de la posicion 
	 *	donde se tendira que insertar, si no hay nodo inicial o el inicial es el hijo devuelve null.
	 */
	if (!nodo) return NULL;
	
	int dir = f_comp(nodo->clave, clave);
	if (dir == 0) return  NULL;

	if ((nodo->izq && f_comp(nodo->izq->clave, clave) == 0) || (nodo->der && f_comp(nodo->der->clave, clave) == 0)) return nodo;

	if (dir > 0 && nodo->izq) return buscar_padre(nodo->izq, f_comp, clave);
	else if (dir < 0 && nodo->der) return buscar_padre(nodo->der, f_comp, clave);

	return nodo;
}

nodo_t* buscar_nodo(nodo_t* nodo, avl_comparar_clave_t f_comp, const char* clave) {
	if (!nodo) return NULL;
	int dir = f_comp(nodo->clave, clave) ;
	if (dir > 0) return buscar_nodo(nodo->izq, f_comp, clave);
	else if (dir < 0) return buscar_nodo(nodo->der, f_comp, clave);
	return nodo;
}

void rot_der_der(avl_t* arbol, nodo_t* padre, nodo_t* z, nodo_t* y, nodo_t* x) {
	nodo_t* aux = y->izq;
	y->izq = z;
	z->der = aux;

	if (padre && padre->der == z) padre->der = y;
	else if (padre && padre->izq == z) padre->izq = y;
	else arbol->raiz = y;

	z->altura = nodo_altura(z);
	x->altura = nodo_altura(x);
	y->altura = nodo_altura(y);
}

void rot_izq_izq(avl_t* arbol, nodo_t* padre, nodo_t* z, nodo_t* y, nodo_t* x) {
	nodo_t* aux = y->der;
	y->der = z;
	z->izq = aux;

	if (padre && padre->der == z) padre->der = y;
	else if (padre && padre->izq == z) padre->izq = y;
	else arbol->raiz = y;

	z->altura = nodo_altura(z);
	x->altura = nodo_altura(x);
	y->altura = nodo_altura(y);
}

void rot_der_izq(avl_t* arbol, nodo_t* padre, nodo_t* z, nodo_t* y, nodo_t* x) {
	nodo_t* aux = x->der;
	x->der = y;
	z->der = x;
	y->izq = aux;

	y->altura = nodo_altura(y);
	x->altura = nodo_altura(x);
	z->altura = nodo_altura(z);
	
	rot_der_der(arbol, padre, z, x, y);
}

void rot_izq_der(avl_t* arbol, nodo_t* padre, nodo_t* z, nodo_t* y, nodo_t* x) {
	nodo_t* aux = x->izq;
	x->izq = y;
	z->izq = x;
	y->der = aux;

	y->altura = nodo_altura(y);
	x->altura = nodo_altura(x);
	z->altura = nodo_altura(z);

	rot_izq_izq(arbol, padre, z, x, y);
}

void avl_rotar(avl_t* arbol, const char* rotura) {

	nodo_t* padre_rot = buscar_padre(arbol->raiz, arbol->f_comp, rotura);
	nodo_t* z;
	nodo_t* y;
	nodo_t* x;
	bool pri_der = false;
	bool seg_der = false;

	if (!padre_rot && !arbol->raiz) return;
	else if (!padre_rot && arbol->raiz) z = arbol->raiz;
	else z = nodo_hijo(padre_rot, arbol->f_comp, rotura);

	if (nodo_altura(z->der) > nodo_altura(z->izq)) {
		y = z->der;
		pri_der = true;
	} else y = z->izq;

	if (nodo_altura(y->der) > nodo_altura(y->izq)) {
		x = y->der;
		seg_der = true;
	} else x = y->izq;

	if (pri_der && seg_der) rot_der_der(arbol, padre_rot, z, y, x);
	else if (!pri_der && !seg_der) rot_izq_izq(arbol, padre_rot, z, y, x);
	else if (pri_der && !seg_der) rot_der_izq(arbol, padre_rot, z, y, x);
	else rot_izq_der(arbol, padre_rot, z, y, x);
}

size_t actualizar_altura(avl_t* arbol, nodo_t* nodo, const char* clave) {
	if (!nodo) return 0;

	int dir = arbol->f_comp(nodo->clave, clave);
	if (dir > 0) nodo->altura = max(actualizar_altura(arbol, nodo->izq, clave), nodo_altura(nodo->der)) + 1;
	else if (dir < 0) nodo->altura = max(actualizar_altura(arbol, nodo->der, clave), nodo_altura(nodo->izq)) + 1;
	else nodo->altura = nodo_altura(nodo);

	if (mod(nodo_altura(nodo->der) - nodo_altura(nodo->izq)) > 1) avl_rotar(arbol, nodo->clave);
	
	return nodo->altura;
}