#define _GNU_SOURCE

#include "pila.h"
#include "strutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long* nuevo_entero(long n) {
	/*	Dado como parametro un entero devuelve un puntero a la posicion
	 *	en memoria (heap) del numero, si no se puede devuelve NULL.
	 */
	long* p_n = malloc(sizeof(long));
	if (!p_n) return NULL;
	*p_n = n;
	return p_n;
}

long loga(long argumento, long base) {
	/*	Dado un argumento y una base devuelve el logarimo entero,
	 *	Pre:	la base debe ser mayor a 1
	 *	Post: 	devuelve el logaritmo entero
	 */
	long div = argumento / base;
	if (div == 1) return 1;
	else if (div == 0) return 0;

	return 1 + loga(div, base);
}

long pot(long numero, long exp) {
	/*	Dado un numero y un exponente devuelve la potencia,
	 *	Pre:	el exponente debe ser mayor o igual a 0
	 *	Post: 	devuelve el resultado de la potencia
	 */
	if (exp == 0) return 1;
	
	if (exp % 2 == 0) {
		long pot_men = pot(numero, exp / 2);
		return pot_men * pot_men;
	} else {
		long pot_men = pot(numero, (exp - 1) / 2);
		return pot_men * pot_men * numero;
	}
}

long raiz(long numero, long men, long may) {
	/*	Dado un numero, el inicio (0) y el fin (numero) donde buscar la raiz entera,
	 *	Pre:	el numero debe ser mayor o igual a 0
	 *	Post: 	devuelve el resultado de la potencia
	 */
	if (may - men == 1 && may * may > numero) return men;
	else if (may - men == 1) return may;
	
	long med = (may + men) / 2;
	if (med * med < numero) return raiz(numero, med, may);
	else if (med * med > numero) return raiz(numero, men, med);
	else return med;
}

bool ternario_pila(pila_t* pila) {
	/*	Dada una pila, desapila 3 numeros y le aplica la operacion ternaria,
	 *	el resultado lo apila a la pila, si la operacion fue exitosa devuelve 
	 *	true sino false.
	 *	Post:	Los 3 ultimos elementos que son desapilados no vuelven
	 *			a apilarse en ningun caso (error o exito).
	 */
	long* ent1 = pila_desapilar(pila);
	long* ent2 = pila_desapilar(pila);
	long* ent3 = pila_desapilar(pila);

	if (!ent1 || !ent2 || !ent3){
		if (ent1) free(ent1);
		if (ent2) free(ent2);
		if (ent3) free(ent3);
		return false;
	}

	if (*ent1 != 0) {
		long* aux = ent2;
		ent2 = ent3;
		ent3 = aux;
	}

	free(ent1);
	free(ent2);

	if (pila_apilar(pila, ent3)) return true;
	else free(ent3);

	return false;
}

bool operacion_2_desapilados(pila_t* pila, char* oper_str) {
	/*	Dada una pila y una operacion, desapila 2 numeros y aplica la operacion 
	 *	correspondiente, el resultado lo apila a la pila, si la operacion fue 
	 *	exitosa devuelve true sino false. Si se pasa una operacion invalida 
	 *	devuelve false y desapilo 2 elementos.
	 *	Post:	Los 2 ultimos elementos que son desapilados no vuelven
	 *			a apilarse en ningun caso (error o exito)
	 */
	long* ent1 = pila_desapilar(pila);
	long* ent2 = pila_desapilar(pila);

	if (!ent1 || !ent2){
		if (ent1) free(ent1);
		if (ent2) free(ent2);
		return false;
	}

	long* res = NULL;

	if (strcmp(oper_str, "+") == 0) res = nuevo_entero(*ent1 + *ent2);
	else if (strcmp(oper_str, "-") == 0) res = nuevo_entero(*ent1 - *ent2);
	else if (strcmp(oper_str, "*") == 0) res = nuevo_entero(*ent1 * *ent2);
	else if (strcmp(oper_str, "/") == 0 && *ent2 != 0) res = nuevo_entero(*ent1 / *ent2);
	else if (strcmp(oper_str, "^")  == 0 && *ent2 >= 0) res = nuevo_entero(pot(*ent1, *ent2));
	else if (strcmp(oper_str, "log") == 0 && *ent2 > 1 && *ent1 > 0) res = nuevo_entero(loga(*ent1, *ent2));

	free(ent1);
	free(ent2);

	if (res && pila_apilar(pila, res)) return true;
	else if (res) free(res);

	return false;
}

