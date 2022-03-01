#include "counting_sort.h"
#include <stdlib.h>

void counting_sort_ints(int* arr, int n){
	if (n<=0) return;

	// Busco el minimo y el maximo, y calculo el rango
	int max = arr[0], min = arr[0];
	for (int i=0; i<n; ++i){
		if (arr[i]>max) max = arr[i];
		if (arr[i]<min) min = arr[i];
	}
	int k = max - min;

	// Creo un arreglo contador y lo inicializo en 0.
	int contador[k+1];
	for (int i=0; i<k; ++i){
		contador[i] = 0;
	}

	// Cuento las apariciones de cada numero.
	for (int i=0; i<n; ++i){
		contador[arr[i]-min]++;
	}

	// Creo el arreglo de sumas acumuladas
	int suma_acumulada[k+1];
	suma_acumulada[0] = 0;
	for (int i=1; i<k+1; ++i){
		suma_acumulada[i] = suma_acumulada[i-1] + contador[i-1];
	}

	// Creo el arreglo que contendra los elementos ordenados
	int* result = malloc(n * sizeof(int));
	for (int i=0; i<n; ++i){
		int valor = arr[i];
		int pos = suma_acumulada[valor - min];
		suma_acumulada[valor - min]++;
		result[pos] = valor;
	}

	// Copio al arreglo original
	for (int i=0; i<n; ++i){
		arr[i] = result[i];
	}

	free(result);
}


void counting_sort_generalizado(void** arr, int n, getter_campo_t obtener_clave){
	if (n<=0) return;

	// Busco el minimo y el maximo, y calculo el rango
	int max = obtener_clave(arr[0]);
	int min = max;
	for (int i=0; i<n; ++i){
		int valor = obtener_clave(arr[i]);
		if (valor>max) max = valor;
		if (valor<min) min = valor;
	}
	int k = max - min;

	// Creo un arreglo contador y lo inicializo en 0.
	int contador[k+1];
	for (int i=0; i<k; ++i){
		contador[i] = 0;
	}

	// Cuento las apariciones de cada numero.
	for (int i=0; i<n; ++i){
		contador[obtener_clave(arr[i])-min]++;
	}

	// Creo el arreglo de sumas acumuladas
	int suma_acumulada[k+1];
	suma_acumulada[0] = 0;
	for (int i=1; i<k+1; ++i){
		suma_acumulada[i] = suma_acumulada[i-1] + contador[i-1];
	}

	// Creo el arreglo que contendra los elementos ordenados
	void** result = malloc(n * sizeof(void*));
	for (int i=0; i<n; ++i){
		int valor = obtener_clave(arr[i]);
		int pos = suma_acumulada[valor - min];
		suma_acumulada[valor - min]++;
		result[pos] = arr[i];
	}

	// Copio al arreglo original
	for (int i=0; i<n; ++i){
		arr[i] = result[i];
	}

	free(result);
}
