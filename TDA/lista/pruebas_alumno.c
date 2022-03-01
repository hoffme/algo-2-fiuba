#include "lista.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------PRUEBAS LISTA FUNCIONES----------------------------------------------

void pruebas_lista_vacia(void){
    printf("----------                PRUEBAS LISTA VACIA                ----------\n");

    lista_t* lista = lista_crear();
    print_test("creacion de la lista", lista != NULL);
    
    print_test("verificacion lista vacia", lista_esta_vacia(lista) == true);
    print_test("lista borrar primero == null", lista_borrar_primero(lista) == NULL);
    print_test("lista ver primero == null", lista_ver_primero(lista) == NULL);
    print_test("lista ver ultimo == null", lista_ver_ultimo(lista) == NULL);
    print_test("lista largo == 0", lista_largo(lista) == 0);

    lista_destruir(lista, NULL);
    print_test("destruccion lista", true);
}

void pruebas_lista_destr_con_ele(void){
    printf("----------       PRUEBAS LISTA ELIMINAR CON ELEMENTOS       ----------\n");

    int entero = 72;
    int entero1 = 55;
    int entero2 = 102;

    void* lista = lista_crear();
    print_test("creacion de la lista", lista != NULL);

    print_test("insertando principio entero", lista_insertar_primero(lista, &entero) == true);
    print_test("insertando final entero1", lista_insertar_ultimo(lista, &entero1) == true);
    print_test("insertando principio entero2", lista_insertar_primero(lista, &entero2) == true);

    print_test("verificacion lista no vacia", lista_esta_vacia(lista) == false);
    print_test("lista ver primero == entero2", lista_ver_primero(lista) == &entero2);
    print_test("lista ver ultimo == entero1", lista_ver_ultimo(lista) == &entero1);
    print_test("lista largo == 3", lista_largo(lista) == 3);

    lista_destruir(lista, NULL);
    print_test("destruccion lista no vacia", true);
}

void pruebas_lista_un_ele_pri(void){
    printf("--------         PRUEBAS LISTA UNICO ELEMENTO PRINCIPIO          ----------\n");

    int entero = 72;

    void* lista = lista_crear();
    print_test("creacion de la lista", lista != NULL);

    print_test("insertando principio entero", lista_insertar_primero(lista, &entero) == true);

    print_test("verificacion lista no vacia", lista_esta_vacia(lista) == false);
    print_test("lista ver primero == entero", lista_ver_primero(lista) == &entero);
    print_test("lista ver ultimo == entero", lista_ver_ultimo(lista) == &entero);
    print_test("lista largo == 1", lista_largo(lista) == 1);

    print_test("lista borrar primero == entero", lista_borrar_primero(lista) == &entero);

    print_test("verificacion lista vacia", lista_esta_vacia(lista) == true);
    print_test("lista borrar primero == null", lista_borrar_primero(lista) == NULL);
    print_test("lista ver primero == null", lista_ver_primero(lista) == NULL);
    print_test("lista ver ultimo == null", lista_ver_ultimo(lista) == NULL);
    print_test("lista largo == 0", lista_largo(lista) == 0);

    lista_destruir(lista, NULL);
    print_test("destruccion lista vacia", true);
}

void pruebas_lista_un_ele_ult(void){
    printf("--------         PRUEBAS LISTA UNICO ELEMENTO ULTIMO          ----------\n");

    int entero = 72;

    void* lista = lista_crear();
    print_test("creacion de la lista", lista != NULL);

    print_test("insertando ultimo entero", lista_insertar_ultimo(lista, &entero) == true);

    print_test("verificacion lista no vacia", lista_esta_vacia(lista) == false);
    print_test("lista ver primero == entero", lista_ver_primero(lista) == &entero);
    print_test("lista ver ultimo == entero", lista_ver_ultimo(lista) == &entero);
    print_test("lista largo == 1", lista_largo(lista) == 1);

    print_test("lista borrar primero == entero", lista_borrar_primero(lista) == &entero);

    print_test("verificacion lista vacia", lista_esta_vacia(lista) == true);
    print_test("lista borrar primero == null", lista_borrar_primero(lista) == NULL);
    print_test("lista ver primero == null", lista_ver_primero(lista) == NULL);
    print_test("lista ver ultimo == null", lista_ver_ultimo(lista) == NULL);
    print_test("lista largo == 0", lista_largo(lista) == 0);

    lista_destruir(lista, NULL);
    print_test("destruccion lista", true);
}

