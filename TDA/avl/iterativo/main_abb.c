#include "testing.h"
#include <stdlib.h>
#include <stdio.h>

void pruebas_avl_alumno(void);

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

int main(int argc, char *argv[])
{
    printf("~~~ PRUEBAS ALUMNO ~~~\n");
    pruebas_avl_alumno();

    return failure_count() > 0;
}
