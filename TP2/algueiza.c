#define _GNU_SOURCE

#include "strutil.h"
#include "hash.h"
#include "abb.h"
#include "heap.h"
#include "pila.h"
#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	POS_COMAND 		0
#define	RUT_ARCH 		1
#define	ID_VUELO 		1
#define	CANT_VUE 		1
#define	DESD_BOR 		1
#define	HAST_BOR 		2
#define	MODO_VTB 		2
#define	DESD_VTB 		3
#define HAST_VTB 		4
#define CANT_ARG_VUELO 	10

enum pos_arg_vuelos {ID, AIRLINE, ORIGIN, DESTIN, TAIL_NUM, PRIORITY, DATE, DELAY, AIR_TIME, CANCEL};

/* -------------------------------    AUXILIARES    ------------------------------ */

int len_arr(char* arr[]){
	int largo;
	for (largo = 0; arr[largo]; ++largo);
	return largo;
}

int cmp_heap(const void* a, const void* b){
	char** vuelo_a = (char**)a;
	char** vuelo_b = (char**)b;

	int cmp_prioridad = strcmp(vuelo_a[PRIORITY], vuelo_b[PRIORITY]);

	if (cmp_prioridad > 0) return -1;
	else if (cmp_prioridad < 0) return 1;

	int cmp_id = strcmp(vuelo_a[ID], vuelo_b[ID]);

	if (cmp_id > 0) return 1;
	else if (cmp_id < 0) return -1;

	return 0;
}

int cmp_arbol(void* a, void* b){
	char** vuelo_a = (char**)a;
	char** vuelo_b = (char**)b;

	int cmp_fecha = strcmp(vuelo_a[DATE], vuelo_b[DATE]);

	if (cmp_fecha > 0) return 1;
	else if (cmp_fecha < 0) return -1;

	int cmp_id = strcmp(vuelo_a[ID], vuelo_b[ID]);

	if (cmp_id > 0) return 1;
	else if (cmp_id < 0) return -1;

	return 0;
}

int cmp_arbol_rango(void* a, void* b){
	int cmp_fecha = strcmp(((char*)a), ((char**)b)[DATE]);

	if (cmp_fecha > 0) return 1;
	else if (cmp_fecha < 0) return -1;

	return 0;
}

void wrp_free_strv(void* arr){
	free_strv((char**)arr);
}

/* -----------------------------    CARGAR VUELOS    ----------------------------- */

bool existe_archivo(char* ruta_archivo){
	FILE* archivo = fopen(ruta_archivo, "r");
	if (!archivo) return false;
	fclose(archivo);
	return true;
}

bool cargar_datos(char* ruta_archivo, hash_t* vuelos, abb_t* arbol_fechas){
	FILE* archivo = fopen(ruta_archivo, "r");
	if (!archivo) return false;

	char* linea = NULL;
	size_t linea_cap = 0;
	long linea_tam;
	char** vuelo;

	while((linea_tam = getline(&linea, &linea_cap, archivo)) > 0){
		if (!strcmp(&linea[linea_tam - 1], "\n")) linea[linea_tam - 1] = '\0';

		vuelo = split(linea, ',');
		if (!vuelo) continue;

		if (hash_pertenece(vuelos, vuelo[ID])){
			char** vuelo_desactualizado = (char**)hash_obtener(vuelos, vuelo[ID]);
			abb_borrar(arbol_fechas, vuelo_desactualizado);
		}

		hash_guardar(vuelos, vuelo[ID], vuelo);
		abb_guardar(arbol_fechas, vuelo);
	}
	free(linea);
	fclose(archivo);
	return true;
}

/* ------------------------------    INFO VUELOS    ------------------------------ */

bool mostrar_info_vuelo(char* codigo_vuelo, hash_t* vuelos){
	char** vuelo = hash_obtener(vuelos, codigo_vuelo);
	if (!vuelo) return false;

	for (int i = 0; vuelo[i]; ++i) {
		if ((char**)vuelo[i + 1]) printf("%s ", ((char**)vuelo)[i]);
		else printf("%s\n", ((char**)vuelo)[i]);
	}

	return true;
}

/* ---------------------------    PRIORIDAD VUELOS    ---------------------------- */

bool mostrar_prioridad_vuelos(int cant_vuelos, hash_t* vuelos){
	// Creacion de estructuras auxiliares
	heap_t* prioridad = heap_crear(cmp_heap); // Heap de maximos con funcion de comparacion invertida
	if (!prioridad) return false;

	pila_t* pila = pila_crear();
	if (!pila){
		heap_destruir(prioridad, NULL);
		return false;
	}

	hash_iter_t* iter_vuelos = hash_iter_crear(vuelos);
	if (!iter_vuelos){
		heap_destruir(prioridad, NULL);
		pila_destruir(pila);
		return false;
	}

	// Itera por el hash de vuelos y encola en el heap los k elementos mayores
	while (!hash_iter_al_final(iter_vuelos)){
		char** vuelo = (char**)hash_obtener(vuelos, hash_iter_ver_actual(iter_vuelos));

		if (heap_cantidad(prioridad) < cant_vuelos || 0 < cmp_heap(heap_ver_max(prioridad),vuelo)) heap_encolar(prioridad, vuelo);
		if (cant_vuelos < heap_cantidad(prioridad)) heap_desencolar(prioridad);

		hash_iter_avanzar(iter_vuelos);
	}

	// Desencola elementos del heap y apila para invertir el orden
	while (!heap_esta_vacio(prioridad)) pila_apilar(pila, heap_desencolar(prioridad));

	// Desapila cada elemento y lo imprime
	while (!pila_esta_vacia(pila)){
		char** vuelo = (char**)pila_desapilar(pila);
		printf("%s - %s\n", vuelo[PRIORITY], vuelo[ID]);
	}

	// Destruccion de estructuras auxiliares
	heap_destruir(prioridad, NULL);
	pila_destruir(pila);
	hash_iter_destruir(iter_vuelos);

	return true;
}

