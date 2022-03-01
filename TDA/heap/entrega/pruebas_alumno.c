#include "heap.h"
#include "testing.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*                  FUNCIONES DE COMPARACION                    */

int wrp_num(const void* var1, const void* var2) {
    if (*((int*)var1) > *((int*)var2)) return 1;
    if (*((int*)var1) < *((int*)var2)) return -1;
    else return 0;
}

int wrp_char(const void* var1, const void* var2) {
    if (!var1 && !var2) return 1;
    else if (!var1) return -1;
    else if (!var2) return 1;
    return strcmp((char*)var1, (char*)var2);
}

/*                  FUNCIONES DE AUXILIARES                    */

char* aleatoria_pal(size_t tam) {
    char* res = malloc(sizeof(char) * (tam + 1));
    if (!res) return NULL;

    for (int i = 0; i < tam; ++i) res[i] = (char)(rand() % 128);
    res[tam] = '\0';

    return res;
}

char* substr(const char *str, size_t n) {
    char* res = calloc(n + 1, sizeof(char));
    if (!res) return NULL;
    strncpy(res, str, n);
    return res;
}

void free_strv(char *strv[]) {
    for (int i = 0; strv[i]; ++i) free(strv[i]);
    free(strv);
}

void* pedir_memoria(int tam_campo, size_t cant) {
    return malloc(tam_campo * cant);
}

char** split(const char *str, char sep) {
    size_t cant_sep = 0;
    size_t pos_res_act = 0;
    size_t ini_pal = 0;
    size_t largo;

    for (largo = 0; str[largo] != '\0'; ++largo) if (str[largo] == sep) ++cant_sep;

    char** res = (char**)pedir_memoria(sizeof(char*), cant_sep + 2); // + 2 uno del null y el otro de la primer cadena
    if (!res) return NULL;
    
    for (int pos_act = 0; pos_act <= largo; ++pos_act) {
        if (str[pos_act] != '\0' && str[pos_act] != sep) continue;

        char* pal = substr(&str[ini_pal], pos_act - ini_pal);
        if (!pal) {
            res[pos_res_act] = NULL;
            free_strv(res);
            return NULL;
        }

        res[pos_res_act] = pal;

        ini_pal = pos_act + 1;
        ++pos_res_act;
    }   
    res[pos_res_act] = NULL;

    return res;
}

/*                  FUNCIONES DE PRUEBA                    */

void pruebas_heap_vacio(void){
    printf("----------                PRUEBAS HEAP VACIA                ----------\n");

    heap_t* heap = heap_crear(wrp_num);
    print_test("creacion del heap", !!heap);
   
    print_test("verificacion heap vacio", heap_esta_vacio(heap));
    print_test("ver primero (vacio -> nulo)", !heap_ver_max(heap));
    print_test("desencolar (vacio -> nulo)", !heap_desencolar(heap));
    print_test("cantidad: 0", heap_cantidad(heap) == 0);

    heap_destruir(heap, NULL);
    print_test("destruccion heap", true);
}

