#define _POSIX_C_SOURCE 200809L

#include "heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_INI 	16
#define MIN_CANT 	4
#define FACT_REDI_ABJ 	2
#define FACT_REDI_ARB 	2

/*-----------				ESTRUCTURA HEAP					-----------*/

struct heap{
	void** datos;
	size_t cant;
	size_t tam;
	cmp_func_t cmp;
};

/*-----------		DEFINICIONES FUNCIONES AUXILIARES		-----------*/

void upheap(heap_t* heap, size_t pos_elem);

void downheap(void** arreglo, size_t pos_elem, size_t cant, cmp_func_t cmp);

void heapify(void** arreglo, size_t tam, cmp_func_t cmp);

bool heap_redimensionar(heap_t* heap, bool agrandar);

void** copiar_arr(void* arreglo[], size_t tam);

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

	n_heap->datos = copiar_arr(arreglo, n);
	if (!n_heap->datos){
		free(n_heap);
		return NULL;
	}

	n_heap->cant = n;
	n_heap->tam = n;
	n_heap->cmp = cmp;

	for (long i = n_heap->cant / 2; i >= 0; --i) downheap(n_heap->datos, i, n, cmp);

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

	if (heap->cant + 1 >= heap->tam && !heap_redimensionar(heap, true)){
		return false;
	}

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

	if (heap->tam > TAM_INI && heap->cant <= heap->tam / MIN_CANT) heap_redimensionar(heap, false);

	void* prim = heap->datos[0];

	heap->datos[0] = heap->datos[heap->cant - 1];
	heap->cant--;
	downheap(heap->datos, 0, heap->cant, heap->cmp);

	return prim;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if (destruir_elemento){
		for (size_t i = 0; i < heap->cant; i++) destruir_elemento(heap->datos[i]);
	}
	free(heap->datos);
	free(heap);
}

/*-----------				FUNCION HEAPSORT				-----------*/

void heap_sort(void** elementos, size_t cant, cmp_func_t cmp){
	heapify(elementos, cant, cmp);

	for (size_t i = 0; i < cant; ++i){
		swap(elementos, 0, cant - 1 - i);
		downheap(elementos, 0, cant - i - 1, cmp);
	}
}

/*-----------				AUXILIARES HEAP 				-----------*/

void swap(void** arr, size_t pos1, size_t pos2){
	void* aux = arr[pos1];
	arr[pos1] = arr[pos2];
	arr[pos2] = aux;
}

void heapify(void** arreglo, size_t tam, cmp_func_t cmp){
	if (!cmp || tam <= 0) return;
	
	for (long i = tam / 2; i >= 0; --i) downheap(arreglo, i, tam, cmp);
}

void** copiar_arr(void* arreglo[], size_t tam){
	void** n_arr = malloc(sizeof(void*) * tam);
	if (!n_arr) return NULL;

	for (size_t i = 0; i < tam && arreglo[i]; ++i) n_arr[i] = arreglo[i];

	return n_arr;
}

void upheap(heap_t* heap, size_t pos_elem){
	if (pos_elem == 0) return;

	size_t pos_padre = (pos_elem - 1) / 2;

	if (heap->cmp(heap->datos[pos_elem], heap->datos[pos_padre]) > 0){
		swap(heap->datos, pos_elem, pos_padre);
		upheap(heap, pos_padre);
	}
}

void downheap(void** arreglo, size_t pos_elem, size_t cant, cmp_func_t cmp) {
	if (pos_elem >= cant) return;

	size_t pos_padre = pos_elem;
	size_t pos_izq = 2 * pos_elem + 1;
	size_t pos_der = 2 * pos_elem + 2;

	if (pos_izq < cant && cmp(arreglo[pos_izq], arreglo[pos_padre]) > 0){ 
		pos_padre = pos_izq;
	}
	if (pos_der < cant && cmp(arreglo[pos_der], arreglo[pos_padre]) > 0){
		pos_padre = pos_der;
	}
	if (pos_padre != pos_elem){
		swap(arreglo, pos_elem, pos_padre);		
		downheap(arreglo, pos_padre, cant, cmp);
	}
}

bool heap_redimensionar(heap_t* heap, bool agrandar){
	size_t n_tam = heap->tam * FACT_REDI_ARB;
	if (!agrandar) n_tam = heap->tam / FACT_REDI_ABJ;

	void** n_arr = realloc(heap->datos, sizeof(void*) * n_tam);
	if (!n_arr) return false;

	heap->datos = n_arr;
	heap->tam = n_tam;

	return true;
}