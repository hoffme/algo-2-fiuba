#include <stdio.h>
#include <stdlib.h>
#include "counting_sort.h"

#define CANT_PERSONAS 7

typedef struct persona {
	char* nombre;
	int edad;
} persona_t;


int obtener_edad_persona(void* p){
	persona_t* pp = (persona_t*) p;
	return pp->edad;
}

char* obtener_nombre_persona(void* p){
	persona_t* pp = (persona_t*) p;
	return pp->nombre;
}


// Funcion que ordena por counting sort un array de enteros
void ordenar_enteros(){
	int arr[10] = { 6, 8, 1, 40, 1, 22, 23, 7, 11, 99 };

	printf("~~~ Ordenando arreglo de enteros ~~~\n");
	printf("Arreglo original:");
	for (int i=0; i<10; ++i){
		printf("%*d", 3, arr[i]);
	}
	printf("\n");

	counting_sort_ints(arr, 10);

	printf("Arreglo ordenado:");
	for (int i=0; i<10; ++i){
		printf("%*d", 3, arr[i]);
	}
	printf("\n");
}

// Funcion que ordena por counting sort un structs persona, por edad
void ordenar_structs(){
	persona_t* personas[CANT_PERSONAS];
	for (int i=0; i<CANT_PERSONAS; ++i){
		persona_t *p = malloc(sizeof(persona_t));
		personas[i] = p;
	}
	personas[0]->nombre = "Marga"; personas[0]->edad = 42;
	personas[1]->nombre = "Gaston"; personas[1]->edad = 28;
	personas[2]->nombre = "Martin"; personas[2]->edad = 27;
	personas[3]->nombre = "Pablo"; personas[3]->edad = 45;
	personas[4]->nombre = "Federico"; personas[4]->edad = 27;
	personas[5]->nombre = "Nacho"; personas[5]->edad = 28;
	personas[6]->nombre = "Bruno"; personas[6]->edad = 29;

	printf("~~~ Ordenando personas por edad ~~~\n");
	printf("Arreglo original:");
	for (int i=0; i<CANT_PERSONAS; ++i){
		printf("%*s [%*d]", 10, personas[i]->nombre, 3, personas[i]->edad);
	}
	printf("\n");

	counting_sort_generalizado((void**) personas, CANT_PERSONAS, obtener_edad_persona);

	printf("Arreglo ordenado:");
	for (int i=0; i<CANT_PERSONAS; ++i){
		printf("%*s [%*d]", 10, personas[i]->nombre, 3, personas[i]->edad);
	}
	printf("\n");

	for (int i=0; i<CANT_PERSONAS; ++i){
		free(personas[i]);
	}
}


int main(void){
	ordenar_enteros();
	printf("\n");
	ordenar_structs();
	return 0;
}
