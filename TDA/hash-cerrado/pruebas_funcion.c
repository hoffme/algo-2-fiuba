#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// size_t funcion_hash(char* str, size_t len)
// {
//    size_t hash = len;
//    size_t i = 0;

//    for(i = 0; i < len; str++, i++)
//    {
//       hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
//    }
//    return hash;
// }

#define PRIME 1000000007u                                       // This value must not be changed.
#define X 263                                                   // Multiplier; this value must not be changed.

size_t funcion_hash(char *s, size_t len) {
    unsigned long long h = 0;
    int slen = (int)strlen(s);

    for (register int i = slen - 1; i >= 0; --i)
        h = (h * X + s[i]) % PRIME;

    return h % len;
}

int main(int argc, char *argv[]) {
	if (argc != 2 || atoi(argv[1]) == 0) return 0;

	size_t len = atoi(argv[1]);
	char* str = calloc(30, sizeof(char));
	char** tabla = calloc(len, sizeof(char*));
	unsigned long coliciones = 0;
	unsigned long analizados = 0;
	char ant[30];

	while (true) {
		strcpy(ant, str);
		scanf("%s", str);
		if (strcmp(ant, str) == 0) break;

		++analizados;

		size_t pos = funcion_hash(str, len);
		
		if (!tabla[pos]) {
			tabla[pos] = calloc(30, sizeof(char));
			if (tabla[pos]) {
				strcpy(tabla[pos], str);
			}
		}
		else {
			++coliciones;
			printf("NUEVA COLICION %ld de %ld, ingresada: %s, tabla: %s\n", coliciones, analizados, str, tabla[pos]);
		}
	}

	free(tabla);
	
}