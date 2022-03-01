#include "abb.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

// static void prueba_crear_abb_vacio(void) {
//     abb_t* arbol = abb_crear(strcmp, NULL);

//     print_test("Prueba abb crear abb vacio", arbol);
//     print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(arbol) == 0);
//     print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(arbol, "A"));
//     print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(arbol, "A"));
//     print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(arbol, "A"));

//     abb_destruir(arbol);
// }

// static void prueba_iterar_abb_vacio(void) {
//     abb_t* arbol = abb_crear(strcmp, NULL);
//     abb_iter_t* iter = abb_iter_in_crear(arbol);
//     print_test("Prueba abb iter crear iterador  interno abb vacio", iter);
//     print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
//     print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
//     print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

//     abb_iter_in_destruir(iter);
//     abb_destruir(arbol);
// }

// static void prueba_abb_insertar(void) {
//     abb_t* arbol = abb_crear(strcmp, NULL);

//     char *clave1 = "perro", *valor1 = "guau";
//     char *clave2 = "gato", *valor2 = "miau";
//     char *clave3 = "vaca", *valor3 = "mu";

//     /* Inserta 1 valor y luego lo borra */
//     print_test("Prueba abb insertar clave1", abb_guardar(arbol, clave1, valor1));
//     print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(arbol) == 1);
//     print_test("Prueba abb obtener clave1 es valor1", abb_obtener(arbol, clave1) == valor1);
//     print_test("Prueba abb obtener clave1 es valor1", abb_obtener(arbol, clave1) == valor1);
//     print_test("Prueba abb pertenece clave1, es true", abb_pertenece(arbol, clave1));
//     print_test("Prueba abb borrar clave1, es valor1", abb_borrar(arbol, clave1) == valor1);
//     print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

//     /* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
//     print_test("Prueba abb insertar clave2", abb_guardar(arbol, clave2, valor2));
//     print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(arbol) == 1);
//     print_test("Prueba abb obtener clave2 es valor2", abb_obtener(arbol, clave2) == valor2);
//     print_test("Prueba abb obtener clave2 es valor2", abb_obtener(arbol, clave2) == valor2);
//     print_test("Prueba abb pertenece clave2, es true", abb_pertenece(arbol, clave2));

//     print_test("Prueba abb insertar clave3", abb_guardar(arbol, clave3, valor3));
//     print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(arbol) == 2);
//     print_test("Prueba abb obtener clave3 es valor3", abb_obtener(arbol, clave3) == valor3);
//     print_test("Prueba abb obtener clave3 es valor3", abb_obtener(arbol, clave3) == valor3);
//     print_test("Prueba abb pertenece clave3, es true", abb_pertenece(arbol, clave3));

//     abb_destruir(arbol);
// }

// static void prueba_abb_reemplazar(void) {
//     abb_t* arbol = abb_crear(strcmp, NULL);

//     char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
//     char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

//     /* Inserta 2 valores y luego los reemplaza */
//     print_test("Prueba abb insertar clave1", abb_guardar(arbol, clave1, valor1a));
//     print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(arbol, clave1) == valor1a);
//     print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(arbol, clave1) == valor1a);
//     print_test("Prueba abb insertar clave2", abb_guardar(arbol, clave2, valor2a));
//     print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(arbol, clave2) == valor2a);
//     print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(arbol, clave2) == valor2a);
//     print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

//     print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(arbol, clave1, valor1b));
//     print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(arbol, clave1) == valor1b);
//     print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(arbol, clave1) == valor1b);
//     print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(arbol, clave2, valor2b));
//     print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(arbol, clave2) == valor2b);
//     print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(arbol, clave2) == valor2b);
//     print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

//     abb_destruir(arbol);
// }

// static void prueba_abb_reemplazar_con_destruir(void) {
//     abb_t* arbol = abb_crear(strcmp, free);

//     char *clave1 = "perro", *valor1a, *valor1b;
//     char *clave2 = "gato", *valor2a, *valor2b;

//     /* Pide memoria para 4 valores */
//     valor1a = malloc(10 * sizeof(char));
//     valor1b = malloc(10 * sizeof(char));
//     valor2a = malloc(10 * sizeof(char));
//     valor2b = malloc(10 * sizeof(char));

