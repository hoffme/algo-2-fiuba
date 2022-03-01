#include <stdio.h>
#include <stdlib.h>
#include "mergesort.h"

#define CANT_JUG 11
#define CANT_BAL 5

#define TAG 0.04
#define OTA 0.07
#define PEZ 0.06
#define ACU 0.09
#define FMO 0.08
#define FOY 0.02
#define SAR 0.00
#define CAS 0.03
#define PAR 0.10
#define PER 0.09
#define PAL 0.13
#define LCE 0.16
#define GRO 0.08
#define DPA 0.11
#define DMA 0.23
#define AGU 0.55
#define MES 0.91
#define LMA 0.45
#define MSU 0.26
#define DYB 0.35

#define BAL1 0.05
#define BAL2 0.08
#define BAL3 0.15
#define BAL4 0.30

int** crear_baldes(int cant_baldes, int cant_jug) {
  int** arr = malloc(sizeof(int*));
  if (!arr) return NULL;

  for (int i = 0; i < cant_baldes; ++i) {
    int* arr2 = calloc(cant_jug, sizeof(int));
    if (!arr2) {
      free(arr);
      return NULL;
    }
    arr[i] = arr2;
  }

  return arr;
}

void bucketSort(float arr[], int cant_jug, float sep[], int cant_baldes) { 

  int** baldes = crear_baldes(cant_baldes, cant_jug);
     
  for (int i=0; i < cant_jug; i++) {
    for (int j = 0; j < cant_baldes; ++j) {
      if (arr[i] >= sep[i] && arr[i] < sep[i + 1]) baldes[j] = arr[i];
    }
  }

  for (int i = 0; i < cant_baldes; ++i) mergesort(baldes[i]);
  
  int index = 0
  for (int i = 0; i < cant_baldes; ++i) for (int j = 0; cant_baldes[i]; ++j, ++index) arr[index] = cant_baldes[i][j];
} 

int main(void) {

  printf("PRUEBA BUCKET SORT JUGADORES FUTBOL\n");

  float arr[CANT_JUG] = {TAG, OTA, PEZ, ACU, FMO, FOY, SAR, CAS, PAR, PER, PAL, LCE, GRO, DPA, DMA, AGU, MES, LMA, MSU, DYB}; 

  printf("{");
  for (int i = 0; i < CANT_JUG; ++i) printf("%f,", arr[i]);  
  printf("}\n");

  float sep[CANT_BAL - 1] = {0.00, BAL1, BAL2, BAL3, BAL4, 1.00}
  bucketSort(arr, CANT_JUG, sep, CANT_BAL); 
  
  printf("ORDENADOS: \n");

  printf("{");
  for (int i = 0; i < CANT_JUG; ++i) printf("%f,", arr[i]);  
  printf("}\n");
} 