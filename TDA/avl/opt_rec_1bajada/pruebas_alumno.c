#include "avl.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* aleatoria_pal(size_t tam) {
    char* res = malloc(sizeof(char) * (tam + 1));
    if (!res) return NULL;

    for (int i = 0; i < tam; ++i) res[i] = (char)((rand() % 25) + 97);
    res[tam] = '\0';

    return res;
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_avl_vacio(void) {
    avl_t* arbol = avl_crear(strcmp, NULL);

    print_test("Prueba avl crear avl vacio", arbol);
    print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);
    print_test("Prueba avl obtener clave A, es NULL, no existe", !avl_obtener(arbol, "A"));
    print_test("Prueba avl pertenece clave A, es false, no existe", !avl_pertenece(arbol, "A"));
    print_test("Prueba avl borrar clave A, es NULL, no existe", !avl_borrar(arbol, "A"));
    print_test("Prueba avl balanceado", balanceado(arbol));

    avl_destruir(arbol);
}

static void prueba_avl_insertar(void) {
    avl_t* arbol = avl_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    print_test("Prueba avl balanceado", balanceado(arbol));
    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1));
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);
    print_test("Prueba avl obtener clave1 es valor1", avl_obtener(arbol, clave1) == valor1);
    print_test("Prueba avl obtener clave1 es valor1", avl_obtener(arbol, clave1) == valor1);
    print_test("Prueba avl pertenece clave1, es true", avl_pertenece(arbol, clave1));
    print_test("Prueba avl borrar clave1, es valor1", avl_borrar(arbol, clave1) == valor1);
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el avl) */
    print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2));
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);
    print_test("Prueba avl obtener clave2 es valor2", avl_obtener(arbol, clave2) == valor2);
    print_test("Prueba avl obtener clave2 es valor2", avl_obtener(arbol, clave2) == valor2);
    print_test("Prueba avl pertenece clave2, es true", avl_pertenece(arbol, clave2));
    print_test("Prueba avl balanceado", balanceado(arbol));

    print_test("Prueba avl insertar clave3", avl_guardar(arbol, clave3, valor3));
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);
    print_test("Prueba avl obtener clave3 es valor3", avl_obtener(arbol, clave3) == valor3);
    print_test("Prueba avl obtener clave3 es valor3", avl_obtener(arbol, clave3) == valor3);
    print_test("Prueba avl pertenece clave3, es true", avl_pertenece(arbol, clave3));
    print_test("Prueba avl balanceado", balanceado(arbol));

    avl_destruir(arbol);
}

static void prueba_avl_insertar_desvalanceando(void) {
    avl_t* arbol = avl_crear(strcmp, free);

    size_t tam = 15;

    void** valores[tam * 2];

    bool ok;
    for (int i = 0; i < tam; ++i) {
        int* dat = malloc(sizeof(int));
        *dat = i;
        char* clave = aleatoria_pal(3);
        ok = avl_guardar(arbol, clave, dat);
        valores[i * 2] = (void*)clave;
        valores[(i * 2) + 1] = (void*)dat;
        if (!ok) break;
    }

    print_test("Prueba avl guardado", ok);
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl la cantidad de elementos correcta", avl_cantidad(arbol) == tam);

    ok = true;
    for (int i = 0; i < tam; ++i) {
        ok = avl_obtener(arbol, (char*)valores[i * 2]) == (int*)valores[(i * 2) + 1];
        if (!ok) break;
    }
    print_test("Prueba avl obtener claves es igual a valor", ok);
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl la cantidad de elementos correcta", avl_cantidad(arbol) == tam);

    int* raiz = (int*)avl_borrar(arbol, (char*)valores[3 * 2]);
    print_test("Prueba borrar raiz", raiz == (int*)valores[(3 * 2) + 1]);
    free(raiz);

    print_test("Prueba avl obtener raiz es igual a null", !avl_obtener(arbol, (char*)valores[3 * 2]));
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl la cantidad de elementos correcta", avl_cantidad(arbol) == tam - 1);

    for (int i = 0; i < tam * 2; ++i) if (i % 2 == 0) free(valores[i]);

    avl_destruir(arbol);
}

static void prueba_avl_reemplazar(void) {
    avl_t* arbol = avl_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1a));
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
    print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
    print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2a));
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
    print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
    print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);

print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl insertar clave1 con otro valor", avl_guardar(arbol, clave1, valor1b));
    print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
    print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
    print_test("Prueba avl insertar clave2 con otro valor", avl_guardar(arbol, clave2, valor2b));
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
    print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
    print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);
    print_test("Prueba avl balanceado", balanceado(arbol));

    avl_destruir(arbol);
}

