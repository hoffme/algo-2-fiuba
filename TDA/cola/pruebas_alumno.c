#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void pruebas_cola_vacia(void){
    printf("----------                PRUEBAS COLA VACIA                ----------\n");

    void* cola = cola_crear();
    print_test("creacion de la cola", cola != NULL);
    
    print_test("verificacion cola vacia", cola_esta_vacia(cola) == true);
    print_test("ver primero (vacia -> nulo)", cola_ver_primero(cola) == NULL);
    print_test("desencolar (vacia -> nulo)", cola_desencolar(cola) == NULL);

    cola_destruir(cola, NULL);
    print_test("destruccion cola", true);
}

void pruebas_cola_destr_con_ele(void){
    printf("----------       PRUEBAS COLA ELIMINAR CON ELEMENTOS       ----------\n");

    int entero = 72;
    int entero1 = 55;
    int entero2 = 102;

    void* cola = cola_crear();
    print_test("creacion de la cola", cola != NULL);

    print_test("encolando entero", cola_encolar(cola, &entero) == true);
    print_test("encolando entero1", cola_encolar(cola, &entero1) == true);
    print_test("encolando entero2", cola_encolar(cola, &entero2) == true);
    print_test("verificacion cola no vacia", cola_esta_vacia(cola) == false);

    cola_destruir(cola, NULL);
    print_test("destruccion cola no vacia", true);
}

void pruebas_cola_un_ele(void){
    printf("----------         PRUEBAS COLA UNICO ELEMENTO          ----------\n");

    int entero = 72;

    void* cola = cola_crear();
    print_test("creacion de la cola", cola != NULL);

    print_test("encolando entero", cola_encolar(cola, &entero) == true);
    print_test("ver primero (entero)", cola_ver_primero(cola) == &entero);
    print_test("verificacion cola no vacia", cola_esta_vacia(cola) == false);

    print_test("desencolar entero", cola_desencolar(cola) == &entero);
    print_test("ver primero (vacia -> nulo)", cola_ver_primero(cola) == NULL);
    print_test("desencolar (vacia -> nulo)", cola_desencolar(cola) == NULL);

    cola_destruir(cola, NULL);
    print_test("destruccion cola", true);
}

void pruebas_cola_var_ele(void){
    printf("----------PRUEBAS COLA ENCOLAR Y DESENCOLAR VARIOS ELEMENTOS----------\n");

    char* cadena = "texto de prueba";
    int entero = 4;
    int arreglo[5] = {32, 46, 5, -3, 0};
    double doble = 5.23612;

    void* cola = cola_crear();
    print_test("creacion de la cola", cola != NULL);
    print_test("verificacion cola vacia", cola_esta_vacia(cola) == true);

    print_test("encolando cadena", cola_encolar(cola, &cadena) == true);

    print_test("encolando entero", cola_encolar(cola, &entero) == true);
    print_test("ver primero (cadena)", cola_ver_primero(cola) == &cadena);

    print_test("encolando arreglo", cola_encolar(cola, &arreglo) == true);
    print_test("verificacion cola no vacia", cola_esta_vacia(cola) == false);

    print_test("desencolar cadena", cola_desencolar(cola) == &cadena);
    print_test("ver primero (entero)", cola_ver_primero(cola) == &entero);

    print_test("encolando doble", cola_encolar(cola, &doble) == true);

    print_test("desencolar entero", cola_desencolar(cola) == &entero);
    print_test("desencolar arreglo", cola_desencolar(cola) == &arreglo);

    print_test("ver primero (doble)", cola_ver_primero(cola) == &doble);

    print_test("desencolar doble", cola_desencolar(cola) == &doble);
    
    print_test("ver primero (vacia -> nulo)", cola_ver_primero(cola) == NULL);
    print_test("verificacion cola vacia", cola_esta_vacia(cola) == true);
    
    cola_destruir(cola, NULL);
    print_test("destruccion cola", true);
}

void pruebas_cola_nulo(void){
    printf("----------       PRUEBAS COLA ENCOLAR Y DESENCOLAR NULOS      ----------\n");

    void* cola = cola_crear();
    print_test("creacion de la cola", cola != NULL);

    print_test("encolando nulo", cola_encolar(cola, NULL) == true);
    print_test("cola con nulo ver primero", cola_ver_primero(cola) == NULL);
    print_test("verificacion cola no vacia", cola_esta_vacia(cola) == false);

    print_test("encolando nulo", cola_encolar(cola, NULL) == true);

    print_test("desencolar cola es igual a nulo", cola_desencolar(cola) == NULL);
    print_test("verificacion cola no vacia", cola_esta_vacia(cola) == false);

    print_test("desencolar cola es igual a nulo", cola_desencolar(cola) == NULL);
    print_test("verificacion cola vacia", cola_esta_vacia(cola) == true);
    print_test("cola vacia ver primero", cola_ver_primero(cola) == NULL);

    cola_destruir(cola, NULL);
    print_test("destruccion cola", true);
}