void pruebas_lista_var_ele(void){
    printf("----------PRUEBAS LISTA AGG Y ELI VARIOS ELEMENTOS----------\n");

    char* cadena = "texto de prueba";
    int entero = 4;
    int arreglo[5] = {32, 46, 5, -3, 0};
    double doble = 5.23612;

    void* lista = lista_crear();
    print_test("creacion de la lista", lista != NULL);
    print_test("verificacion lista vacia", lista_esta_vacia(lista) == true);

    print_test("insertando ultimo entero", lista_insertar_ultimo(lista, &entero) == true);
    print_test("lista ver primero == entero", lista_ver_primero(lista) == &entero);
    print_test("lista largo == 1", lista_largo(lista) == 1);

    print_test("insertando principio cadena", lista_insertar_primero(lista, &cadena) == true);
    print_test("lista ver ultimo == entero", lista_ver_ultimo(lista) == &entero);

    print_test("insertando principio arreglo", lista_insertar_primero(lista, &arreglo) == true);
    print_test("lista ver primero == arreglo", lista_ver_primero(lista) == &arreglo);

    print_test("lista borrar primero == arreglo", lista_borrar_primero(lista) == &arreglo);
    print_test("lista largo == 2", lista_largo(lista) == 2);

    print_test("insertando ultimo doble", lista_insertar_ultimo(lista, &doble) == true);
    print_test("lista largo == 3", lista_largo(lista) == 3);

    print_test("lista borrar primero == cadena", lista_borrar_primero(lista) == &cadena);
    print_test("lista borrar primero == entero", lista_borrar_primero(lista) == &entero);
    
    print_test("lista ver ultimo == doble", lista_ver_ultimo(lista) == &doble);
    print_test("lista ver primero == doble", lista_ver_primero(lista) == &doble);
    print_test("verificacion lista no vacia", lista_esta_vacia(lista) == false);

    print_test("lista borrar primero == doble", lista_borrar_primero(lista) == &doble);
    
    print_test("verificacion lista vacia", lista_esta_vacia(lista) == true);
    print_test("lista ver primero == null", lista_ver_primero(lista) == NULL);
    print_test("lista ver ultimo == null", lista_ver_ultimo(lista) == NULL);
    print_test("lista largo == 0", lista_largo(lista) == 0);
    
    lista_destruir(lista, NULL);
    print_test("destruccion lista", true);
}

void pruebas_lista_nulo(void){
    printf("----------       PRUEBAS LISTA CON NULOS      ----------\n");

    void* lista = lista_crear();
    print_test("creacion de la lista", lista != NULL);

    print_test("insertando ultimo null", lista_insertar_ultimo(lista, NULL) == true);

    print_test("lista ver primero == null", lista_ver_primero(lista) == NULL);
    print_test("lista ver ultimo == null", lista_ver_ultimo(lista) == NULL);
    print_test("verificacion lista no vacia", lista_esta_vacia(lista) == false);
    print_test("lista largo == 1", lista_largo(lista) == 1);

    print_test("insertando principio nulo", lista_insertar_primero(lista, NULL) == true);

    print_test("lista borrar primero == null", lista_borrar_primero(lista) == NULL);
    print_test("lista largo == 1", lista_largo(lista) == 1);

    print_test("lista borrar primero == null", lista_borrar_primero(lista) == NULL);
    print_test("verificacion lista vacia", lista_esta_vacia(lista) == true);
    print_test("lista ver primero == null", lista_ver_primero(lista) == NULL);
    print_test("lista ver ultimo == null", lista_ver_ultimo(lista) == NULL);

    lista_destruir(lista, NULL);
    print_test("destruccion lista", true);
}

void pruebas_lista_comple_ele(void){
    printf("----------       PRUEBAS LISTA STRUCTS      ----------\n");

    typedef struct estructura{
        int entero;
        double doble;
        char cadena[25];
    } estructura_t;

    estructura_t* estructura1;
    estructura_t* estructura2;

    void* lista = lista_crear();
    print_test("creacion de la lista", lista != NULL);

    print_test("insertando ultimo estructura1", lista_insertar_ultimo(lista, &estructura1) == true);
    print_test("lista ver primero == estructura1", lista_ver_primero(lista) == &estructura1);
    print_test("lista largo == 1", lista_largo(lista) == 1);

    print_test("insertando ultimo estructura2", lista_insertar_ultimo(lista, &estructura2) == true);    
    print_test("lista ver ultimo == estructura2", lista_ver_ultimo(lista) == &estructura2);

    print_test("lista borrar primero == estructura1", lista_borrar_primero(lista) == &estructura1);
    print_test("verificacion lista no vacia", lista_esta_vacia(lista) == false);

    print_test("lista borrar primero == estructura2", lista_borrar_primero(lista) == &estructura2);
    print_test("verificacion lista vacia", lista_esta_vacia(lista) == true);
    print_test("lista ver primero == null", lista_ver_primero(lista) == NULL);
    print_test("lista ver ultimo == null", lista_ver_ultimo(lista) == NULL);

    lista_destruir(lista, NULL);
    print_test("destruccion lista", true);
}

