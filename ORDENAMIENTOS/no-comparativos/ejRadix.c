#include <stdio.h>
#include <stdlib.h>
#include "counting_sort.h"

#define CANT_COLORES 5

typedef struct rgb_color {
	int red;
	int green;
	int blue;
} rgb_color_t;

void imprimir_color(rgb_color_t* color){
	printf(" (%*d,%*d,%*d)", 3, color->red, 3, color->green, 3, color->blue);
}

void imprimir_arreglo_colores(rgb_color_t** colores, size_t cant, char* mensaje){
	printf("%s", mensaje);
	for (int i=0; i<cant; ++i){
		imprimir_color(colores[i]);
	}
	printf("\n");
}


/*******************************
 * Getters de campos
 *******************************/
int obtener_red(void* c){
	rgb_color_t* color = c;
	return color->red;
}

int obtener_green(void* c){
	rgb_color_t* color = c;
	return color->green;
}

int obtener_blue(void* c){
	rgb_color_t* color = c;
	return color->blue;
}


/*
 * Funcion que ordena por radix sort, colores rgb por criterio R -> G -> B,
 * usando counting sort como ordenamiento interno
*/
void ordenar_colores_rgb(){
	rgb_color_t* colores[CANT_COLORES];
	for (int i=0; i<CANT_COLORES; ++i){
		colores[i] = malloc(sizeof(rgb_color_t));
	}
	colores[0]->red = 1; colores[0]->green = 127; colores[0]->blue = 127;
	colores[1]->red = 0; colores[1]->green = 127; colores[1]->blue = 255;
	colores[2]->red = 255; colores[2]->green = 255; colores[2]->blue = 255;
	colores[3]->red = 0; colores[3]->green = 0; colores[3]->blue = 1;
	colores[4]->red = 0; colores[4]->green = 255; colores[4]->blue = 0;

	imprimir_arreglo_colores(colores, CANT_COLORES, "Arreglo original:");

	counting_sort_generalizado((void**) colores, CANT_COLORES, obtener_blue);
	imprimir_arreglo_colores(colores, CANT_COLORES, "Arreglo ordenado por azul:");

	counting_sort_generalizado((void**) colores, CANT_COLORES, obtener_green);
	imprimir_arreglo_colores(colores, CANT_COLORES, "Arreglo ordenado por verde:");

	counting_sort_generalizado((void**) colores, CANT_COLORES, obtener_red);
	imprimir_arreglo_colores(colores, CANT_COLORES, "Arreglo ordenado por rojo:");

	for (int i=0; i<CANT_COLORES; ++i){
		free(colores[i]);
	}
}


int main(void){
	ordenar_colores_rgb();
	return 0;
}
