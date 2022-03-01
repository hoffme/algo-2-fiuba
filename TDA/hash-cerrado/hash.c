#define _POSIX_C_SOURCE 200809L

#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CAPACIDADES 12
#define FC_MIN 0.2
#define FC_MAX 0.7

/* ---------------------------------   DECLARACIONES   --------------------------------- */

size_t buscar_posicion(const hash_t* hash, const char* clave);

bool redimensionar(hash_t* hash, bool aumentar);

long pos_siguiente(const hash_t *hash, long pos);

/* ----------------------------------   ESTRUCTURAS   ---------------------------------- */

enum est_campo {VACIO, OCUPADO, BORRADO};

typedef struct campo{
	char* clave;
	void* valor;
	int estado;
} campo_t;

struct hash{
	size_t cantidad;
	size_t borrados;

	size_t* capacidades;
	int cap_actual; // índice del vector "capacidades"
	
	campo_t* campos;
	hash_destruir_dato_t f_destruir;
};

struct hash_iter{
	const hash_t* hash;
	long pos_act;
};

/* --------------------------------   FUNCION DE HASH   -------------------------------- */

size_t funcion_hash(const char *str, size_t largo){
	size_t hashval;

	for (hashval = 0; *str != '\0'; str++){
		hashval = (size_t)*str + 31*hashval;
	}
	return hashval % largo;
}

/* ------------------------------   PRIMITIVAS DE HASH   ------------------------------- */
/* CREAR */
hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* nuevo_hash = malloc(sizeof(hash_t));
	if (!nuevo_hash) return NULL;

	size_t* capacidades = malloc(sizeof(size_t) * MAX_CAPACIDADES);
	if (!capacidades){
		free(nuevo_hash);
		return NULL;
	}

	size_t largos_tabla[MAX_CAPACIDADES] = {101, 499, 2503, 10007, 49999, 100003, 250007, 500009, 749993, 1000003, 4999999, 19999999};
	for (int i=0; i < MAX_CAPACIDADES; ++i){
		capacidades[i] = largos_tabla[i];
	}

	nuevo_hash->cantidad = 0;
	nuevo_hash->borrados = 0;
	nuevo_hash->capacidades = capacidades;
	nuevo_hash->cap_actual = 0;

	campo_t* arr_campos = calloc(nuevo_hash->capacidades[nuevo_hash->cap_actual], sizeof(campo_t));
	if (!arr_campos){
		free(nuevo_hash);
		return NULL;
	}
	nuevo_hash->campos = arr_campos;
	nuevo_hash->f_destruir = destruir_dato;

	return nuevo_hash;
}

/* GUARDAR */
bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	if ((float)(hash->cantidad + hash->borrados) / (float)hash->capacidades[hash->cap_actual] >= FC_MAX){
		if (!redimensionar(hash, true)) return false; 
	}

	size_t pos = buscar_posicion(hash, clave);
	if (hash->campos[pos].estado == OCUPADO){
		if (hash->f_destruir) hash->f_destruir(hash->campos[pos].valor);
		hash->campos[pos].valor = dato;
		return true;
	}

	char* str_clave = strdup(clave);
	if (!str_clave) return false;

	hash->campos[pos].clave = str_clave;
	hash->campos[pos].valor = dato;
	hash->campos[pos].estado = OCUPADO;

	hash->cantidad += 1;
	return true;
}

/* BORRAR */
void* hash_borrar(hash_t *hash, const char *clave){
	if ((float)(hash->cantidad + hash->borrados) / (float)hash->capacidades[hash->cap_actual] <= FC_MIN && hash->cap_actual > 0){
		if (!redimensionar(hash, false)) return false; 
	}

	if (!hash_pertenece(hash, clave)) return NULL;

	size_t pos = buscar_posicion(hash, clave);
	if (hash->campos[pos].estado != OCUPADO) return NULL;

	void* devolver = hash->campos[pos].valor;

	hash->campos[pos].estado = BORRADO;
	hash->cantidad -= 1;
	hash->borrados += 1;

	return devolver;
}