void pruebas_lista_comple_ele_heap(void){
    printf("----------       PRUEBAS LISTA STRUCTS HEAP      ----------\n");

    typedef struct estructura{
        int entero;
        double doble;
        char cadena[25];
    } estructura_t;

    estructura_t* estructura1 = malloc(sizeof(estructura_t));
    estructura_t* estructura2 = malloc(sizeof(estructura_t));

    void* lista = lista_crear();
    print_test("creacion de la lista", lista != NULL);

    print_test("insertando ultimo estructura1", lista_insertar_ultimo(lista, estructura1) == true);
    print_test("lista ver primero == estructura1", lista_ver_primero(lista) == estructura1);
    print_test("lista largo == 1", lista_largo(lista) == 1);

    print_test("insertando ultimo estructura2", lista_insertar_ultimo(lista, estructura2) == true);    
    print_test("lista ver ultimo == estructura2", lista_ver_ultimo(lista) == estructura2);
    print_test("lista largo == 2", lista_largo(lista) == 2);

    estructura_t* estructura1_recibida = lista_borrar_primero(lista);
    print_test("lista borrar primero == estructura1", estructura1_recibida == estructura1);
    free(estructura1_recibida);
    print_test("verificacion lista no vacia", lista_esta_vacia(lista) == false);

    estructura_t* estructura2_recibida = lista_borrar_primero(lista);
    print_test("lista borrar primero == estructura2", estructura2_recibida == estructura2);
    free(estructura2_recibida);
    print_test("verificacion lista vacia", lista_esta_vacia(lista) == true);
    print_test("lista ver primero == null", lista_ver_primero(lista) == NULL);
    print_test("lista ver ultimo == null", lista_ver_ultimo(lista) == NULL);

    lista_destruir(lista, NULL);
    print_test("destruccion lista", true);
}

void pruebas_lista_destr_con_comple_ele_heap(void){
    printf("----------       PRUEBAS LISTA STRUCTS HEAP SIN BORRAR      ----------\n");

    typedef struct estructura{
        int entero;
        double doble;
        char cadena[25];
    } estructura_t;

    estructura_t* estructura1 = malloc(sizeof(estructura_t));
    estructura_t* estructura2 = malloc(sizeof(estructura_t));

    void* lista = lista_crear();
    print_test("creacion de la lista", lista != NULL);

    print_test("insertando ultimo estructura1", lista_insertar_ultimo(lista, estructura1) == true);
    print_test("lista ver primero == estructura1", lista_ver_primero(lista) == estructura1);
    print_test("lista largo == 1", lista_largo(lista) == 1);

    print_test("insertando ultimo estructura2", lista_insertar_ultimo(lista, estructura2) == true);    
    print_test("lista ver ultimo == estructura2", lista_ver_ultimo(lista) == estructura2);
    print_test("lista largo == 2", lista_largo(lista) == 2);

    lista_destruir(lista, free);
    print_test("destruccion lista", true);
}

void pruebas_lista_volumen(void){
    printf("----------               PRUEBAS GRANDES VOLUMENES           ----------\n");

    void* lista = lista_crear();
    print_test("creacion de la lista", lista != NULL);

    int entero = 0;

    int max = 10000;
    bool ok = true;
    for (int i = 0; i < max; i++) {
        ok &= lista_insertar_primero(lista, &entero);
    }
    print_test("se pudieron agregar todos los elementos al principio", ok);
    print_test("lista largo == 10000", lista_largo(lista) == 10000);
    print_test("verificacion lista no vacia", lista_esta_vacia(lista) == false);

    ok = true;
    for (int i = 0; i < max; i++) {
        ok &= lista_insertar_ultimo(lista, &entero);
    }
    print_test("se pudieron agregar todos los elementos alo ultimo", ok);
    print_test("lista largo == 20000", lista_largo(lista) == 20000);

    max = 20000;
    ok = true;
    for (int i = 0; i < max; i++) {
        int* valor = lista_borrar_primero(lista);
        ok &= (valor == &entero);
    }
    print_test("se pudieron borrar todos los elementos", ok);

    print_test("verificacion lista vacia", lista_esta_vacia(lista) == true);
    print_test("lista ver primero == null", lista_ver_primero(lista) == NULL);
    print_test("lista ver ultimo == null", lista_ver_ultimo(lista) == NULL);
    print_test("lista largo == 0", lista_largo(lista) == 0);

    lista_destruir(lista, NULL);
    print_test("destruccion lista", true);
}

