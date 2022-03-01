#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_INI			16
#define MIN_CANT		4
#define FACT_REDI_ABJ	2
#define FACT_REDI_ARB	2

/*-----------	   DEFINICIONES FUNCIONES COMPARACION   	-----------*/

typedef int (*cmp_func_t) (const void *a, const void *b);

/*-----------				ESTRUCTURA NODO					-----------*/

typedef struct heap{
	void** datos;
	size_t cant;
	size_t tam;
	cmp_func_t cmp;
} heap_t;

/*-----------		DEFINICIONES FUNCIONES AUXILIARES		-----------*/

void upheap(heap_t* heap, size_t pos_elem);

void downheap(heap_t* heap, size_t pos_elem);

heap_t* heapify(void** arreglo, size_t tam, cmp_func_t cmp);

bool redimencionar(heap_t* heap, bool agrandar);

void** copi_arr(void* arreglo[], size_t tam);

void swap(void** arr, size_t pos1, size_t pos2);

/*-----------				PRIMITIVAS HEAP 				-----------*/

heap_t* heap_crear(cmp_func_t cmp){
	if (!cmp) return NULL;

	heap_t* n_heap = malloc(sizeof(heap_t));
	if (!n_heap) return NULL;

	n_heap->datos = malloc(sizeof(void*) * TAM_INI);
	if (!n_heap->datos){
		free(n_heap);
		return NULL;
	}

	n_heap->cant = 0;
	n_heap->tam = TAM_INI;
	n_heap->cmp = cmp;

	return n_heap;
}

heap_t* heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	if (!cmp) return NULL;

	heap_t* n_heap = malloc(sizeof(heap_t));
	if (!n_heap) return NULL;

	n_heap->datos = copi_arr(arreglo, n);
	if (!n_heap->datos){
		free(n_heap);
		return NULL;
	}

	n_heap->cant = n;
	n_heap->tam = n;
	n_heap->cmp = cmp;

	for (long i = n_heap->cant / 2; i >= 0; --i) downheap(n_heap, i);

	return n_heap;
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
	return heap->cant == 0;
}

bool heap_encolar(heap_t *heap, void *elem){
	if (!elem) return false;
	
	if (heap->cant + 1 == heap->tam && !redimencionar(heap, true)) return false;

	heap->datos[heap->cant] = elem;
	heap->cant++;

	upheap(heap, heap->cant - 1);

	return true;
}

void* heap_ver_max(const heap_t *heap){
	if (heap_esta_vacio(heap)) return NULL;
	return heap->datos[0];
}

void* heap_desencolar(heap_t *heap){
	if (heap_esta_vacio(heap)) return NULL;

	if (heap->tam > TAM_INI && heap->cant <= heap->tam / MIN_CANT) redimencionar(heap, false);

	void* prim = heap->datos[0];

	heap->datos[0] = heap->datos[heap->cant - 1];
	heap->cant--;
	downheap(heap, 0);

	return prim;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if (destruir_elemento){
		for (size_t i = 0; i < heap->cant; i++) destruir_elemento(heap->datos[i]);
	}
	free(heap->datos);
	free(heap);
}

void heap_sort(void* elementos[], size_t cant, cmp_func_t cmp) {
	heap_t* heap = heapify(elementos, cant, cmp);
	if (!heap) return;

	size_t fin = heap->cant;
	for (size_t i = 0; i < fin; ++i) {
		swap(heap->datos, 0, heap->cant - 1);
		--heap->cant;
		downheap(heap, 0);
	}
	free(heap);
}

/*-----------				AUXILIARES HEAP 				-----------*/

void swap(void** arr, size_t pos1, size_t pos2) {
	void* aux = arr[pos1];
	arr[pos1] = arr[pos2];
	arr[pos2] = aux;
}

heap_t* heapify(void** arreglo, size_t tam, cmp_func_t cmp) {
	if (!cmp) return NULL;

	heap_t* n_heap = malloc(sizeof(heap_t));
	if (!n_heap) return NULL;

	n_heap->cmp = cmp;
	n_heap->cant = tam;
	n_heap->tam = tam;
	n_heap->datos = arreglo;

	for (long i = (tam / 2); i >= 0; --i) downheap(n_heap, i);

	return n_heap;
}

void** copi_arr(void* arreglo[], size_t tam) {
	void** n_arr = malloc(sizeof(void*) * tam);
	if (!n_arr) return NULL;

	for (size_t i = 0; i < tam && arreglo[i]; ++i) n_arr[i] = arreglo[i];

	return n_arr;
}

void upheap(heap_t* heap, size_t pos_elem){
	if (pos_elem <= 0) return;

	size_t pos_padre = (pos_elem - 1) / 2;

	if (heap->cmp(heap->datos[pos_elem], heap->datos[pos_padre]) > 0){
		swap(heap->datos, pos_elem, pos_padre);
		upheap(heap, pos_padre);
	}
}

void downheap(heap_t* heap, size_t pos){
	if (pos >= heap->cant) return;

	size_t padre = pos;
	size_t izq = (2 * pos) + 1;
	size_t der = (2 * pos) + 2;
	size_t cant = heap->cant;

	if (izq < cant && heap->cmp(heap->datos[izq], heap->datos[padre]) > 0) padre = izq;
	if (der < cant && heap->cmp(heap->datos[der], heap->datos[padre]) > 0) padre = der;

	if (padre != pos){
		swap(heap->datos, pos, padre);		
		downheap(heap, padre);
	}
}

bool redimencionar(heap_t* heap, bool agrandar) {
	size_t n_tam = heap->tam * FACT_REDI_ARB;
	if (!agrandar) n_tam = heap->tam / FACT_REDI_ABJ;

	void** n_arr = realloc(heap->datos, sizeof(void*) * n_tam);
	if (!n_arr) return false;

	heap->datos = n_arr;
	heap->tam = n_tam;

	return true;
}