#include "avl.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

// static void prueba_crear_avl_vacio(void) {
//     avl_t* arbol = avl_crear(strcmp, NULL);

//     print_test("Prueba avl crear avl vacio", arbol);
//     print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);
//     print_test("Prueba avl obtener clave A, es NULL, no existe", !avl_obtener(arbol, "A"));
//     print_test("Prueba avl pertenece clave A, es false, no existe", !avl_pertenece(arbol, "A"));
//     print_test("Prueba avl borrar clave A, es NULL, no existe", !avl_borrar(arbol, "A"));

//     avl_destruir(arbol);
// }

// static void prueba_avl_insertar(void) {
//     avl_t* arbol = avl_crear(strcmp, NULL);

//     char *clave1 = "perro", *valor1 = "guau";
//     char *clave2 = "gato", *valor2 = "miau";
//     char *clave3 = "vaca", *valor3 = "mu";

//     /* Inserta 1 valor y luego lo borra */
//     print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1));
//     print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);
//     print_test("Prueba avl obtener clave1 es valor1", avl_obtener(arbol, clave1) == valor1);
//     print_test("Prueba avl obtener clave1 es valor1", avl_obtener(arbol, clave1) == valor1);
//     print_test("Prueba avl pertenece clave1, es true", avl_pertenece(arbol, clave1));
//     print_test("Prueba avl borrar clave1, es valor1", avl_borrar(arbol, clave1) == valor1);
//     print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

//     /* Inserta otros 2 valores y no los borra (se destruyen con el avl) */
//     print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2));
//     print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);
//     print_test("Prueba avl obtener clave2 es valor2", avl_obtener(arbol, clave2) == valor2);
//     print_test("Prueba avl obtener clave2 es valor2", avl_obtener(arbol, clave2) == valor2);
//     print_test("Prueba avl pertenece clave2, es true", avl_pertenece(arbol, clave2));

//     print_test("Prueba avl insertar clave3", avl_guardar(arbol, clave3, valor3));
//     print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);
//     print_test("Prueba avl obtener clave3 es valor3", avl_obtener(arbol, clave3) == valor3);
//     print_test("Prueba avl obtener clave3 es valor3", avl_obtener(arbol, clave3) == valor3);
//     print_test("Prueba avl pertenece clave3, es true", avl_pertenece(arbol, clave3));

//     avl_destruir(arbol);
// }

// static void prueba_avl_reemplazar(void) {
//     avl_t* arbol = avl_crear(strcmp, NULL);

//     char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
//     char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

//     /* Inserta 2 valores y luego los reemplaza */
//     print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1a));
//     print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
//     print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
//     print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2a));
//     print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
//     print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
//     print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);

//     print_test("Prueba avl insertar clave1 con otro valor", avl_guardar(arbol, clave1, valor1b));
//     print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
//     print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
//     print_test("Prueba avl insertar clave2 con otro valor", avl_guardar(arbol, clave2, valor2b));
//     print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
//     print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
//     print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);

//     avl_destruir(arbol);
// }

// static void prueba_avl_reemplazar_con_destruir(void) {
//     avl_t* arbol = avl_crear(strcmp, free);

//     char *clave1 = "perro", *valor1a, *valor1b;
//     char *clave2 = "gato", *valor2a, *valor2b;

//     /* Pide memoria para 4 valores */
//     valor1a = malloc(10 * sizeof(char));
//     valor1b = malloc(10 * sizeof(char));
//     valor2a = malloc(10 * sizeof(char));
//     valor2b = malloc(10 * sizeof(char));

//     /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
//     print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1a));
//     print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
//     print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
//     print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2a));
//     print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
//     print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
//     print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);

//     print_test("Prueba avl insertar clave1 con otro valor", avl_guardar(arbol, clave1, valor1b));
//     print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
//     print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
//     print_test("Prueba avl insertar clave2 con otro valor", avl_guardar(arbol, clave2, valor2b));
//     print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
//     print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
//     print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);

//     /* Se destruye el avl (se debe liberar lo que quedó dentro) */
//     avl_destruir(arbol);
// }

// static void prueba_avl_borrar(void) {
//     avl_t* arbol = avl_crear(strcmp, NULL);

//     char *clave1 = "perro", *valor1 = "guau";
//     char *clave2 = "gato", *valor2 = "miau";
//     char *clave3 = "vaca", *valor3 = "mu";

//     /* Inserta 3 valores y luego los borra */
//     print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1));
//     print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2));
//     print_test("Prueba avl insertar clave3", avl_guardar(arbol, clave3, valor3));

//     /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
//     print_test("Prueba avl pertenece clave3, es verdadero", avl_pertenece(arbol, clave3));
//     print_test("Prueba avl borrar clave3, es valor3", avl_borrar(arbol, clave3) == valor3);
//     print_test("Prueba avl borrar clave3, es NULL", !avl_borrar(arbol, clave3));
//     print_test("Prueba avl pertenece clave3, es falso", !avl_pertenece(arbol, clave3));
//     print_test("Prueba avl obtener clave3, es NULL", !avl_obtener(arbol, clave3));
//     print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);

