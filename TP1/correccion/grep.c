#define _GNU_SOURCE

#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENT_STDIN 3
#define ENT_ARCH 4

enum arg_ent {POS_NOM_PROG, POS_CAD, POS_CANT_LIN, POS_ARCH};

/* --------------------------------		ARCHIVO		-------------------------------- */

bool existe_archivo(char* ruta_archivo) {
	/*	Dada una ruta del archivo devuelve true si existe el archivo, sino 
	 *	devuelve false.
	 */
	FILE* archivo = fopen(ruta_archivo, "r");
	if (!archivo) return false;
	fclose(archivo);
	return true;
}

FILE* cargar_archivo(char* ruta_archivo) {
	/*	Dados una ruta de un archivo, devuelve un puntero al manegador de archivo
	 * 	si hay algun error lo imprime por stderr y devuelve null, si no se pasa una,
	 *	ruta por parametro por defecto el archivo es stdin.
	 */
	FILE* archivo;
	if (ruta_archivo) archivo = fopen(ruta_archivo, "r");
	else archivo = stdin;

	if (!archivo) fprintf(stderr, "%s\n", "No se puede leer el archivo");
	return archivo;
}

/* --------------------------------		ENTRADA 	-------------------------------- */

bool entrada_valida(int argc, char** argv) {
	/*	Dados los parametros de entrada y la cantidad de parametros, devuelve 
	 *	true si la entrada es valida y si hay algun error lo 
	 *	imprime por stderr y devuelve false.
	 */
	if (argc != ENT_STDIN && argc != ENT_ARCH) {
		fprintf(stderr, "%s\n", "Cantidad de parametros erronea");
		return false;
	}

	int can_lin = atoi(argv[POS_CANT_LIN]);
	if (can_lin <= 0 && argv[POS_CANT_LIN][0] != '0') {
		fprintf(stderr, "%s\n", "Tipo de parametro incorrecto");
		return false;
	}

	if (argc == ENT_ARCH && !existe_archivo(argv[POS_ARCH])) {
		fprintf(stderr, "%s\n", "No se pudo leer el archivo indicado");
		return false;
	}

	return true;
}

/* --------------------------------		GREP		-------------------------------- */

bool grep(FILE* archivo, char* cadena_buscada, int cant_impresiones_prev) {
	/*	Dado un archivo, la palabra a buscar y la cantidad de lineas a imprimir,
	 * 	busca las coincidencia en las lineas de la palabra a buscar e imprime
	 * 	la cantidad de lineas pasada por parametro, si hay algun error devuelve
	 *	false e imprime el error por stderr.
	 */
	lista_t* lineas_previas = lista_crear();
	if (!lineas_previas) {
		fprintf(stderr, "%s\n", "Error");
		return false;
	}

	char* linea = NULL;
	size_t linea_cap = 0;
	long linea_tam;

	while((linea_tam = getline(&linea, &linea_cap, archivo)) > 0) {
		if (strcmp(&linea[linea_tam - 1], "\n") == 0) linea[linea_tam - 1] = '\0';

		char* coincidencia = strstr(linea, cadena_buscada);

		lista_insertar_ultimo(lineas_previas, linea);

		if (coincidencia) {
			while (!lista_esta_vacia(lineas_previas)) {
				char* linea_actual = (char*)lista_borrar_primero(lineas_previas);
				printf("%s\n", linea_actual);
				free(linea_actual);
			}
		} else if (lista_largo(lineas_previas) > cant_impresiones_prev) free(lista_borrar_primero(lineas_previas));

		linea = NULL;
	}
	free(linea);

	lista_destruir(lineas_previas, free);
	
	return true;
}

/* --------------------------------		MAIN		-------------------------------- */

int main(int argc, char** argv){
	if (!entrada_valida(argc, argv)) return 1;

	char* ruta_archivo = NULL;
	if (argc == ENT_ARCH) ruta_archivo = argv[POS_ARCH];

	FILE* archivo = cargar_archivo(ruta_archivo);

	int ok = 0;
	if (!grep(archivo, argv[POS_CAD], atoi(argv[POS_CANT_LIN]))) ok = 1;

	if (ruta_archivo) fclose(archivo);
	return ok;
}