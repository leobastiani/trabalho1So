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
 * retorna o momento em segundos da aplicação
 * retorna uma resposta boa msmo se eu tentar acelearar a aplicação
 * com o fatorTempo
 */
double segundosFicticios() {
	return (timediff(false)/1E3)/fatorTempo;
}


/**
 * retorna uma string com a hora atual do sistema, não precisa de free
 * @return formato "11h15m30s"
 */
char *horaAtual() {
	static char result[30] = {0};
	time_t timeAtual = time(NULL);
	struct tm *tmAtual = localtime(&timeAtual);

	sprintf(result, "%02dh%02dm%02ds", tmAtual->tm_hour, tmAtual->tm_min, tmAtual->tm_sec);
	return result;
}


/**
 * Inicio da aplicação
 * os parametros são iguais aos especificados pelo pdf
 */
void run(int S_param, int C_param, int P_param, int A_param) {
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
		return ;
	}
	if(P <= A) {
		printf("Por favor, utilize uma quantiadede de passageiros maior do que a quantidade de assentos em cada ônibus.\n");
		return ;
	}
	if(A <= C) {
		printf("Por favor, utilize uma quantidade de assentos maior do que a quantidade de ônibus.\n");
		return ;
	}
	if(S == 1) {
		printf("A aplicação precisa ter mais de um ponto de ônibus.\n");
		return ;
	}
	if(S+C+P > 1023) {
		printf("A aplicação não pode gerar mais do que 1023 threads.\n");
		return ;
	}

	// cria uma seed para cada nova thread
	// cada thread precisa de uma seed e fazer um srand
	seeds = createList();
	for(int i=0; i<S+C+A+P; i++) {
		filaPush(seeds, rand());
	}
	sem_init(&semDepoisDePegarSeed, 0, 0);


	// começa a contar o tempo a partir daqui
	timediff(true);



	// iniciando objetos
	// primeiro alloco
	pontosOnibus = (pontoOnibus_t *) malloc(sizeof(pontoOnibus_t) * S);
	onibusArray  = (onibus_t *) malloc(sizeof(onibus_t) * C);
	passageiros  = (passageiro_t *) malloc(sizeof(passageiro_t) * P);
	// agora eu inicializo
	sectionDebug("Iniciando as variaveis");
	for(int i=0; i<S; i++) {
		pontoOnibusInit(&pontosOnibus[i], i);
	}
	for(int i=0; i<C; i++) {
		onibusInit(&onibusArray[i], i);
	}
	for(int i=0; i<P; i++) {
		passageiroInit(&passageiros[i], i);
	}


	// criando as threads
	// alocando a memória
	threadsPontoOnibus = (pthread_t *) malloc(sizeof(pthread_t) * S);
	threadsOnibus      = (pthread_t *) malloc(sizeof(pthread_t) * C);
	threadsPassageiro  = (pthread_t *) malloc(sizeof(pthread_t) * P);


	// inicializando as threads
	sectionDebug("Inicializando threads");
	for(int i=0; i<S; i++) {
		pthread_create(&threadsPontoOnibus[i], NULL, pontoOnibusRun, cast(void *, i));
		// sempre aguardo ele obter a seed de número aleatório para pegar outra
		sem_wait(&semDepoisDePegarSeed);
	}
	for(int i=0; i<C; i++) {
		pthread_create(&threadsOnibus[i], NULL, onibusRun, cast(void *, i));
		sem_wait(&semDepoisDePegarSeed);
	}
	for(int i=0; i<P; i++) {
		// devo passar uma strutura contendo o id
		// ponto de origem e de destino
		passageiro_param_t *param = (passageiro_param_t *) malloc(sizeof(passageiro_param_t));
		// vou precisar liberar param em passageiroRun

		param->id = i;
		// definindo pontos de ônibus
		int iPontoOrigem, iPontoDestino;
		while(true) {
			// procura um ponto que seja diferente de outro ponto
			iPontoOrigem = randMinMax(0, S-1);
			iPontoDestino = randMinMax(0, S-1);
			// se eles forem diferentes, posso sair
			if(iPontoOrigem != iPontoDestino) {
				break;
			}
		}
		param->iPontoOrigem = iPontoOrigem;
		param->iPontoDestino = iPontoDestino;

		// já posso pedir para ele executar
		pthread_create(&threadsPassageiro[i], NULL, passageiroRun, cast(void *, param));
		sem_wait(&semDepoisDePegarSeed);
	}

	// finalizando as threads
	// devem seguir a ordem
	for(int i=0; i<P; i++) {
		pthread_join(threadsPassageiro[i], NULL);
	}
	debug("Todos os passageiros terminaram.\n");

	for(int i=0; i<C; i++) {
		debug("cancelando a thread %d de onibus\n", i);
		pthread_cancel(threadsOnibus[i]);
	}
	debug("Todos os ônibus terminaram\n");

	for(int i=0; i<S; i++) {
		debug("cancelando a thread %d de ponto de onibus\n", i);
		pthread_cancel(threadsPontoOnibus[i]);
	}
	debug("Todos os pontos de onibus terminaram\n");


	// liberando os recursos
	for(int i=0; i<C; i++) {
		onibusFinish(&onibusArray[i]);
	}
	for(int i=0; i<S; i++) {
		pontoOnibusFinish(&pontosOnibus[i]);
	}

	debug("\n");
	sectionDebug("Todas as threads foram encerradas");
	debug("Tempo total da aplicação: %g minutos\n", segundosFicticios() / 60);
	debug("Tempo total no mundo real: %g segundos\n", timediff(false) / 1E3);


	// desalocando a memória
	free(threadsOnibus);
	free(threadsPassageiro);
	free(threadsPontoOnibus);

	free(pontosOnibus);
	free(onibusArray);
	free(passageiros);

	freeList(seeds);
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


	// inicializa uma seed para números randômicos
	_srand(0);


	/**
	 * Preparando para a função run
	 */
	char *Sstr = argv[1];
	char *Cstr = argv[2];
	char *Pstr = argv[3];
	char *Astr = argv[4];

	// convertendo para inteiros
	run(atoi(Sstr), atoi(Cstr), atoi(Pstr), atoi(Astr));

	return 0;
}
#endif // TEST