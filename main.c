#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "misc.h"

/**
 * Função que é chamada quando ós argumentos
 * não estão no formato padrão
 */
void usage(char *argv0) {
	printf("Utilize a seguinte linha de comando:\n");
	printf("\t%s S C P A\n", argv0);
	printf("S pontos de ônibus.\n");
	printf("C carros que representam os ônibus.\n");
	printf("P passageiros que desejam andar de ônibus.\n");
	printf("A assentos em cada ônibus.\n");
}

#ifndef TEST
int main(int argc, char *argv[]) {

	// testa os argumentos no início
	if(argc != 5) {
		// são 4 argumentos +1 do inicial
		
		usage(argv[0]);

		return 0;
	}
	/**
	 * são 4 argumentos no formato:
	 * Há S pontos de ônibus.
	 * Há C carros que representam os ônibus.
	 * Há P passageiros que desejam andar de ônibus.
	 * Há A assentos em cada ônibus.
	 * Garanta que P >> A >> C.
	 */
	
	// testando so valores se são inteirios
	int i;
	for(i=1; i<5; i++) {
		if(!is_int(argv[i])) {
			// se um deles não for um inteiro, devo sair do programa
			return 0;
		}
	}
	
	char *Sstr = argv[1];
	char *Cstr = argv[2];
	char *Pstr = argv[3];
	char *Astr = argv[4];
	// convertendo para inteiros
	int S = atoi(Sstr); debug("S: %d\n", S);
	int C = atoi(Cstr); debug("C: %d\n", C);
	int P = atoi(Pstr); debug("P: %d\n", P);
	int A = atoi(Astr); debug("A: %d\n", A);

	return 0;
}
#endif // TEST