// ----------------------------------------------PRUEBAS ITERADOR EXTERNO FUNCIONES-----------------------------------------

void iter_ext_vacia(lista_t* lista) {
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("iterador externo creado", iter);
    int cont = 0;
    while (!lista_iter_al_final(iter)) {
        cont++;
        lista_iter_avanzar(iter);
    }
    print_test("iterador externo lista vacia", cont == 0);
    print_test("iterador ver actual lista vacia = null", lista_iter_ver_actual(iter) == NULL);
    print_test("iterador borrar lista vacia = null", lista_iter_borrar(iter) == NULL);
    
    lista_iter_destruir(iter);
    print_test("iterador externo destruido", true);
}

void iter_ext_insert_vacio(lista_t* lista, int cant) {
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("iterador externo creado", iter);
    
    char* prod = "producto"; 

    for (int i = 0; i < cant; ++i) lista_iter_insertar(iter, prod);

    print_test("insertado todos los elemntos", lista_largo(lista) == cant);

    lista_iter_destruir(iter);
    print_test("iterador externo destruido", true);
}

void iter_ext_iter_lim(lista_t* lista,int max_iter) {
    lista_iter_t *iter = lista_iter_crear(lista);

    int cont = 0;
    while (!lista_iter_al_final(iter) && cont < max_iter) {
        cont++;
        lista_iter_avanzar(iter);
    }    
    print_test("iterador externo lista itera hasta limite o final", (cont == max_iter) || lista_iter_al_final(iter));

    lista_iter_destruir(iter);
}

void iter_ext_bord_borr(lista_t* lista, int cant_borr, int pos_ini_borr) {
    char* primero = "prod prim";
    char* ultimo = "prod ulti";

    print_test("insertando ultimo", lista_insertar_ultimo(lista, primero) == true);
    print_test("insertando principio", lista_insertar_primero(lista, ultimo) == true);
    
    lista_iter_t *iter = lista_iter_crear(lista);
    
    char* pro_pri = lista_iter_borrar(iter);
    print_test("borrar extremo_principal", *pro_pri == *primero);

    int cant_lista = (int)lista_largo(lista);

    for (int i = 0; i < (cant_lista - 1); ++i){
        if (i < (cant_borr + pos_ini_borr) && i >= pos_ini_borr) lista_iter_borrar(iter);
        else lista_iter_avanzar(iter);
    }

    print_test("borrar en el medio", true);
    print_test("lista largo", lista_largo(lista) == (cant_lista - cant_borr));
    print_test("iter no al final", !lista_iter_al_final(iter));

    char* pro_ult = lista_iter_borrar(iter);
    print_test("borrar extremo final", *pro_ult == *ultimo);
    print_test("iter al final", lista_iter_al_final(iter));

    lista_iter_destruir(iter);
}

void iter_ext_bord_inse(lista_t* lista, int cant_insert, int pos_ini_inser) {
    lista_iter_t *iter = lista_iter_crear(lista);
    
    char* primero = "prod prim";
    char* medio = "prod med";
    char* ultimo = "prod ulti";
    
    print_test("iterador lista insertar primero", lista_iter_insertar(iter, primero));
    char* pro_pri = lista_iter_ver_actual(iter);
    print_test("ver actual = (recien insertado)", *pro_pri == *primero);

    int cant_lista = (int)lista_largo(lista);

    for (int i = 0; i < (cant_lista + cant_insert - 1); ++i) {
        if (i < (cant_insert + pos_ini_inser) && i >= pos_ini_inser) {
            print_test("iterador lista insertar", lista_iter_insertar(iter, medio));
            print_test("ver actual = (recien insertado)", *(char*)lista_iter_ver_actual(iter) == *medio);
        }
        lista_iter_avanzar(iter);
    }

    print_test("lista largo", lista_largo(lista) == (cant_lista + cant_insert));
    print_test("iter no al final", !lista_iter_al_final(iter));

    print_test("iterador lista insertar ultimo", lista_iter_insertar(iter, ultimo));
    char* pro_ult = lista_iter_ver_actual(iter);
    print_test("ver actual = (recien insertado)", *pro_ult == *ultimo);
    print_test("iter no al final", !lista_iter_al_final(iter));

    int cont = 0;
    while (!lista_iter_al_final(iter)) {
        lista_iter_avanzar(iter);
        cont++;
    }

    print_test("pos restantes = 2", cont == 2);

    lista_iter_destruir(iter);
}

