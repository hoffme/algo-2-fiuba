#include "pila.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

long* nuevo_entero(long entero) {
	long* ent = malloc(sizeof(entero));
	if (!ent) return NULL;
	*ent = entero;
	return ent;
}

long _log(long base, long argumento) {
	long div = argumento / base;
	if (div == 1) return 1;
	else if (div == 0) return 0;

	return 1 + _log(base, div);
}

long _potencia(long numero, long exp) {
	if (exp == 0) return 1;
	
	if (exp % 2 == 0) {
		long pot_men = _potencia(numero, exp / 2);
		return pot_men * pot_men;
	} else {
		long pot_men = _potencia(numero, (exp - 1) / 2);
		return pot_men * pot_men * numero;
	}
}

long _raiz(long numero, long men, long may) {
	if (may - men == 1 && may * may > numero) return men;
	else if (may - men == 1) return may;
	
	long med = (may + men) / 2;
	if (med * med < numero) return _raiz(numero, med, may);
	else if (med * med > numero) return _raiz(numero, men, med);
	else return med;
}

bool sumar_pila(pila_t* pila) {
	long* entero1 = pila_desapilar(pila);
	if (!entero1) return false;

	long* entero2 = pila_desapilar(pila);
	if (!entero2) {
		pila_apilar(pila, entero1);
		return false;
	}

	long* res = nuevo_entero(*entero1 + *entero2);
	if (!res) {
		pila_apilar(pila, entero1);
		pila_apilar(pila, entero2);
		return false;
	}

	free(entero1);
	free(entero2);

	pila_apilar(pila, res);
	return true;
}

bool restar_pila(pila_t* pila) {
	long* entero1 = pila_desapilar(pila);
	if (!entero1) return false;

	long* entero2 = pila_desapilar(pila);
	if (!entero2) {
		pila_apilar(pila, entero1);
		return false;
	}
	
	long* res = nuevo_entero(*entero1 - *entero2);
	if (!res) {
		pila_apilar(pila, entero1);
		pila_apilar(pila, entero2);
		return false;
	}
	
	free(entero1);
	free(entero2);

	pila_apilar(pila, res);
	return true;
}

bool multiplicar_pila(pila_t* pila) {
	long* entero1 = pila_desapilar(pila);
	if (!entero1) return false;

	long* entero2 = pila_desapilar(pila);
	if (!entero2) {
		pila_apilar(pila, entero1);
		return false;
	}
	
	long* res = nuevo_entero(*entero1 * *entero2);
	if (!res) {
		pila_apilar(pila, entero1);
		pila_apilar(pila, entero2);
		return false;
	}
	
	free(entero1);
	free(entero2);

	pila_apilar(pila, res);
	return true;
}

bool dividir_pila(pila_t* pila) {
	long* entero1 = pila_desapilar(pila);
	if (!entero1) return false;

	long* entero2 = pila_desapilar(pila);
	if (!entero2) {
		pila_apilar(pila, entero1);
		return false;
	}

	if (*entero2 == 0) {		
		pila_apilar(pila, entero1);
		pila_apilar(pila, entero2);
		return false;
	}
	
	long* res = nuevo_entero(*entero1 / *entero2);
	if (!res) {
		pila_apilar(pila, entero1);
		pila_apilar(pila, entero2);
		return false;
	}
	
	free(entero1);
	free(entero2);

	pila_apilar(pila, res);
	return true;
}

bool raiz_pila(pila_t* pila) {
	long* entero = pila_desapilar(pila);
	if (!entero) return false;

	if (*entero < 0) {
		pila_apilar(pila, entero);
		return false;
	}

	long* res = nuevo_entero(_raiz(*entero, 0, *entero));
	if (!res) {
		pila_apilar(pila, entero);
		return false;
	}
	
	free(entero);

	pila_apilar(pila, res);
	return true;
}

bool potencia_pila(pila_t* pila) {
	long* entero1 = pila_desapilar(pila);
	if (!entero1) return false;

	long* entero2 = pila_desapilar(pila);
	if (!entero2) {
		pila_apilar(pila, entero1);
		return false;
	}

	if (*entero2 < 0) {
		pila_apilar(pila, entero1);
		pila_apilar(pila, entero2);
		return false;
	}
	
	long* res = nuevo_entero(_potencia(*entero1, *entero2));
	if (!res) {
		pila_apilar(pila, entero1);
		pila_apilar(pila, entero2);
		return false;
	}
	
	free(entero1);
	free(entero2);

	pila_apilar(pila, res);
	return true;
}

bool logaritmo_pila(pila_t* pila) {
	long* entero1 = pila_desapilar(pila);
	if (!entero1) return false;

	long* entero2 = pila_desapilar(pila);
	if (!entero2) {
		pila_apilar(pila, entero1);
		return false;
	}

	if (*entero2 <= 1) {
		pila_apilar(pila, entero1);
		pila_apilar(pila, entero2);
		return false;
	}

	long* res = nuevo_entero(_log(*entero2, *entero1));
	if (!res) {
		pila_apilar(pila, entero1);
		pila_apilar(pila, entero2);
		return false;
	}
	
	free(entero1);
	free(entero2);

	pila_apilar(pila, res);
	return true;
}

bool ternario_pila(pila_t* pila) {
	long* entero1 = pila_desapilar(pila);
	if (!entero1) return false;

	long* entero2 = pila_desapilar(pila);
	if (!entero2) {
		pila_apilar(pila, entero1);
		return false;
	}

	long* entero3 = pila_desapilar(pila);
	if (!entero3) {
		pila_apilar(pila, entero2);
		pila_apilar(pila, entero1);
		return false;
	}

	if (*entero1 != 0) {
		long* aux = entero2;
		entero2 = entero3;
		entero3 = aux;
	}

	free(entero2);
	free(entero1);

	pila_apilar(pila, entero3);
	return true;
}