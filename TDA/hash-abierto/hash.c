#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#define MAX_CAPACIDADES 12
#define FC_MIN 0.2
#define FC_MAX 0.7

/* --------------------------------			DECLARACIONES 	-------------------------------- */

size_t buscar_posicion(const hash_t* hash, const char* clave);

char* strdup(const char *src);

bool redimensionar(hash_t* hash, bool arriba);

/* -------------------------------- 		ESTRUCTURAS		-------------------------------- */

typedef struct campo {
	char* clave;
	void* valor;
} campo_t;

struct hash {
	int capacidad;
	size_t* capacidades;
	size_t cantidad;
	size_t borrados;
	campo_t* campos;
	hash_destruir_dato_t f_destruir;
}

struct hash_iter {
	const hash_t* hash;
	long pos_act;
}

/* -------------------------------- 	FUNCION DE HASH 	-------------------------------- */

size_t funcion_hash(const char *str, size_t largo) {
	
	size_t hashval;

	for (hashval = 0; *str != '\0'; str++) {
		hashval = (size_t)*str + 31*hashval;
	}

	return hashval % largo;
}

/* -------------------------------- 	PRIMITIVAS DE HASH 	-------------------------------- */

hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* nuevo_hash = malloc(sizeof(hash_t));
	if (!nuevo_hash) return NULL;

	size_t* capacidades = malloc(sizeof(size_t) * MAX_CAPACIDADES);
	if (!capacidades) {
		free(nuevo_hash);
		return NULL;
	}
	size_t cap_aux[MAX_CAPACIDADES] = {101, 499, 2503, 10007, 49999, 100003, 250007, 500009, 749993, 1000003, 4999999, 19999999};
	for (int i = 0; i < MAX_CAPACIDADES; ++i) capacidades[i] = cap_aux[i];

	nuevo_hash->cantidad = 0;
	nuevo_hash->capacidad = 0;
	nuevo_hash->borrados = 0;
	nuevo_hash->capacidades = capacidades;

	campo_t* arr_campos = calloc(nuevo_hash->capacidades[nuevo_hash->capacidad], sizeof(campo_t));
	if (!arr_campos){
		free(nuevo_hash);
		return NULL;
	}
	nuevo_hash->campos = arr_campos;
	nuevo_hash->f_destruir = destruir_dato;

	return nuevo_hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	if ((float)(hash->cantidad + hash->borrados) / (float)hash->capacidades[hash->capacidad] >= FC_MAX){
		if (!redimensionar(hash, true)) return false; 
	}

	size_t pos = buscar_posicion(hash, clave);
	if (hash->campos[pos]){
		if (hash->f_destruir) hash->f_destruir(hash->campos[pos]->valor);
		hash->campos[pos]->valor = dato;
		return true;
	}

	campo_t* nuevo_campo = malloc(sizeof(campo_t));
	if (!nuevo_campo) return false;

	char* str_clave = strdup(clave);
	if (!str_clave) {
		free(nuevo_campo);
		return false;
	}

	nuevo_campo->clave = str_clave;
	nuevo_campo->valor = dato;
	nuevo_campo->estado = OCUPADO;

	hash->campos[pos] = nuevo_campo;
	hash->cantidad += 1;

	return true;
}

void* hash_borrar(hash_t *hash, const char *clave){
	if ((float)(hash->cantidad + hash->borrados) / (float)hash->capacidades[hash->capacidad] <= FC_MIN && hash->capacidad > 0){
		if (!redimensionar(hash, false)) return false; 
	}

	size_t pos = buscar_posicion(hash, clave);

	if (!hash->campos[pos]) return NULL;

	void* devolver = hash->campos[pos]->valor;

	hash->campos[pos]->estado = BORRADO;
	hash->cantidad -= 1;
	hash->borrados += 1;

	return devolver;
}

void* hash_obtener(const hash_t *hash, const char *clave){
	size_t pos = buscar_posicion(hash, clave);

	if (!hash->campos[pos]) return NULL;

	return hash->campos[pos]->valor;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t pos = buscar_posicion(hash, clave);

	if (!hash->campos[pos]){
		return false;
	}
	return true;
}

size_t hash_cantidad(const hash_t *hash) {
	return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
	for (int i = 0; i < hash->capacidades[hash->capacidad]; ++i) {
		if (hash->campos[i]) {
			if (hash->f_destruir) hash->f_destruir(hash->campos[i]->valor);
			free(hash->campos[i]->clave);
			free(hash->campos[i]);
		} 
	}
	free(hash->campos);
	free(hash->capacidades);
	free(hash);
}

size_t buscar_posicion(const hash_t* hash, const char* clave){
	size_t pos = funcion_hash(clave, hash->capacidades[hash->capacidad]);

	while (hash->campos[pos]){
		if (strcmp(hash->campos[pos]->clave,clave) == 0 && hash->campos[pos]->estado != BORRADO) break;
		pos++;
		if (pos == hash->capacidades[hash->capacidad]) pos = 0;
	}

	return pos;
}

char *strdup(const char *src) {
	char *dst = malloc(strlen(src) + 1);  
	if (dst == NULL) return NULL;         
	strcpy(dst, src);                     
	return dst;                           
}

bool redimensionar(hash_t* hash, bool arriba) {
	int pos_capacidad;
	if (arriba && hash->capacidad < (MAX_CAPACIDADES - 1)) pos_capacidad = hash->capacidad + 1;
	else if (!arriba && hash->capacidad > 0) pos_capacidad = hash->capacidad - 1;
	else return false;

	campo_t** n_campos = calloc(hash->capacidades[pos_capacidad], sizeof(campo_t*));
	if (!n_campos){
		free(n_campos);
		return false;
	}

	campo_t** v_campos = hash->campos;
	size_t v_capacidad = hash->capacidad;

	hash->campos = n_campos;
	hash->capacidad = pos_capacidad;
	hash->borrados = 0;
	hash->cantidad = 0;

	for (int i = 0; i < hash->capacidades[v_capacidad]; ++i) {
		if (v_campos[i] && v_campos[i]->estado == OCUPADO) {
			size_t n_pos = buscar_posicion(hash, v_campos[i]->clave);
			hash->campos[n_pos] = v_campos[i];
			hash->cantidad++;
		} else if (v_campos[i]) {
			free(v_campos[i]->clave);
			free(v_campos[i]);
		}
	}
	free(v_campos);

	return true;
}

long pos_siguiente(const hash_t *hash, long pos) {
	long pos_nueva;
	for (pos_nueva = pos; pos_nueva < hash->capacidades[hash->capacidad]; ++pos_nueva) {
		if (hash->campos[pos_nueva] && hash->campos[pos_nueva]->estado == OCUPADO) break;
	}
	if (pos_nueva == hash->capacidades[hash->capacidad]) return -1;
	return pos_nueva;
}

hash_iter_t *hash_iter_crear(const hash_t *hash) {
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (!iter) return NULL;
	iter->hash = hash;
	iter->pos_act = pos_siguiente(hash, 0);

	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
	if (hash_iter_al_final(iter)) return false;
	iter->pos_act = pos_siguiente(iter->hash, iter->pos_act + 1);
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	if (hash_iter_al_final(iter)) return NULL;
	return iter->hash->campos[iter->pos_act]->clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
	return iter->pos_act == -1;
}

void hash_iter_destruir(hash_iter_t* iter) {
	free(iter);
}