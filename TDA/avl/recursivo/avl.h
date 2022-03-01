#ifndef AVL_H
#define AVL_H

#include <stdbool.h>
#include <stddef.h>

typedef struct avl avl_t;

typedef int (*avl_comparar_clave_t) (const char *, const char *);
typedef void (*avl_destruir_dato_t) (void *);

avl_t* avl_crear(avl_comparar_clave_t cmp, avl_destruir_dato_t destruir_dato);

bool avl_guardar(avl_t *arbol, const char *clave, void *dato);
void *avl_borrar(avl_t *arbol, const char *clave);

void *avl_obtener(const avl_t *arbol, const char *clave);
bool avl_pertenece(const avl_t *arbol, const char *clave);

size_t avl_cantidad(avl_t *arbol);

void avl_destruir(avl_t *arbol);

void avl_in_order(avl_t *arbol, bool visitar(const char *, void *, void *), void *extra);

#endif // AVL_H