//     print_test("Prueba avl pertenece clave1, es verdadero", avl_pertenece(arbol, clave1));
//     print_test("Prueba avl borrar clave1, es valor1", avl_borrar(arbol, clave1) == valor1);
//     print_test("Prueba avl borrar clave1, es NULL", !avl_borrar(arbol, clave1));
//     print_test("Prueba avl pertenece clave1, es falso", !avl_pertenece(arbol, clave1));
//     print_test("Prueba avl obtener clave1, es NULL", !avl_obtener(arbol, clave1));
//     print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);

//     print_test("Prueba avl pertenece clave2, es verdadero", avl_pertenece(arbol, clave2));
//     print_test("Prueba avl borrar clave2, es valor2", avl_borrar(arbol, clave2) == valor2);
//     print_test("Prueba avl borrar clave2, es NULL", !avl_borrar(arbol, clave2));
//     print_test("Prueba avl pertenece clave2, es falso", !avl_pertenece(arbol, clave2));
//     print_test("Prueba avl obtener clave2, es NULL", !avl_obtener(arbol, clave2));
//     print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

//     avl_destruir(arbol);
// }

// static void prueba_avl_clave_vacia(void) {
//     avl_t* arbol = avl_crear(strcmp, NULL);

//     char *clave = "", *valor = "";

//     print_test("Prueba avl insertar clave vacia", avl_guardar(arbol, clave, valor));
//     print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);
//     print_test("Prueba avl obtener clave vacia es valor", avl_obtener(arbol, clave) == valor);
//     print_test("Prueba avl pertenece clave vacia, es true", avl_pertenece(arbol, clave));
//     print_test("Prueba avl borrar clave vacia, es valor", avl_borrar(arbol, clave) == valor);
//     print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

//     avl_destruir(arbol);
// }

// static void prueba_avl_valor_null(void) {
//     avl_t* arbol = avl_crear(strcmp, NULL);

//     char *clave = "", *valor = NULL;

//     /* Inserta 1 valor y luego lo borra */
//     print_test("Prueba avl insertar clave vacia valor NULL", avl_guardar(arbol, clave, valor));
//     print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);
//     print_test("Prueba avl obtener clave vacia es valor NULL", avl_obtener(arbol, clave) == valor);
//     print_test("Prueba avl pertenece clave vacia, es true", avl_pertenece(arbol, clave));
//     print_test("Prueba avl borrar clave vacia, es valor NULL", avl_borrar(arbol, clave) == valor);
//     print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

//     avl_destruir(arbol);
// }

static bool ver_in_order(const char* clave, void* dato, void* pos) {
    // printf("CLAVE %s\n", clave);
    return true;
}