bool raiz_pila(pila_t* pila) {
	/*	Dada una pila, desapila 1 numero y aplica la operacion raiz, el 
	 *	resultado lo apila en la pila, si la operacion fue exitosa devuelve 
	 *	true sino false.
	 *	Post:	El ultimo elemento que es desapilado no vuelve a apilarse 
	 *			en ningun caso (error o exito)
	 */
	long* ent1 = pila_desapilar(pila);

	if (!ent1 || *ent1 < 0) {
		if (ent1) free(ent1);
		return false;
	}

	long* res = nuevo_entero(raiz(*ent1, 0, *ent1));

	free(ent1);

	if (res && pila_apilar(pila, res)) return true;
	else if (res) free(res);

	return false;
}

bool operacion(char* oper_str, pila_t* pila) {
	/*	Dada una pila y una operacion, desapila los elementos que necesita
	 *	para la operacion y la realiza. Si la operacion fue exitos apila el
	 *	resultado y devuelve true, caso contrario devuelve false y tambien
	 *	la pila vuelve con menos elementos.
	 *	Post:	Si la operacion es ?, desapila 3 elementos, si es sqrt desapila
	 *			1 elementos y en otro caso desapila 2 elementos.
	 */
	if (strcmp(oper_str, "?") == 0) return ternario_pila(pila);
	else if (strcmp(oper_str, "sqrt") == 0) return raiz_pila(pila);
	return operacion_2_desapilados(pila, oper_str);
}

bool dc(pila_t* pila, char* linea) {
	/*	Dada una pila y una cadena con operaciones en notacion polaca inversa
	 *	devuelve true si se pudieron aplicar todas las operaciones y apila el
	 *	resultado en caso de error devuelve false y es indefinido lo que queda en la pila.
	 */

	char** parametros = split(linea, ' ');
	if (!parametros) return false;

	bool ok = true;
	for (size_t i = 0; parametros[i]; ++i) {
		if (strcmp(parametros[i],"") == 0) continue;

		long entero = atoi(parametros[i]);
		if (entero != 0 || strcmp(parametros[i], "0") == 0) {

			long* ent = nuevo_entero(entero);
			if (ent && pila_apilar(pila, ent)) {
				ok = false;
				continue;
			} else if (ent) free(ent);
			
		}

		if (operacion(parametros[i], pila)) {
			ok = true;
			continue;
		}
		
		ok = false;
		break;
	}
	free_strv(parametros);
	return ok;
}

void leer_y_aplicar(pila_t* pila) {
	/*	Lee por stdin linea a linea y le aplica dc a cada linea, si se
	 *	realizo correctamente la operacion de la linea imprime el resultado
	 *	sino imprime ERROR.
	 */
	char* linea = NULL;
	size_t cap_linea = 0;
	long tam_linea;
	bool ok = true;

	while((tam_linea = getline(&linea, &cap_linea, stdin)) > 0) {
		if (strcmp(&linea[tam_linea - 1], "\n") == 0) linea[tam_linea - 1] = '\0';

		if (ok) ok = dc(pila, linea);		

		if (ok) {
			int* res = pila_desapilar(pila);
			if (res && pila_esta_vacia(pila)) printf("%d\n", *res);
			else ok = false;
			free(res);
		}

		if (!ok) printf("ERROR\n");

		while (!pila_esta_vacia(pila)) free(pila_desapilar(pila));
		ok = true;
	}
	free(linea);
}

int main(int argc, char** argv){
	pila_t* pila = pila_crear();
	if (!pila) {
		printf("ERROR\n");
		return 1;
	}

	leer_y_aplicar(pila);

	pila_destruir(pila);
	return 0;
}