void pruebas_cola_comple_ele(void){
    printf("----------       PRUEBAS COLA ENCOLAR Y DESENCOLAR STRUCTS      ----------\n");

    typedef struct estructura{
    	int entero;
    	double doble;
    	char cadena[25];
	} estructura_t;

    estructura_t* estructura1;
    estructura_t* estructura2;

    void* cola = cola_crear();
    print_test("creacion de la cola", cola != NULL);

    print_test("encolando estructura1", cola_encolar(cola, &estructura1) == true);
    print_test("cola con estructura1 ver primero", cola_ver_primero(cola) == &estructura1);
    print_test("verificacion cola no vacia", cola_esta_vacia(cola) == false);

    print_test("encolando estructura2", cola_encolar(cola, &estructura2) == true);

    print_test("desencolar cola es igual a estructura1", cola_desencolar(cola) == &estructura1);
    print_test("verificacion cola no vacia", cola_esta_vacia(cola) == false);

    print_test("desencolar cola es igual a estructura2", cola_desencolar(cola) == &estructura2);
    print_test("verificacion cola vacia", cola_esta_vacia(cola) == true);
    print_test("cola vacia ver primero", cola_ver_primero(cola) == NULL);

    cola_destruir(cola, NULL);
    print_test("destruccion cola", true);
}

void pruebas_cola_comple_ele_heap(void){
    printf("----------       PRUEBAS COLA STRUCTS HEAP DESENCOLANDO      ----------\n");

    typedef struct estructura{
        int entero;
        double doble;
        char cadena[25];
    } estructura_t;

    estructura_t* estructura1 = malloc(sizeof(estructura_t));
    estructura_t* estructura2 = malloc(sizeof(estructura_t));

    void* cola = cola_crear();
    print_test("creacion de la cola", cola != NULL);

    print_test("encolando estructura1", cola_encolar(cola, estructura1) == true);
    print_test("cola con estructura1 ver primero", cola_ver_primero(cola) == estructura1);
    print_test("verificacion cola no vacia", cola_esta_vacia(cola) == false);

    print_test("encolando estructura2", cola_encolar(cola, estructura2) == true);

    estructura_t* estructura1_recibida = cola_desencolar(cola);
    print_test("desencolar cola es igual a estructura1", estructura1_recibida == estructura1);
    free(estructura1_recibida);
    print_test("verificacion cola no vacia", cola_esta_vacia(cola) == false);

    estructura_t* estructura2_recibida = cola_desencolar(cola);
    print_test("desencolar cola es igual a estructura2", estructura2_recibida == estructura2);
    free(estructura2_recibida);
    print_test("verificacion cola vacia", cola_esta_vacia(cola) == true);
    print_test("cola vacia ver primero", cola_ver_primero(cola) == NULL);

    cola_destruir(cola, NULL);
    print_test("destruccion cola", true);
}


void pruebas_cola_destr_con_comple_ele_heap(void){
    printf("----------       PRUEBAS COLA STRUCTS HEAP SIN DESENCOLAR      ----------\n");

    typedef struct estructura{
        int entero;
        double doble;
        char cadena[25];
    } estructura_t;

    estructura_t* estructura1 = malloc(sizeof(estructura_t));
    estructura_t* estructura2 = malloc(sizeof(estructura_t));

    void* cola = cola_crear();
    print_test("creacion de la cola", cola != NULL);

    print_test("encolando estructura1", cola_encolar(cola, estructura1) == true);
    print_test("encolando estructura2", cola_encolar(cola, estructura2) == true);

    print_test("verificacion cola no vacia", cola_esta_vacia(cola) == false);
    print_test("cola vacia ver primero", cola_ver_primero(cola) == estructura1);

    cola_destruir(cola, free);
    print_test("destruccion cola", true);
}


void pruebas_cola_volumen(void){
    printf("----------               PRUEBAS GRANDES VOLUMENES           ----------\n");

    void* cola = cola_crear();
    print_test("creacion de la cola", cola != NULL);

    int entero = 0;

    int max = 10000;
    bool ok = true;
    for (int i = 0; i < max; i++) {
        ok &= cola_encolar(cola, &entero);
    }
    print_test("se pudieron encolar todos los elementos", ok);
    print_test("verificacion cola no vacia", cola_esta_vacia(cola) == false);

    ok = true;
    for (int i = 0; i < max; i++) {
        int* valor = cola_desencolar(cola);
        ok &= (valor == &entero);
    }
    print_test("se pudieron desencolar todos los elementos", ok);

    print_test("verificacion cola vacia", cola_esta_vacia(cola) == true);
    print_test("cola vacia ver primero", cola_ver_primero(cola) == NULL);

    cola_destruir(cola, NULL);
    print_test("destruccion cola", true);
}
void pruebas_cola_alumno() {

    pruebas_cola_vacia();
    pruebas_cola_destr_con_ele();
    pruebas_cola_un_ele();
    pruebas_cola_var_ele();
    pruebas_cola_nulo();
    pruebas_cola_comple_ele();
    pruebas_cola_comple_ele_heap();
    pruebas_cola_destr_con_comple_ele_heap();
    pruebas_cola_volumen();

}