void pruebas_heap_destruir_con_ele(void){
    printf("----------       PRUEBAS HEAP ELIMINAR CON ELEMENTOS       ----------\n");

    int entero = 72;
    int entero1 = 55;
    int entero2 = 102;

    heap_t* heap = heap_crear(wrp_num);
    print_test("creacion del heap de numeros", !!heap);
    print_test("cantidad: 0", heap_cantidad(heap) == 0);

    print_test("encolando entero", heap_encolar(heap, &entero));
    print_test("cantidad: 1", heap_cantidad(heap) == 1);

    print_test("encolando entero1", heap_encolar(heap, &entero1));
    print_test("cantidad: 2", heap_cantidad(heap) == 2);

    print_test("maximo", heap_ver_max(heap) == &entero);

    print_test("encolando entero2", heap_encolar(heap, &entero2));
    print_test("cantidad: 3", heap_cantidad(heap) == 3);

    print_test("maximo", heap_ver_max(heap) == &entero2);

    print_test("verificacion heap no vacio", !heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
    print_test("destruccion heap de numeros", true);
}

void pruebas_heap_un_ele(void){
    printf("----------         PRUEBAS HEAP UNICO ELEMENTO          ----------\n");

    char* pal = "hola";

    heap_t* heap = heap_crear(wrp_char);
    print_test("creacion del heap de char", !!heap);

    print_test("encolando pal", heap_encolar(heap, &pal));
    print_test("cantidad: 1", heap_cantidad(heap) == 1);
    print_test("ver max (char)", heap_ver_max(heap) == &pal);
    print_test("verificacion heap no vacio", !heap_esta_vacio(heap));

    print_test("desencolar pal", heap_desencolar(heap) == &pal);
    print_test("cantidad: 0", heap_cantidad(heap) == 0);
    print_test("ver max (vacio -> nulo)", !heap_ver_max(heap));
    print_test("desencolar (vacio -> nulo)", !heap_desencolar(heap));

    heap_destruir(heap, NULL);
    print_test("destruccion heap de char", true);
}

void pruebas_heap_nulo(void){
    printf("----------       PRUEBAS HEAP ENCOLAR Y DESENCOLAR NULOS      ----------\n");

    heap_t* heap = heap_crear(wrp_char);
    print_test("creacion de la heap", !!heap);

    print_test("encolando nulo", !heap_encolar(heap, NULL));
    print_test("heap con nulo ver primero", !heap_ver_max(heap));
    print_test("verificacion heap no vacio", heap_esta_vacio(heap));
    print_test("desencolar heap es igual a nulo", !heap_desencolar(heap));

    heap_destruir(heap, NULL);
    print_test("destruccion heap", true);
}

void pruebas_heap_volumen(int largo){
    printf("----------       PRUEBAS HEAP VOLUMEN     ----------\n");

    heap_t* heap = heap_crear(wrp_char);
    print_test("creacion del heap", !!heap);

    const size_t largo_clave = 11;
    char* claves[largo];

    bool ok = true;
    for (int i = 0; i < largo ; i++) {
        claves[i] = malloc(sizeof(char) * largo_clave);
        sprintf(claves[i], "%08d", i);
        ok = heap_encolar(heap, claves[i]);
        if (!ok) break;
    }

    print_test("heap encolar muchos elementos", ok);
    print_test("heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);

    for (int i = (largo - 1); i >= 0; i--) {
        ok = heap_desencolar(heap) == claves[i];
        if (!ok) break;
    }

    print_test("heap desencolar muchos elementos", ok);
    print_test("heap la cantidad de elementos es correcta", heap_cantidad(heap) == 0);

    heap_destruir(heap, NULL);
    heap = heap_crear(wrp_char);

    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = heap_encolar(heap, claves[i]);
        if (!ok) break;
    }

    heap_destruir(heap, free);
    print_test("destruccion heap", true);
}

void pruebas_heap_copiar_arr(void) {
    printf("----------       PRUEBAS HEAP COPIAR     ----------\n");

    size_t tam = 50;
    int* arr[tam];

    for (int i = 0; i < tam; ++i) {
        arr[i] = malloc(sizeof(int));
        *arr[i] = rand()%2000;
    }

    heap_t* heap = heap_crear_arr((void**)arr, tam, wrp_num);

    print_test("creacion del heap de numeros", !!heap);
    print_test("cantidad", heap_cantidad(heap) == tam);
    print_test("verificacion heap no vacio", !heap_esta_vacio(heap));

    bool ok = true;
    int* ant = (int*)heap_desencolar(heap);
    int* act;
    for (int i = 0; i < (tam - 1); ++i) {
        act = (int*)heap_desencolar(heap);
        ok = wrp_num(ant, act) > 0;
        if (!ok) break;
        free(ant);
        ant = act;
    }
    free(ant);

    print_test("orden arreglo", ok);
    print_test("cantidad", heap_cantidad(heap) == 0);
    print_test("verificacion heap no vacio", heap_esta_vacio(heap));

    heap_destruir(heap, free);
    print_test("destruccion heap de numeros", true);
}

void pruebas_heapsort_vacio(void) {
    printf("----------       PRUEBAS HEAPSORT VACIO     ----------\n");

    size_t tam = 0;
    void* arr[tam];

    heap_sort(arr, tam, wrp_num);

    print_test("heapsor vacio", true);
}

void pruebas_heapsort(void) {
    printf("----------       PRUEBAS HEAPSORT    ----------\n");

    size_t tam = 10;
    char** arr = split("voy a escribir palabras desordenadas y las tiene que ordenar",' ');
    char* ordenado[] = {arr[1], arr[4], arr[2], arr[6], arr[9], arr[3], arr[8], arr[7], arr[0], arr[5]};

    heap_sort((void**)arr, tam, wrp_char);

    bool ok = true;
    for (int i = 0; arr[i]; ++i) ok = arr[i] == ordenado[i];

    print_test("heapsort resultado en orden", ok);

    free_strv(arr);
}

void pruebas_heapsort_volumen(size_t largo, size_t largo_pal) {
    printf("----------       PRUEBAS HEAPSORT VOLUMEN    ----------\n");

    char* arr[largo + 1];
    for (int i = 0; i < largo; ++i) arr[i] = aleatoria_pal(largo_pal);
    arr[largo] = NULL;

    heap_sort((void**)arr, largo, wrp_char);

    bool ok = true;
    for (int i = 0; i < largo; ++i) {
        if (i + 1 < largo) ok = wrp_char(arr[i], arr[i + 1]) <= 0;
        else if (i - 1 > 0) ok = wrp_char(arr[i - 1], arr[i]) <= 0;
        if (!ok) break;
    }

    print_test("heapsort resultado en orden", ok);

    for (int i = 0; arr[i]; ++i) free(arr[i]);
}

void pruebas_heap_alumno() {

    pruebas_heap_vacio();
    pruebas_heap_destruir_con_ele();
    pruebas_heap_un_ele();
    pruebas_heap_nulo();
    pruebas_heap_volumen(1000);

    pruebas_heap_copiar_arr();

    pruebas_heapsort_vacio();
    pruebas_heapsort();
    pruebas_heapsort_volumen(1000, 10);

}