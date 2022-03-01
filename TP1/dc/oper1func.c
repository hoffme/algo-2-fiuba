bool operacion(char* oper_str, pila_t* pila) {
	/*	Dado un string de la operacion a realizar devuelve el puntero a la 
	 * 	funciond de la operacion, caso contrario devuelve NULL.
	 */
	long* res = NULL;
	long* ent1 = pila_desapilar(pila);
	long* ent2 = pila_desapilar(pila);
	long* ent3 = pila_desapilar(pila);

	if (ent1 && ent2 && ent3 && strcmp(oper_str, "?") == 0) {
		if (*ent1 == 0) res = nuevo_entero(*ent3);
		else if (*ent1 != 0) res = nuevo_entero(*ent2);

		if (res) {
			if (!pila_apilar(pila, res)) return false;
			free(ent1);
			free(ent3);
			free(ent2);
			return true;
		}
	}
	if (ent1 && ent2) {
		if (strcmp(oper_str, "+") == 0) res = nuevo_entero(*ent1 + *ent2);
		else if (strcmp(oper_str, "-") == 0) res = nuevo_entero(*ent1 - *ent2);
		else if (strcmp(oper_str, "*") == 0) res = nuevo_entero(*ent1 * *ent2);
		else if (strcmp(oper_str, "/") == 0 && *ent2 != 0) res = nuevo_entero(*ent1 / *ent2);
		else if (strcmp(oper_str, "^")  == 0 && *ent2 >= 0) res = nuevo_entero(pot(*ent1, *ent2));
		else if (strcmp(oper_str, "log") == 0 && *ent2 > 1 && *ent1 > 0) res = nuevo_entero(loga(*ent1, *ent2));

		if (res) {
			if (ent3 && !pila_apilar(pila, ent3)) return false;
			if (!pila_apilar(pila, res)) return false;
			free(ent1);
			free(ent2);
			return true;
		}
	}
	if (ent1 && strcmp(oper_str, "sqrt") == 0 && *ent1 >= 0) {
		res = nuevo_entero(raiz(*ent1, 0, *ent1));

		if (res) {
			if (ent3 && !pila_apilar(pila, ent3)) return false;
			if (ent2 && !pila_apilar(pila, ent2)) return false;
			if (!pila_apilar(pila, res)) return false;
			free(ent1);
			return true;
		}
	}

	if (ent1) free(ent1);
	if (ent2) free(ent2);
	if (ent3) free(ent3);

	return false;	
}
