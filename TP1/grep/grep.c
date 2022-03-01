#define _GNU_SOURCE
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENT_STDIN 3
#define ENT_ARCH 4

enum arg_ent {POS_NOM_PROG, POS_CAD, POS_CANT_LIN, POR_ARCH};

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

/* --------------------------------		ENTRADA 		-------------------------------- */

typedef struct entrada {
	char* cadena;
	int lineas_anteriores;
	char* ruta_archivo;	
} entrada_t;

entrada_t* crear_entrada(int argc, char** argv) {
	/*	Dados los parametros de entrada y la cantidad de parametros, devuelve 
	 *	un puntero con la informacion de la entrada, si hay algun error lo 
	 *	imprime por stderr y devuelve null.
	 */
	if (argc != ENT_STDIN && argc != ENT_ARCH) {
		fprintf(stderr, "%s\n", "Cantidad de parametros erronea");
		return NULL;
	}

	int can_lin = atoi(argv[POS_CANT_LIN]);
	if (can_lin <= 0 && argv[POS_CANT_LIN][0] != '0') {
		fprintf(stderr, "%s\n", "Tipo de parametro incorrecto");
		return NULL;
	}

	if (argc == ENT_ARCH && !existe_archivo(argv[POR_ARCH])) {
		fprintf(stderr, "%s\n", "No se pudo leer el archivo indicado");
		return NULL;
	}

	entrada_t* entrada = malloc(sizeof(entrada_t));
	if (!entrada) {
		fprintf(stderr, "%s\n", "No hay espacio para guardar la entrada");
		return NULL;	
	}

	entrada->cadena = argv[POS_CAD];
	entrada->lineas_anteriores = can_lin;
	entrada->ruta_archivo = NULL;
	if (argc == ENT_ARCH) entrada->ruta_archivo = argv[POR_ARCH];

	return entrada;
}

/* --------------------------------		MATCH		-------------------------------- */

bool matchear(FILE* archivo, char* bus_str, int cant_lin) {
	/*	Dado un archivo, la palabra a buscar y la cantidad de lineas a imprimir,
	 * 	busca las coincidenncias en las lineas de la palabra a buscar e imprime
	 * 	la cantidad de lineas pasada por parametro, si hay algun error devuelve
	 *	false e imprime el error por stderr.
	 */
	lista_t* buffer = lista_crear();
	if (!buffer) {
		fprintf(stderr, "%s\n", "No se pudo crear una lista");
		return false;
	}

	char* n_linea = NULL;
	size_t cap_entrada = 0;
	ssize_t tam_entrada;

	while((tam_entrada = getline(&n_linea, &cap_entrada, archivo)) > 0) {
		if (strcmp(&n_linea[tam_entrada - 1], "\n") == 0) n_linea[tam_entrada - 1] = '\0';

		char* matchs = strstr(n_linea, bus_str);

		lista_insertar_ultimo(buffer, n_linea);

		if (matchs) {
			while (!lista_esta_vacia(buffer)) {
				char* linea = (char*)lista_borrar_primero(buffer);
				printf("%s\n", linea);
				free(linea);
			}
		} else if (lista_largo(buffer) > cant_lin) free(lista_borrar_primero(buffer));

		n_linea = NULL;
	}
	free(n_linea);

	lista_destruir(buffer, free);
	
	return true;
}

/* --------------------------------		MAIN		-------------------------------- */

int main(int argc, char** argv){
	entrada_t* entrada = crear_entrada(argc, argv);
	if (!entrada) return 0;

	FILE* archivo = cargar_archivo(entrada->ruta_archivo);
	if (!archivo) {
		free(entrada);
		return 0;
	}

	if (!matchear(archivo, entrada->cadena, entrada->lineas_anteriores)) {
		if (entrada->ruta_archivo) fclose(archivo);
		free(entrada);
		return 0;
	}

	if (entrada->ruta_archivo) fclose(archivo);
	free(entrada);
	return 1;
}