// ----------------------------------------------PRUEBAS ITERADOR EXTERNO MAIN----------------------------------------------

void pruebas_iter_ext(void) {
    printf("----------               PRUEBAS ITERADORES EXTERNOS           ----------\n");

    lista_t *super = lista_crear();

    int cant_insertar = 50;    
    int lim_iter = 10;
    int cant_borr_med = 5;
    int pos_ini_borr_med = 5;    
    int cant_inser_med = 5;
    int pos_ini_inser_med = 5;

    printf("PRUEBA VACIO\n");
    iter_ext_vacia(super);

    printf("PRUEBA INSERTAR VACIO\n");
    iter_ext_insert_vacio(super, cant_insertar);

    printf("PRUEBA ITERAR HASTA LIMITE\n");
    iter_ext_iter_lim(super, lim_iter);

    printf("PRUEBA BORRAR CASOS BORDES\n");
    iter_ext_bord_borr(super, cant_borr_med, pos_ini_borr_med);

    printf("PRUEBA INSERTAR CASOS BORDES\n");
    iter_ext_bord_inse(super, cant_inser_med, pos_ini_inser_med);

    lista_destruir(super, NULL);    
}

// ----------------------------------------------PRUEBAS ITERADOR INTERNO FUNCIONES------------------------------------------

bool iter_int_imprim(void* data, void* extra) {
    char texto[30] = "entre y lei ";
    strcat(texto, data);
    print_test(texto, *(bool*)extra);
    return true;
}

bool iter_int_cont(void* data, void* cont) {
    (*(int*)cont)++; 
    return true;
}

bool iter_int_cont_lim(void* data, void* arr) {
    ((int*)arr)[0]++;
    if (((int*)arr)[0] >= ((int*)arr)[1]) return false;
    return true;
}

// ----------------------------------------------PRUEBAS ITERADOR INTERNO MAIN----------------------------------------------

void pruebas_iter_int(void) {
    printf("----------               PRUEBAS ITERADOR INTERNO           ----------\n");

    lista_t *super = lista_crear();

    bool estado = false;

    printf("PRUEBA VACIO\n");
    lista_iterar(super, iter_int_imprim, &estado);
    print_test("prueba vacio", true);

    lista_insertar_ultimo(super, "leche");
    lista_insertar_ultimo(super, "huevos");
    lista_insertar_ultimo(super, "pan");
    lista_insertar_ultimo(super, "mermelada");

    estado = true;
    printf("PRUEBA HAY ELEMENTOS\n");
    lista_iterar(super, iter_int_imprim, &estado);

    printf("PRUEBA RECORRER TODO\n");
    int cont = 0;
    lista_iterar(super, iter_int_cont, &cont);
    print_test("cantidad de iteraciones = largo lista", lista_largo(super) == cont);
    
    printf("PRUEBA RECORRER HASTA LIMITE\n");
    int* datos = (int*)malloc(sizeof(int)*2);
    datos[0] = 0;
    datos[1] = 2; //maximo a iterar 
    lista_iterar(super, iter_int_cont_lim, datos);
    print_test("cantidad de iteraciones = maximo (2)", datos[0] == datos[1]);
    free(datos);

    lista_destruir(super, NULL);  
}

// ----------------------------------------------MAIN DE PRUEBAS------------------------------------------------------------

void pruebas_lista_alumno(void) {

    pruebas_lista_vacia();
    pruebas_lista_destr_con_ele();

    pruebas_lista_un_ele_pri();
    pruebas_lista_un_ele_ult();

    pruebas_lista_var_ele();
    pruebas_lista_nulo();

    pruebas_lista_comple_ele();
    pruebas_lista_comple_ele_heap();
    pruebas_lista_destr_con_comple_ele_heap();

    pruebas_lista_volumen();

    pruebas_iter_ext();
    pruebas_iter_int();
}