//     /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
//     print_test("Prueba abb insertar clave1", abb_guardar(arbol, clave1, valor1a));
//     print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(arbol, clave1) == valor1a);
//     print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(arbol, clave1) == valor1a);
//     print_test("Prueba abb insertar clave2", abb_guardar(arbol, clave2, valor2a));
//     print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(arbol, clave2) == valor2a);
//     print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(arbol, clave2) == valor2a);
//     print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

//     print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(arbol, clave1, valor1b));
//     print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(arbol, clave1) == valor1b);
//     print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(arbol, clave1) == valor1b);
//     print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(arbol, clave2, valor2b));
//     print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(arbol, clave2) == valor2b);
//     print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(arbol, clave2) == valor2b);
//     print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

//     /* Se destruye el abb (se debe liberar lo que quedó dentro) */
//     abb_destruir(arbol);
// }

// static void prueba_abb_borrar(void) {
//     abb_t* arbol = abb_crear(strcmp, NULL);

//     char *clave1 = "perro", *valor1 = "guau";
//     char *clave2 = "gato", *valor2 = "miau";
//     char *clave3 = "vaca", *valor3 = "mu";

//     /* Inserta 3 valores y luego los borra */
//     print_test("Prueba abb insertar clave1", abb_guardar(arbol, clave1, valor1));
//     print_test("Prueba abb insertar clave2", abb_guardar(arbol, clave2, valor2));
//     print_test("Prueba abb insertar clave3", abb_guardar(arbol, clave3, valor3));

//     /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
//     print_test("Prueba abb pertenece clave3, es verdadero", abb_pertenece(arbol, clave3));
//     print_test("Prueba abb borrar clave3, es valor3", abb_borrar(arbol, clave3) == valor3);
//     print_test("Prueba abb borrar clave3, es NULL", !abb_borrar(arbol, clave3));
//     print_test("Prueba abb pertenece clave3, es falso", !abb_pertenece(arbol, clave3));
//     print_test("Prueba abb obtener clave3, es NULL", !abb_obtener(arbol, clave3));
//     print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(arbol) == 2);

//     print_test("Prueba abb pertenece clave1, es verdadero", abb_pertenece(arbol, clave1));
//     print_test("Prueba abb borrar clave1, es valor1", abb_borrar(arbol, clave1) == valor1);
//     print_test("Prueba abb borrar clave1, es NULL", !abb_borrar(arbol, clave1));
//     print_test("Prueba abb pertenece clave1, es falso", !abb_pertenece(arbol, clave1));
//     print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(arbol, clave1));
//     print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(arbol) == 1);

//     print_test("Prueba abb pertenece clave2, es verdadero", abb_pertenece(arbol, clave2));
//     print_test("Prueba abb borrar clave2, es valor2", abb_borrar(arbol, clave2) == valor2);
//     print_test("Prueba abb borrar clave2, es NULL", !abb_borrar(arbol, clave2));
//     print_test("Prueba abb pertenece clave2, es falso", !abb_pertenece(arbol, clave2));
//     print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(arbol, clave2));
//     print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

//     abb_destruir(arbol);
// }

// static void prueba_abb_clave_vacia(void) {
//     abb_t* arbol = abb_crear(strcmp, NULL);

//     char *clave = "", *valor = "";

//     print_test("Prueba abb insertar clave vacia", abb_guardar(arbol, clave, valor));
//     print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(arbol) == 1);
//     print_test("Prueba abb obtener clave vacia es valor", abb_obtener(arbol, clave) == valor);
//     print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(arbol, clave));
//     print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(arbol, clave) == valor);
//     print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

//     abb_destruir(arbol);
// }

// static void prueba_abb_valor_null(void) {
//     abb_t* arbol = abb_crear(strcmp, NULL);

//     char *clave = "", *valor = NULL;

//     /* Inserta 1 valor y luego lo borra */
//     print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(arbol, clave, valor));
//     print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(arbol) == 1);
//     print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(arbol, clave) == valor);
//     print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(arbol, clave));
//     print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(arbol, clave) == valor);
//     print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

//     abb_destruir(arbol);
// }

