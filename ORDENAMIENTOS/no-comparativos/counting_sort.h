#ifndef ORDENAMIENTOS_NOCOMP_H
#define ORDENAMIENTOS_NOCOMP_H

// Funcion que devuelve el valor entero del elemento por el cual se desea ordenar
typedef int (*getter_campo_t) (void*);


// Recibe un arreglo de enteros y su largo, y lo ordena utilizando counting sort
void counting_sort_ints(int* arr, int n);

// Recibe un arreglo de elementos, su largo y una funcion que devuelve el valor
// en int del valor por el cual se quiere ordenar a los elementos, y lo ordena
// utilizando counting sort
void counting_sort_generalizado(void** arr, int n, getter_campo_t obtener_clave);



#endif // ORDENAMIENTOS_NOCOMP_H