/* -----------------------------    BORRAR VUELOS    ----------------------------- */

bool lista_borrar_vuelo(void* vuelo, void* extra){
	hash_t* vuelos = ((void**)extra)[0];
	abb_t* arbol_fechas = ((void**)extra)[1];

	abb_borrar(arbol_fechas, vuelo);
	hash_borrar(vuelos, ((char**)vuelo)[ID]);

	for (int i = 0; ((char**)vuelo)[i]; ++i) {
		if (((char**)vuelo)[i + 1]) printf("%s ", ((char**)vuelo)[i]);
		else printf("%s\n", ((char**)vuelo)[i]);
	}

	return true;
}

bool borrar_vuelos(char* desde, char* hasta, hash_t* vuelos, abb_t* arbol_fechas){
	lista_t* vuelos_borrar = abb_rango_lista(arbol_fechas, cmp_arbol_rango, desde, hasta, true, -1);
	if (!vuelos_borrar) return false;

	void* arr[] = {vuelos, arbol_fechas};
	lista_iterar(vuelos_borrar, lista_borrar_vuelo, &arr);

	lista_destruir(vuelos_borrar, wrp_free_strv);
	return true;
}

/* ------------------------------    VER TABLERO    ------------------------------ */

bool lista_mostrar_vuelo(void* vuelo, void* _){
	printf("%s - %s\n", ((char**)vuelo)[DATE], ((char**)vuelo)[ID]);
	return true;
}

bool ver_tablero(int cant_vuelos_mostrar, char* modo, char* desde, char* hasta, abb_t* arbol_fechas){
	bool asc = false;
	if (!strcmp(modo, "asc")) asc = true;

	lista_t* vuelos = abb_rango_lista(arbol_fechas, cmp_arbol_rango, desde, hasta, asc, cant_vuelos_mostrar);
	if (!vuelos) return false;

	lista_iterar(vuelos, lista_mostrar_vuelo, NULL);

	lista_destruir(vuelos, NULL);
	return true;
}

/* -----------------------------    VERIFICACIONES    ---------------------------- */

void verificar_y_ejecutar(char** param, hash_t* vuelos, abb_t* arbol_fechas){
	bool ok = false;
	
	if (len_arr(param) == 2){

		if (!strcmp(param[POS_COMAND], "agregar_archivo") && existe_archivo(param[RUT_ARCH])){
			ok = cargar_datos(param[RUT_ARCH], vuelos, arbol_fechas);

		} else if (!strcmp(param[POS_COMAND], "info_vuelo")){
			ok = mostrar_info_vuelo(param[ID_VUELO], vuelos);

		} else if (!strcmp(param[POS_COMAND], "prioridad_vuelos") && atoi(param[CANT_VUE]) > 0){		
			ok = mostrar_prioridad_vuelos(atoi(param[CANT_VUE]), vuelos);
		}

	} else if (len_arr(param) == 3 && !strcmp(param[POS_COMAND], "borrar") && strcmp(param[DESD_BOR], param[HAST_BOR]) < 0){
		ok = borrar_vuelos(param[DESD_BOR], param[HAST_BOR], vuelos, arbol_fechas);

	} else if (len_arr(param) == 5 && !strcmp(param[POS_COMAND], "ver_tablero") && atoi(param[CANT_VUE]) > 0){

		if (strcmp(param[DESD_VTB], param[HAST_VTB]) < 0 && (!strcmp(param[MODO_VTB], "asc") || !strcmp(param[MODO_VTB], "desc"))){
			ok = ver_tablero(atoi(param[CANT_VUE]), param[MODO_VTB], param[DESD_VTB], param[HAST_VTB], arbol_fechas);
		}
	}
	
	if (ok) printf("OK\n");
	else fprintf(stderr, "Error en comando %s\n", param[POS_COMAND]);
}

/* ---------------------------    FUNCION PRINCIPAL    --------------------------- */

void control_aeropuerto(void){
	bool ok = true;

	hash_t* vuelos = hash_crear(wrp_free_strv);
	abb_t* arbol_fechas = abb_crear(cmp_arbol);

	if (!vuelos || !arbol_fechas) ok = false;

	char* entrada = NULL;
	size_t entrada_cap = 0;
	long entrada_tam;
	char** parametros;

	while(ok && (entrada_tam = getline(&entrada, &entrada_cap, stdin)) > 0){
		if (!strcmp(&entrada[entrada_tam - 1], "\n")) entrada[entrada_tam - 1] = '\0';

		parametros = split(entrada, ' ');
		if (!parametros) ok = false;
		else verificar_y_ejecutar(parametros, vuelos, arbol_fechas);

		free_strv(parametros);
	}

	if (!ok) fprintf(stderr, "Error\n");

	free(entrada);
	abb_destruir(arbol_fechas);
	hash_destruir(vuelos);
}

int main(int argc, char** argv){
	control_aeropuerto();
	return 0;
}