// static void prueba_abb_volumen(size_t largo, bool debug){
//     abb_t* arbol = abb_crear(strcmp, NULL);

//     const size_t largo_clave = 10;
//     char (*claves)[largo_clave] = malloc(largo * largo_clave);

//     unsigned* valores[largo];

//     /* Inserta 'largo' parejas en el abb */
//     bool ok = true;
//     for (unsigned i = 0; i < largo; i++) {
//         valores[i] = malloc(sizeof(int));
//         sprintf(claves[i], "%08d", i);
//         *valores[i] = i;
//         ok = abb_guardar(arbol, claves[i], valores[i]);
//         if (!ok) break;
//     }

//     if (debug) print_test("Prueba abb almacenar muchos elementos", ok);
//     if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(arbol) == largo);

//     /* Verifica que devuelva los valores correctos */
//     for (size_t i = 0; i < largo; i++) {
//         ok = abb_pertenece(arbol, claves[i]);
//         if (!ok) break;
//         ok = abb_obtener(arbol, claves[i]) == valores[i];
//         if (!ok) break;
//     }

//     if (debug) print_test("Prueba abb pertenece y obtener muchos elementos", ok);
//     if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(arbol) == largo);

//     /* Verifica que borre y devuelva los valores correctos */
//     for (size_t i = 0; i < largo; i++) {
//         ok = abb_borrar(arbol, claves[i]) == valores[i];
//         if (!ok) break;
//     }

//     if (debug) print_test("Prueba abb borrar muchos elementos", ok);
//     if (debug) print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(arbol) == 0);

//     /* Destruye el abb y crea uno nuevo que sí libera */
//     abb_destruir(arbol);
//     arbol = abb_crear(strcmp, free);

//     /* Inserta 'largo' parejas en el abb */
//     ok = true;
//     for (size_t i = 0; i < largo; i++) {
//         ok = abb_guardar(arbol, claves[i], valores[i]);
//         if (!ok) break;
//     }

//     free(claves);

//     /* Destruye el abb - debería liberar los enteros */
//     abb_destruir(arbol);
// }

static void abb_insertar_mejor_caso(size_t largo, size_t ini, size_t fin, abb_t* arbol, bool* estado) {
    size_t med = (fin + ini) / 2;
    char clave[20];
    sprintf(clave, "%ld", med);
    *estado = abb_guardar(arbol, clave, (void*)med);
    printf("INSERT ");
    print_test(clave, *estado);
    if (fin - ini <= 1) return;
    if (*estado) abb_insertar_mejor_caso(largo, ini, med - 1, arbol, estado);
    if (*estado) abb_insertar_mejor_caso(largo, med + 1, fin, arbol, estado);
}

static void abb_borrar_mejor_caso(size_t largo, size_t ini, size_t fin, abb_t* arbol, bool* estado) {
    size_t med = (fin + ini) / 2;
    char clave[20];
    sprintf(clave, "%ld", med);
    *estado = abb_borrar(arbol, clave) == (void*)med;
    printf("BORRAR ");
    print_test(clave, *estado);
    if (fin - ini <= 1) return;
    if (*estado) abb_borrar_mejor_caso(largo, ini, med - 1, arbol, estado);
    if (*estado) abb_borrar_mejor_caso(largo, med + 1, fin, arbol, estado);
}

static void abb_esta_mejor_caso(size_t largo, size_t ini, size_t fin, abb_t* arbol, bool* estado) {
    size_t med = (fin + ini) / 2;
    char clave[20];
    sprintf(clave, "%ld", med);
    *estado = abb_pertenece(arbol, clave);
    if (*estado) *estado = abb_obtener(arbol, clave) == (void*)med;
    printf("VER ");
    print_test(clave, *estado);
    if (fin - ini <= 1) return;
    if (*estado) abb_esta_mejor_caso(largo, ini, med - 1, arbol, estado);
    if (*estado) abb_esta_mejor_caso(largo, med + 1, fin, arbol, estado);
}

