#define _GNU_SOURCE
#include "pila.h"
#include "strutil.h"
#include "operaciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef bool(oper_t)(pila_t*);

oper_t* operacion(char* oper_str) {
	/*	Dado un string de la operacion a realizar devuelve el puntero a la 
	 * 	funciond de la operacion, caso contrario devuelve NULL.
	 */
	if (strcmp(oper_str, "+") == 0) return sumar_pila;
	else if (strcmp(oper_str, "-") == 0) return restar_pila;
	else if (strcmp(oper_str, "*") == 0) return multiplicar_pila;
	else if (strcmp(oper_str, "/") == 0) return dividir_pila;
	else if (strcmp(oper_str, "sqrt") == 0) return raiz_pila;
	else if (strcmp(oper_str, "^") == 0) return potencia_pila;
	else if (strcmp(oper_str, "log") == 0) return logaritmo_pila;
	else if (strcmp(oper_str, "?") == 0) return ternario_pila;
	else return NULL;
}

bool cargar_entrada(pila_t* pila, char** parametros) {
	/*	Dada una pila, un vector de cadenas y un puntero a la cantidad de parametros, devuelve
	 * 	true o false si puede aplicar todas las operaciones y apila el resultado en la pila 
	 *	le asigna a cant_param la cantidad, si algo falla devuelve false.	
	 */
	size_t cant_param;
	bool ult_par_val_oper = true;
	for (cant_param = 0; parametros[cant_param]; ++cant_param) {
		if (strcmp(parametros[cant_param],"") == 0) continue;

		long entero = atoi(parametros[cant_param]);
		if (entero != 0 || strcmp(parametros[cant_param], "0") == 0) {
			long* ent = nuevo_entero(entero);					//	funcion dentro de operaciones
			if (ent && pila_apilar(pila, ent)) continue;
			else if (ent) free(ent);
			ult_par_val_oper = false;
		}

		oper_t* oper = operacion(parametros[cant_param]);
		if (oper && oper(pila)) {
			ult_par_val_oper = true;
			continue;
		}
		
		return false;
	}
	if (ult_par_val_oper) return true;
	return false;
}

int main(int argc, char** argv){
	pila_t* pila = pila_crear();
	if (!pila) {
		printf("ERROR\n");
		return 0;
	}

	char* entrada = NULL;
	size_t cap_entrada = 0;
	ssize_t tam_entrada;
	char** parametros;
	bool estado;

	while((tam_entrada = getline(&entrada, &cap_entrada, stdin)) > 0) {
		if (strcmp(&entrada[tam_entrada - 1], "\n") == 0) entrada[tam_entrada - 1] = '\0';
		estado = true;

		parametros = split(entrada, ' ');
		if (!parametros) estado = false;

		estado = cargar_entrada(pila, parametros);		

		if (estado) {
			int* res = pila_desapilar(pila);
			if (res && pila_esta_vacia(pila)) printf("%d\n", *res);
			else estado = false;
			free(res);
		}
		if (!estado) printf("ERROR\n");

		while (!pila_esta_vacia(pila)) free(pila_desapilar(pila));
		if (parametros) free_strv(parametros);
		free(entrada);
		entrada = NULL;
	}
	free(entrada);
	pila_destruir(pila);
	return 1;
}