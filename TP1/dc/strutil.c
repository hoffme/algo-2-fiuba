#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_strv(char *strv[]);

char* substr(const char *str, size_t n) {
	char* res = calloc(n + 1, sizeof(char));
	if (!res) return NULL;
	strncpy(res, str, n);
	return res;
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

char* join(char **strv, char sep) {
	size_t sum_str = 0;
	size_t cant_sep = 0;
	for (cant_sep = 0; strv[cant_sep]; ++cant_sep) sum_str += strlen(strv[cant_sep]);

	char* res = (char*)pedir_memoria(sizeof(char), sum_str + cant_sep + 1);
	if (!res) return NULL;

	int pos_res = 0;
	for (int i = 0; strv[i]; ++i) {
		for (int pos_act = 0; strv[i][pos_act] != '\0'; ++pos_act, ++pos_res) res[pos_res] = strv[i][pos_act];
		if (sep != '\0') {
			res[pos_res] = sep;
			if (strv[i + 1]) ++pos_res;
		}
	}

	res[pos_res] = '\0';

	return res;
}

void free_strv(char *strv[]) {
	for (int i = 0; strv[i]; ++i) free(strv[i]);
	free(strv);
}