static void prueba_abb_volumen_mc(size_t largo, bool debug){
    abb_t* arbol = abb_crear(strcmp, NULL);
    
    /* Inserta 'largo' parejas en el abb */
    bool ok = true;
    abb_insertar_mejor_caso(largo, 0, largo, arbol, &ok);

    if (debug) print_test("Prueba abb almacenar muchos elementos en mc", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta en mc", abb_cantidad(arbol) == largo);

    /* Verifica que devuelva los valores correctos */
    ok = true;
    abb_esta_mejor_caso(largo, 0, largo, arbol, &ok);

    if (debug) print_test("Prueba abb pertenece y obtener muchos elementos en mc", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta en mc", abb_cantidad(arbol) == largo);

    /* Verifica que borre y devuelva los valores correctos */
    ok = true;
    abb_borrar_mejor_caso(largo, 0, largo, arbol, &ok);

    if (debug) print_test("Prueba abb borrar muchos elementos en mc", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es 0 en mc", abb_cantidad(arbol) == 0);

    abb_destruir(arbol);
}

// static ssize_t buscar(const char* clave, char* claves[], size_t largo) {
//     for (size_t i = 0; i < largo; i++) {
//         if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
//     }
//     return -1;
// }

// static void prueba_abb_iterar(void) {
//     abb_t* arbol = abb_crear(strcmp, NULL);

//     char *claves[] = {"perro", "gato", "vaca", "aguila", "iguana", "rinoceronte", "yakare"};
//     char *valores[] = {"guau", "miau", "mu", "ruuu", "auiih", "bufff", "claclu"};

//     /* Inserta 6 valores */
//     for (int i = 0; i < 7; ++i) {
//         print_test("Prueba abb insertar clave", abb_guardar(arbol, claves[i], valores[i]));
//     }

//     // Prueba de iteración sobre las claves almacenadas.
//     abb_iter_t* iter = abb_iter_in_crear(arbol);
//     const char *clave;
//     ssize_t indice;
//     ssize_t indice_ok;

//     print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

//     /* Primer valor */
//     clave = abb_iter_in_ver_actual(iter);
//     indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
//     indice_ok = buscar("aguila", claves, sizeof(claves) / sizeof(char *));
//     print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
//     print_test("Prueba abb iterador ver actual, es en inorder", indice == indice_ok);
//     print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
//     print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
//     print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

//     /* Segundo valor */
//     clave = abb_iter_in_ver_actual(iter);
//     indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
//     indice_ok = buscar("gato", claves, sizeof(claves) / sizeof(char *));
//     print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
//     print_test("Prueba abb iterador ver actual, es en inorder", indice == indice_ok);
//     print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
//     print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
//     print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

//     /* Tercer valor */
//     clave = abb_iter_in_ver_actual(iter);
//     indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
//     indice_ok = buscar("iguana", claves, sizeof(claves) / sizeof(char *));
//     print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
//     print_test("Prueba abb iterador ver actual, es en inorder", indice == indice_ok);
//     print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
//     abb_iter_in_avanzar(iter);

//     /* Cuarto valor */
//     clave = abb_iter_in_ver_actual(iter);
//     indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
//     indice_ok = buscar("perro", claves, sizeof(claves) / sizeof(char *));
//     print_test("Prueba abb iterador ver actual, es en inorder", indice == indice_ok);
//     abb_iter_in_avanzar(iter);

//     /* Quinto valor */
//     clave = abb_iter_in_ver_actual(iter);
//     indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
//     indice_ok = buscar("rinoceronte", claves, sizeof(claves) / sizeof(char *));
//     print_test("Prueba abb iterador ver actual, es en inorder", indice == indice_ok);
//     abb_iter_in_avanzar(iter);

//     /* Sexto valor */
//     clave = abb_iter_in_ver_actual(iter);
//     indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
//     indice_ok = buscar("vaca", claves, sizeof(claves) / sizeof(char *));
//     print_test("Prueba abb iterador ver actual, es en inorder", indice == indice_ok);
//     abb_iter_in_avanzar(iter);

//     /* Septimo valor */
//     clave = abb_iter_in_ver_actual(iter);
//     indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
//     indice_ok = buscar("yakare", claves, sizeof(claves) / sizeof(char *));
//     print_test("Prueba abb iterador ver actual, es en inorder", indice == indice_ok);
//     abb_iter_in_avanzar(iter);

//     /* Vuelve a tratar de avanzar, por las dudas */
//     print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
//     print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
//     print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

//     abb_iter_in_destruir(iter);
//     abb_destruir(arbol);
// }

// static bool imprimir(const char* clave, void* valor, void* extra) {
//     size_t pos = ((size_t*)extra)[1];
//     char* clave_act = ((char*)extra) + pos;
//     print_test("clave in order", strcmp(clave, clave_act));
//     ++pos;
//     return true;
// }

// static void prueba_abb_iter_interno_vacio(void) {
//     abb_t* arbol = abb_crear(strcmp, NULL);

//     print_test("Prueba abb interno vacio", true);

//     abb_in_order(arbol, imprimir, NULL);

//     print_test("Prueba abb in order vacio", true);
    
//     abb_destruir(arbol);
// }

// static void prueba_abb_iter_interno(void) {
//     abb_t* arbol = abb_crear(strcmp, NULL);

//     size_t cant = 6;
//     char* claves[] = {"r", "h", "q", "c", "k", "l"};
//     char* valores[] = {"guau", "miau", "mu", "cuak", "ruar", "buff"};

//     /* Inserta los valores */    
//     for (int i = 0; i < cant; ++i) {
//         abb_guardar(arbol, claves[i], valores[i]);
//     }

//     char* claves_in_order[] = {"c", "h", "k", "l", "q", "r"};
//     void* extra[] = {&claves_in_order, 0};

//     abb_in_order(arbol, imprimir, &extra);

//     print_test("Prueba abb in order", true);
    
//     abb_destruir(arbol);
// }

// static void prueba_abb_iterar_volumen(size_t largo) {
//     abb_t* arbol = abb_crear(strcmp, NULL);

//     const size_t largo_clave = 10;
//     char (*claves)[largo_clave] = malloc(largo * largo_clave);

//     size_t valores[largo];

//     /* Inserta 'largo' parejas en el abb */
//     bool ok = true;
//     for (unsigned i = 0; i < largo; i++) {
//         sprintf(claves[i], "%08d", i);
//         valores[i] = i;
//         ok = abb_guardar(arbol, claves[i], &valores[i]);
//         if (!ok) break;
//     }

//     // Prueba de iteración sobre las claves almacenadas.
//     abb_iter_t* iter = abb_iter_in_crear(arbol);
//     print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

//     ok = true;
//     unsigned i;
//     const char *clave;
//     size_t *valor;

//     for (i = 0; i < largo; i++) {
//         if ( abb_iter_in_al_final(iter) ) {
//             ok = false;
//             break;
//         }
//         clave = abb_iter_in_ver_actual(iter);
//         if ( clave == NULL ) {
//             ok = false;
//             break;
//         }
//         valor = abb_obtener(arbol, clave);
//         if ( valor == NULL ) {
//             ok = false;
//             break;
//         }
//         *valor = largo;
//         abb_iter_in_avanzar(iter);
//     }
//     print_test("Prueba abb iteración en volumen", ok);
//     print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
//     print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

//     ok = true;
//     for (i = 0; i < largo; i++) {
//         if ( valores[i] != largo ) {
//             ok = false;
//             break;
//         }
//     }
//     print_test("Prueba abb iteración en volumen, se cambiaron todo los elementos", ok);

//     free(claves);
//     abb_iter_in_destruir(iter);
//     abb_destruir(arbol);
// }

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

void pruebas_abb_alumno(void) {
    /* Ejecuta todas las pruebas unitarias. */
    // prueba_crear_abb_vacio();
    // prueba_iterar_abb_vacio();
    // prueba_abb_insertar();
    // prueba_abb_reemplazar();
    // prueba_abb_reemplazar_con_destruir();
    // prueba_abb_borrar();
    // prueba_abb_clave_vacia();
    // prueba_abb_valor_null();
    // prueba_abb_iter_interno_vacio();
    // prueba_abb_iter_interno();
    // prueba_abb_volumen(200, true);
    prueba_abb_volumen_mc(15, true);
    // prueba_abb_iterar();
    // prueba_abb_iterar_volumen(200);
}