/* OBTENER */
void* hash_obtener(const hash_t *hash, const char *clave){
	size_t pos = buscar_posicion(hash, clave);

	if (hash->campos[pos].estado == OCUPADO) return hash->campos[pos].valor;
	return NULL;
}

/* PERTENECE */
bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t pos = buscar_posicion(hash, clave);

	if (hash->campos[pos].estado != OCUPADO){
		return false;
	}
	return true;
}

/* CANTIDAD */
size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}

/* DESTRUIR */
void hash_destruir(hash_t *hash){
	for (int i = 0; i < hash->capacidades[hash->cap_actual]; ++i) {
		if (hash->campos[i].estado == OCUPADO || hash->campos[i].estado == BORRADO){
			free(hash->campos[i].clave);
			if (hash->f_destruir && hash->campos[i].estado != BORRADO){
				hash->f_destruir(hash->campos[i].valor);
			}
		}
	}
	free(hash->campos);
	free(hash->capacidades);
	free(hash);
}

/* AUX: BUSCAR_POSICION */
// Recorre el hash hasta encontrar una posición vacía, o la posición de una clave que ya existe.
size_t buscar_posicion(const hash_t* hash, const char* clave){
	size_t pos = funcion_hash(clave, hash->capacidades[hash->cap_actual]);

	while (hash->campos[pos].estado != VACIO){
		if (strcmp(hash->campos[pos].clave, clave) == 0 && hash->campos[pos].estado != BORRADO) break;
		pos++;
		if (pos == hash->capacidades[hash->cap_actual]) pos = 0;
	}
	return pos;
}

/* AUX: REDIMENSIONAR */
// Aumenta (o reduce) la capacidad máxima del hash, y reubica todos los datos guardados.
bool redimensionar(hash_t* hash, bool aumentar){
	int nuevo_cap_actual;

	if (aumentar && hash->cap_actual < (MAX_CAPACIDADES - 1)){
		nuevo_cap_actual = hash->cap_actual + 1;
	} else if (!aumentar && hash->cap_actual > 0){ 
		nuevo_cap_actual = hash->cap_actual - 1;
	} else return false;

	campo_t* nuevo_campos = calloc(hash->capacidades[nuevo_cap_actual], sizeof(campo_t));
	if (!nuevo_campos) return false; 

	campo_t* viejo_campos = hash->campos;
	size_t viejo_cap_actual = (size_t)hash->cap_actual;

	hash->campos = nuevo_campos;
	hash->cap_actual = nuevo_cap_actual;
	hash->borrados = 0;
	hash->cantidad = 0;

	for (int i = 0; i < hash->capacidades[viejo_cap_actual]; ++i){
		if (viejo_campos[i].estado == OCUPADO){
			hash_guardar(hash, viejo_campos[i].clave, viejo_campos[i].valor);
		}
		free(viejo_campos[i].clave);
	}
	free(viejo_campos);

	return true;
}

/* ----------------------------   PRIMITIVAS DE ITERADOR   ----------------------------- */
/* CREAR */
hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (!iter) return NULL;

	iter->hash = hash;
	iter->pos_act = pos_siguiente(hash, 0);

	return iter;
}

/* AVANZAR */
bool hash_iter_avanzar(hash_iter_t *iter){
	if (hash_iter_al_final(iter)) return false;

	iter->pos_act = pos_siguiente(iter->hash, iter->pos_act + 1);
	return true;
}

/* VER ACTUAL */
const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if (hash_iter_al_final(iter)) return NULL;

	return iter->hash->campos[iter->pos_act].clave;
}

/* AL FINAL */
bool hash_iter_al_final(const hash_iter_t *iter){
	return iter->pos_act == -1;
}

/* DESTRUIR */
void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}

/* AUX: POS_SIGUIENTE */
// Busca en el hash la posición más próxima que contenga un campo ocupado.
long pos_siguiente(const hash_t *hash, long pos){
	long pos_nueva;

	for (pos_nueva = pos; pos_nueva < hash->capacidades[hash->cap_actual]; ++pos_nueva){
		if (hash->campos[pos_nueva].estado == OCUPADO) break;
	}

	if (pos_nueva >= hash->capacidades[hash->cap_actual]) return -1;
	return pos_nueva;
}