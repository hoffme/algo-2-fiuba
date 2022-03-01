#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void pruebas_pila_vacia(void){
    printf("----------                PRUEBAS PILA VACIA                ----------\n");

    void* pila = pila_crear();
    print_test("creacion de la pila", pila != NULL);
    
    print_test("verificacion pila vacia", pila_esta_vacia(pila) == true);
    print_test("ver tope (vacia -> nulo)", pila_ver_tope(pila) == NULL);
    print_test("desapilar (vacia -> nulo)", pila_desapilar(pila) == NULL);

    pila_destruir(pila);
    print_test("destruccion pila", true);
}

void pruebas_pila_eliminar_con_elementos(void){
    printf("----------       PRUEBAS PILA ELIMINAR CON ELEMENTOS       ----------\n");

    int entero = 72;
    int entero1 = 55;
    int entero2 = 102;

    void* pila = pila_crear();
    print_test("creacion de la pila", pila != NULL);

    print_test("apilando entero", pila_apilar(pila, &entero) == true);
    print_test("apilando entero1", pila_apilar(pila, &entero1) == true);
    print_test("apilando entero2", pila_apilar(pila, &entero2) == true);
    print_test("verificacion pila no vacia", pila_esta_vacia(pila) == false);

    pila_destruir(pila);
    print_test("destruccion pila no vacia", true);
}

void pruebas_pila_unico_elemento(void){
    printf("----------         PRUEBAS PILA UNICO ELEMENTO          ----------\n");

    int entero = 72;

    void* pila = pila_crear();
    print_test("creacion de la pila", pila != NULL);

    print_test("apilando entero", pila_apilar(pila, &entero) == true);
    print_test("ver tope (entero)", pila_ver_tope(pila) == &entero);
    print_test("verificacion pila no vacia", pila_esta_vacia(pila) == false);

    print_test("desapilar entero", pila_desapilar(pila) == &entero);
    print_test("ver tope (vacia -> nulo)", pila_ver_tope(pila) == NULL);
    print_test("desapilar (vacia -> nulo)", pila_desapilar(pila) == NULL);

    pila_destruir(pila);
    print_test("destruccion pila", true);
}

void pruebas_pila_elementos_varios(void){
    printf("----------PRUEBAS PILA APILAR Y DESAPILAR VARIOS ELEMENTOS----------\n");

    char* cadena = "texto de prueba";
    int entero = 4;
    int arreglo[5] = {32, 46, 5, -3, 0};
    double doble = 5.23612;

    void* pila = pila_crear();
    print_test("creacion de la pila", pila != NULL);
    print_test("verificacion pila vacia", pila_esta_vacia(pila) == true);

    print_test("apilando cadena", pila_apilar(pila, &cadena) == true);    
    print_test("ver tope (cadena)", pila_ver_tope(pila) == &cadena);

    print_test("apilando entero", pila_apilar(pila, &entero) == true);    
    print_test("ver tope (entero)", pila_ver_tope(pila) == &entero);

    print_test("apilando arreglo", pila_apilar(pila, &arreglo) == true);
    print_test("ver tope (arreglo)", pila_ver_tope(pila) == &arreglo);
    print_test("verificacion pila no vacia", pila_esta_vacia(pila) == false);

    print_test("desapilar arreglo", pila_desapilar(pila) == &arreglo);
    print_test("ver tope (entero)", pila_ver_tope(pila) == &entero);

    print_test("apilando doble", pila_apilar(pila, &doble) == true);

    print_test("desapilar doble", pila_desapilar(pila) == &doble);
    print_test("desapilar entero", pila_desapilar(pila) == &entero);
    print_test("desapilar cadena", pila_desapilar(pila) == &cadena);
    
    print_test("ver tope (vacia -> nulo)", pila_ver_tope(pila) == NULL);
    print_test("verificacion pila vacia", pila_esta_vacia(pila) == true);
    
    pila_destruir(pila);
    print_test("destruccion pila", true);
}

void pruebas_pila_elementos_nulo(void){
    printf("----------       PRUEBAS PILA APILAR Y DESAPILAR NULOS      ----------\n");

    void* pila = pila_crear();
    print_test("creacion de la pila", pila != NULL);

    print_test("apilando nulo", pila_apilar(pila, NULL) == true);
    print_test("pila con nulo ver tope", pila_ver_tope(pila) == NULL);
    print_test("verificacion pila no vacia", pila_esta_vacia(pila) == false);

    print_test("apilando nulo", pila_apilar(pila, NULL) == true);

    print_test("desapilar pila es igual a nulo", pila_desapilar(pila) == NULL);
    print_test("verificacion pila no vacia", pila_esta_vacia(pila) == false);

    print_test("desapilar pila es igual a nulo", pila_desapilar(pila) == NULL);
    print_test("verificacion pila vacia", pila_esta_vacia(pila) == true);
    print_test("pila vacia ver tope", pila_ver_tope(pila) == NULL);

    pila_destruir(pila);
    print_test("destruccion pila", true);
}

void pruebas_pila_volumen(void){
    printf("----------               PRUEBAS GRANDES VOLUMENES           ----------\n");

    void* pila = pila_crear();
    print_test("creacion de la pila", pila != NULL);

    int entero = 73;
    bool estado = true;

    for (int i = 0; i < 15000; ++i) {
        if (pila_apilar(pila, &entero) == false || pila_esta_vacia(pila) == true || pila_ver_tope(pila) != &entero) {
            estado = false;
            break;
        }
    }

    print_test("apilar 15000", estado);

    for (int i = 0; i < 15000; ++i) {
        if (pila_desapilar(pila) != &entero || (i < 14999 && pila_esta_vacia(pila) == true) || (i < 14999 && pila_ver_tope(pila) != &entero)) {
            estado = false;
            break;
        }
    }    
    print_test("desapilar 15000", estado);

    print_test("verificacion pila vacia", pila_esta_vacia(pila) == true);
    print_test("pila vacia ver tope", pila_ver_tope(pila) == NULL);

    pila_destruir(pila);
    print_test("destruccion pila", true);
}

void pruebas_pila_alumno() {

    pruebas_pila_vacia();
    pruebas_pila_eliminar_con_elementos();
    pruebas_pila_unico_elemento();
    pruebas_pila_elementos_varios();
    pruebas_pila_elementos_nulo();
    pruebas_pila_volumen();

}