static void prueba_avl_volumen_pc(size_t largo, bool debug){
    avl_t* arbol = avl_crear(strcmp, NULL);

    const size_t largo_clave = 15;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    size_t* valores[largo];

    /* Inserta 'largo' parejas en el avl */
    bool ok = true;
    for (size_t i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08ld", largo - i - 1);
        *valores[i] = i;
        // printf("guardando -> %s\n", claves[i]);
        ok = avl_guardar(arbol, claves[i], valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("Prueba avl almacenar muchos elementos", ok);
    if (debug) print_test("Prueba avl la cantidad de elementos es correcta", avl_cantidad(arbol) == largo);

    /* Verifica que devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = avl_pertenece(arbol, claves[i]);
        if (!ok) break;
        ok = avl_obtener(arbol, claves[i]) == valores[i];
        if (!ok) break;
    }

    avl_in_order(arbol, ver_in_order, NULL);

    if (debug) print_test("Prueba avl pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba avl la cantidad de elementos es correcta", avl_cantidad(arbol) == largo);

    /* Verifica que borre y devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = avl_borrar(arbol, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba avl borrar muchos elementos", ok);
    if (debug) print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

    /* Destruye el avl y crea uno nuevo que sí libera */
    avl_destruir(arbol);
    arbol = avl_crear(strcmp, free);

    /* Inserta 'largo' parejas en el avl */
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = avl_guardar(arbol, claves[i], valores[i]);
        if (!ok) break;
    }

    free(claves);

    /* Destruye el avl - debería liberar los enteros */
    avl_destruir(arbol);
}

// static void avl_insertar_mejor_caso(size_t largo, size_t ini, size_t fin, avl_t* arbol, bool* estado) {
//     size_t med = (fin + ini) / 2;
//     char clave[20];
//     sprintf(clave, "%ld", med);
//     *estado = avl_guardar(arbol, clave, (void*)med);
//     // printf("INSERT ");
//     // print_test(clave, *estado);
//     if (fin - ini <= 1) return;
//     if (*estado) avl_insertar_mejor_caso(largo, ini, med - 1, arbol, estado);
//     if (*estado) avl_insertar_mejor_caso(largo, med + 1, fin, arbol, estado);
// }

// static void avl_borrar_mejor_caso(size_t largo, size_t ini, size_t fin, avl_t* arbol, bool* estado) {
//     size_t med = (fin + ini) / 2;
//     char clave[20];
//     sprintf(clave, "%ld", med);
//     *estado = avl_borrar(arbol, clave) == (void*)med;
//     // printf("BORRAR ");
//     // print_test(clave, *estado);
//     if (fin - ini <= 1) return;
//     if (*estado) avl_borrar_mejor_caso(largo, ini, med - 1, arbol, estado);
//     if (*estado) avl_borrar_mejor_caso(largo, med + 1, fin, arbol, estado);
// }

// static void avl_esta_mejor_caso(size_t largo, size_t ini, size_t fin, avl_t* arbol, bool* estado) {
//     size_t med = (fin + ini) / 2;
//     char clave[20];
//     sprintf(clave, "%ld", med);
//     *estado = avl_pertenece(arbol, clave);
//     if (*estado) *estado = avl_obtener(arbol, clave) == (void*)med;
//     // printf("VER ");
//     // print_test(clave, *estado);
//     if (fin - ini <= 1) return;
//     if (*estado) avl_esta_mejor_caso(largo, ini, med - 1, arbol, estado);
//     if (*estado) avl_esta_mejor_caso(largo, med + 1, fin, arbol, estado);
// }

// static void prueba_avl_volumen_mc(size_t largo, bool debug){
//     avl_t* arbol = avl_crear(strcmp, NULL);
    
//     /* Inserta 'largo' parejas en el avl */
//     bool ok = true;
//     avl_insertar_mejor_caso(largo, 0, largo, arbol, &ok);

//     if (debug) print_test("Prueba avl almacenar muchos elementos en mc", ok);
//     if (debug) print_test("Prueba avl la cantidad de elementos es correcta en mc", avl_cantidad(arbol) == largo);

//     /* Verifica que devuelva los valores correctos */
//     ok = true;
//     avl_esta_mejor_caso(largo, 0, largo, arbol, &ok);

//     if (debug) print_test("Prueba avl pertenece y obtener muchos elementos en mc", ok);
//     if (debug) print_test("Prueba avl la cantidad de elementos es correcta en mc", avl_cantidad(arbol) == largo);

//     /* Verifica que borre y devuelva los valores correctos */
//     ok = true;
//     avl_borrar_mejor_caso(largo, 0, largo, arbol, &ok);

//     if (debug) print_test("Prueba avl borrar muchos elementos en mc", ok);
//     if (debug) print_test("Prueba avl la cantidad de elementos es 0 en mc", avl_cantidad(arbol) == 0);

//     avl_destruir(arbol);
// }

// static bool imprimir(const char* clave, void* valor, void* extra) {
//     size_t pos = ((size_t*)extra)[1];
//     char* clave_act = ((char*)extra) + pos;
//     print_test("clave in order", strcmp(clave, clave_act));
//     ++pos;
//     return true;
// }

// static void prueba_avl_iter_interno_vacio(void) {
//     avl_t* arbol = avl_crear(strcmp, NULL);

//     print_test("Prueba avl interno vacio", true);

//     avl_in_order(arbol, imprimir, NULL);

//     print_test("Prueba avl in order vacio", true);
    
//     avl_destruir(arbol);
// }

// static void prueba_avl_iter_interno(void) {
//     avl_t* arbol = avl_crear(strcmp, NULL);

//     size_t cant = 6;
//     char* claves[] = {"r", "h", "q", "c", "k", "l"};
//     char* valores[] = {"guau", "miau", "mu", "cuak", "ruar", "buff"};

//     /* Inserta los valores */    
//     for (int i = 0; i < cant; ++i) {
//         avl_guardar(arbol, claves[i], valores[i]);
//     }

//     char* claves_in_order[] = {"c", "h", "k", "l", "q", "r"};
//     void* extra[] = {&claves_in_order, 0};

//     avl_in_order(arbol, imprimir, &extra);

//     print_test("Prueba avl in order", true);
    
//     avl_destruir(arbol);
// }

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

void pruebas_avl_alumno(void) {
    /* Ejecuta todas las pruebas unitarias. 
    pc == peor caso y mc == mejor caso  */
    // prueba_crear_avl_vacio();
    // prueba_avl_insertar();
    // prueba_avl_reemplazar();
    // prueba_avl_reemplazar_con_destruir();
    // prueba_avl_borrar();
    // prueba_avl_clave_vacia();
    // prueba_avl_iter_interno_vacio();
    // prueba_avl_iter_interno();
    // prueba_avl_valor_null();
    prueba_avl_volumen_pc(200000, true);
    // prueba_avl_volumen_mc(4095, true); //utilizar valores de log_2(x) - 1
}