static void prueba_avl_reemplazar_con_destruir(void) {
    avl_t* arbol = avl_crear(strcmp, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1a));
    print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
    print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2a));
    print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
    print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
    print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);
    print_test("Prueba avl balanceado", balanceado(arbol));

    print_test("Prueba avl insertar clave1 con otro valor", avl_guardar(arbol, clave1, valor1b));
    print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
    print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
    print_test("Prueba avl insertar clave2 con otro valor", avl_guardar(arbol, clave2, valor2b));
    print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
    print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
    print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);
    print_test("Prueba avl balanceado", balanceado(arbol));

    /* Se destruye el avl (se debe liberar lo que quedó dentro) */
    avl_destruir(arbol);
}

static void prueba_avl_borrar(void) {
    avl_t* arbol = avl_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 3 valores y luego los borra */
    print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1));
    print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2));
    print_test("Prueba avl insertar clave3", avl_guardar(arbol, clave3, valor3));

    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
    print_test("Prueba avl pertenece clave3, es verdadero", avl_pertenece(arbol, clave3));
    print_test("Prueba avl borrar clave3, es valor3", avl_borrar(arbol, clave3) == valor3);
    print_test("Prueba avl borrar clave3, es NULL", !avl_borrar(arbol, clave3));
    print_test("Prueba avl pertenece clave3, es falso", !avl_pertenece(arbol, clave3));
    print_test("Prueba avl obtener clave3, es NULL", !avl_obtener(arbol, clave3));
    print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);

    print_test("Prueba avl pertenece clave1, es verdadero", avl_pertenece(arbol, clave1));
    print_test("Prueba avl borrar clave1, es valor1", avl_borrar(arbol, clave1) == valor1);
    print_test("Prueba avl borrar clave1, es NULL", !avl_borrar(arbol, clave1));
    print_test("Prueba avl pertenece clave1, es falso", !avl_pertenece(arbol, clave1));
    print_test("Prueba avl obtener clave1, es NULL", !avl_obtener(arbol, clave1));
    print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);

    print_test("Prueba avl pertenece clave2, es verdadero", avl_pertenece(arbol, clave2));
    print_test("Prueba avl borrar clave2, es valor2", avl_borrar(arbol, clave2) == valor2);
    print_test("Prueba avl borrar clave2, es NULL", !avl_borrar(arbol, clave2));
    print_test("Prueba avl pertenece clave2, es falso", !avl_pertenece(arbol, clave2));
    print_test("Prueba avl obtener clave2, es NULL", !avl_obtener(arbol, clave2));
    print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

    avl_destruir(arbol);
}

static void prueba_avl_clave_vacia(void) {
    avl_t* arbol = avl_crear(strcmp, NULL);

    char *clave = "", *valor = "";

    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl insertar clave vacia", avl_guardar(arbol, clave, valor));
    print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);
    print_test("Prueba avl obtener clave vacia es valor", avl_obtener(arbol, clave) == valor);
    print_test("Prueba avl pertenece clave vacia, es true", avl_pertenece(arbol, clave));
    print_test("Prueba avl balanceado", balanceado(arbol));
    print_test("Prueba avl borrar clave vacia, es valor", avl_borrar(arbol, clave) == valor);
    print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

    avl_destruir(arbol);
}

static void prueba_avl_valor_null(void) {
    avl_t* arbol = avl_crear(strcmp, NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba avl insertar clave vacia valor NULL", avl_guardar(arbol, clave, valor));
    print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);
    print_test("Prueba avl obtener clave vacia es valor NULL", avl_obtener(arbol, clave) == valor);
    print_test("Prueba avl pertenece clave vacia, es true", avl_pertenece(arbol, clave));
    print_test("Prueba avl borrar clave vacia, es valor NULL", avl_borrar(arbol, clave) == valor);
    print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

    avl_destruir(arbol);
}


/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

void pruebas_avl_alumno(void) {
    /* Ejecuta todas las pruebas unitarias. 
    pc == peor caso y mc == mejor caso  */
    printf("--NUEVA PRUEBA--\n");
    prueba_crear_avl_vacio();

    printf("--NUEVA PRUEBA--\n");
    prueba_avl_insertar();

    printf("--NUEVA PRUEBA--\n");
    prueba_avl_insertar_desvalanceando();

    printf("--NUEVA PRUEBA--\n");
    prueba_avl_reemplazar();

    printf("--NUEVA PRUEBA--\n");
    prueba_avl_reemplazar_con_destruir();

    printf("--NUEVA PRUEBA--\n");
    prueba_avl_borrar();

    printf("--NUEVA PRUEBA--\n");
    prueba_avl_clave_vacia();

    printf("--NUEVA PRUEBA--\n");
    prueba_avl_valor_null();
}