#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){

	printf("PRUEBAS SUBSTR\n");

	const char* ejemplo = "Ejemplo";
	
	char* subcadena1 = substr(ejemplo, 6);
	char* subcadena2 = substr(ejemplo + 2, 2);
	char* subcadena3 = substr("", 2);
	char* subcadena4 = substr(ejemplo, 300);

	printf("1_ '%s'\n", subcadena1);
	printf("2_ '%s'\n", subcadena2);
	printf("3_ '%s'\n", subcadena3);
	printf("4_ '%s'\n", subcadena4);

	free(subcadena1);
	free(subcadena2);
	free(subcadena3);
	free(subcadena4);

	printf("PRUEBAS SPLIT\n");

	char* cadena1 = "abc,,def";
	char* cadena2 = "abc,def,";
	char* cadena3 = ",abc,def";
	char* cadena4 = "abc";
	char* cadena5 = "";
	char* cadena6 = ",";

	char** splt1 = split(cadena1, ',');
	char** splt2 = split(cadena2, ',');
	char** splt3 = split(cadena3, ',');
	char** splt4 = split(cadena4, '\0');
	char** splt5 = split(cadena5, ',');
	char** splt6 = split(cadena6, ',');
	
	printf("PRUEBA CAD1\n");
	for (int i = 0; splt1[i] != NULL; ++i)	{
		printf("N° %d -> '%s'\n", i, splt1[i]);
	}

	printf("PRUEBA CAD2\n");
	for (int i = 0; splt2[i] != NULL; ++i)	{
		printf("N° %d -> '%s'\n", i, splt2[i]);
	}

	printf("PRUEBA CAD3\n");
	for (int i = 0; splt3[i] != NULL; ++i)	{
		printf("N° %d -> '%s'\n", i, splt3[i]);
	}

	printf("PRUEBA CAD4\n");
	for (int i = 0; splt4[i] != NULL; ++i)	{
		printf("N° %d -> '%s'\n", i, splt4[i]);
	}

	printf("PRUEBA CAD5\n");
	for (int i = 0; splt5[i] != NULL; ++i)	{
		printf("N° %d -> '%s'\n", i, splt5[i]);
	}

	printf("PRUEBA CAD6\n");
	for (int i = 0; splt6[i] != NULL; ++i)	{
		printf("N° %d -> '%s'\n", i, splt6[i]);
	}

	printf("PRUEBAS JOIN\n");

	char* join_5 = join(splt5, ',');
	printf("N° 5 -> '%s'\n", join_5);

	char* join_4 = join(splt4, ',');
	printf("N° 4 -> '%s'\n", join_4);

	char* join_6 = join(splt6, ',');
	printf("N° 6 -> '%s'\n", join_6);

	char* arr[1] = {NULL};
	char* join_arr = join(arr, ',');
	printf("N° arr -> '%s'\n", join_arr);

	char* cadena7 = "abc,def";
	char** splt7 = split(cadena7, ',');
	char* join_7 = join(splt7, '\0');
	printf("N° 7 -> '%s'\n", join_7);

	free(join_5);
	free(join_4);
	free(join_6);
	free(join_arr);
	free(join_7);

	printf("PRUEBA FREE STRV\n");

	free_strv(splt1);
	free_strv(splt2);
	free_strv(splt3);
	free_strv(splt4);
	free_strv(splt5);
	free_strv(splt6);
	free_strv(splt7);

	return 1;
}