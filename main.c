#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "main.h"
#include "misc.h"
#include "onibus.h"
#include "passageiro.h"
#include "pontoOnibus.h"
#include "tela.h"

/**
 * Função que é chamada quando ós argumentos
 * não estão no formato padrão
 */
void usage(char *argv0) {
	printf("Utilize a seguinte linha de comando:\n");
	printf("\t%s <S> <C> <P> <A>\n", argv0);
	printf("S número de pontos de ônibus.\n");
	printf("C número de carros que representam os ônibus.\n");
	printf("P número de passageiros que desejam andar de ônibus.\n");
	printf("A número de assentos em cada ônibus.\n");
}



/**
 * Inicio da aplicação
 * os parametros são iguais aos especificados pelo pdf
 */
void init(int S_param, int C_param, int P_param, int A_param) {
	sectionDebug("Inicializando variáveis da linha de comando");
	// atribuindo os parametros as variáveis globais
	S = S_param; debug("Pontos de ônibus: %d\n", S);
	C = C_param; debug("Número de ônibus: %d\n", C);
	P = P_param; debug("Número de passageiros: %d\n", P);
	A = A_param; debug("Assentos em cada ônibus: %d\n", A);

	//----------------------------------------------------
	// Testa as condições de existencia
	//----------------------------------------------------
	// todos eles devem ser maiores do que zero
	if(S <= 0 || C <= 0 || P <= 0 || A <= 0) {
		printf("Por favor, todos os parâmetros de entrada devem ser maiores do que zero.\n");
		exit(0);
	}
	if(P <= A) {
		printf("Por favor, utilize uma quantiadede de passageiros maior do que a quantidade de assentos.\n");
		exit(0);
	}
	if(A <= C) {
		printf("Por favor, utilize uma quantidade de assentos maior do que a quantidade de ônibus.\n");
		exit(0);
	}


	sectionDebug("Inicializando threads");
	// criando as threads
	// alocando a memória
	threadsOnibus      = (pthread_t *) calloc(C, sizeof(pthread_t));
	threadsPassageiro  = (pthread_t *) calloc(P, sizeof(pthread_t));
	threadsPontoOnibus = (pthread_t *) calloc(S, sizeof(pthread_t));


	// inicializando as threads
	for(int i=0; i<S; i++) {
		pthread_create(&threadsPontoOnibus[i], NULL, pontoOnibusRun, voidptr_t(i));
	}
	for(int i=0; i<C; i++) {
		pthread_create(&threadsOnibus[i], NULL, onibusRun, voidptr_t(i));
	}
	for(int i=0; i<P; i++) {
		pthread_create(&threadsPassageiro[i], NULL, passageiroRun, voidptr_t(i));
	}
	// só falta criar a thread tela
	pthread_create(&threadTela, NULL, telaRun, NULL);



	// finalizando as threads
	for(int i=0; i<C; i++) {
		pthread_join(threadsOnibus[i], NULL);
	}
	for(int i=0; i<P; i++) {
		pthread_join(threadsPassageiro[i], NULL);
	}
	for(int i=0; i<S; i++) {
		pthread_join(threadsPontoOnibus[i], NULL);
	}
	// só falta dar join na tela
	pthread_join(threadTela, NULL);


	debug("\n");
	sectionDebug("Função init na main voltando a executar.");


	// desalocando a memória
	free(threadsOnibus);
	free(threadsPassageiro);
	free(threadsPontoOnibus);

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
	
	//----------------------------------------------------
	// Testando parametros de entrada
	//----------------------------------------------------
	int i;
	for(i=1; i<5; i++) {
		if(!is_int(argv[i])) {
			// se um deles não for um inteiro, devo sair do programa
			printf("Por favor, todos os argumentos devem ser número inteiros.\n");
			printf("Para mais detalhes, use %s --help.\n", argv[0]);
			return 0;
		}
	}



	/**
	 * Preparando para a função init
	 */
	char *Sstr = argv[1];
	char *Cstr = argv[2];
	char *Pstr = argv[3];
	char *Astr = argv[4];

	// convertendo para inteiros
	init(atoi(Sstr), atoi(Cstr), atoi(Pstr), atoi(Astr));

	return 0;
}
#endif // TEST