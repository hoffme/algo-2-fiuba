gcc -g -O2 -std=c99 -Wall -Werror -pedantic -Wformat=2 -Wshadow -Wpointer-arith -Wunreachable-code -Wconversion -Wno-sign-conversion -Wbad-function-cast *.c -o pruebas
valgrind --leak-check=full --track-origins=yes -s --show-reachable=yes ./pruebas