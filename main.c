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
	return (timediff(false)/1E3)/fatorTempo;	//Em algum momento do cógigo vai ter timediff(true) e vai começar a contar
												//o tempo. Aqui para de contar e retorna o tempo em milisegundo, por isso
												//tem que dividir por 1000 (1E3). Divide pelo fator de tempo, pois se for
												//DEBUG o tempo fica maior (pq? não sei, mas fica).
}


/**
 * retorna uma string com a hora atual do sistema, não precisa de free
 * @return formato "11h15m30s"
 */
char *horaAtual() {
	static char result[30] = {0};	//A variável estática continua declarada, entre as chamadas da função, por isso
									//não precisa de free, pois usa sempre a mesma.
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

	// cria uma seed para cada nova thread (Acentos têm threads?)
	// cada thread precisa de uma seed e fazer um srand
	seeds = createList();
	for(int i=0; i<S+C+A+P; i++) {
		filaPush(seeds, rand());	//Coloca valores aleatórios na lista de seeds
	}
	sem_init(&semDepoisDePegarSeed, 0, 0);	//Semáforo inicializado com 0
											//É um quando uma thread já pegou a seed e a seguinte pode ser criada
											//É 0 quando isso não ocorreu e essa thread tem que esperar pra criar outra thread
											//Thread criada dá UP após pegar a seed, essa thread dá down pra ver se pode
											//criar outra thread ou ir dormir.

	// começa a contar o tempo a partir daqui
	timediff(true);



	// iniciando objetos
	
	// primeiro alloco
	pontosOnibus = (pontoOnibus_t *) malloc(sizeof(pontoOnibus_t) * S);	//Lista com S pontos de ônibus
	onibusArray  = (onibus_t *) malloc(sizeof(onibus_t) * C);			//Lista com C ônibus
	passageiros  = (passageiro_t *) malloc(sizeof(passageiro_t) * P);	//Lista com P passageiros

	// agora eu inicializo
	sectionDebug("Iniciando as variaveis");
	for(int i=0; i<S; i++) {
		pontoOnibusInit(&pontosOnibus[i], i);	//Tah nos arquivo pontoOnibus
	}
	for(int i=0; i<C; i++) {
		onibusInit(&onibusArray[i], i);			//Tah no arquivo onibus
	}
	for(int i=0; i<P; i++) {
		passageiroInit(&passageiros[i], i);		//Tah no arquivo passageiro
	}


	// criando os identificadores das threads
	// alocando a memória
	threadsPontoOnibus = (pthread_t *) malloc(sizeof(pthread_t) * S);	//Lista com S identificadores de threads 
																		// dos pontos de ônibus
	threadsOnibus      = (pthread_t *) malloc(sizeof(pthread_t) * C);	//Lista com C identificadores de threads dos ônibus
	threadsPassageiro  = (pthread_t *) malloc(sizeof(pthread_t) * P);	//Lista com P identificadores de threads dod passageiros


	// inicializando as threads
	sectionDebug("Inicializando threads");
	//i é o número de identificação do ponto de ônibus, do ônibus e do passageiro.
	for(int i=0; i<S; i++) {
		//Para criar um thread, passamos como argumentos, o endereço do identificador da thread correspondente, um NULL,
		//a função que vai rodar na thread, e o ID do ponto de ônibus como void *.
		pthread_create(&threadsPontoOnibus[i], NULL, pontoOnibusRun, cast(void *, i));	
		// sempre aguardo ele obter a seed de número aleatório para pegar outra
		//Em pontoOnibusRun vai ser pega uma semente, e vai ser dado UP no semáforo (vai de 0 para 1). 
		//Se isso acontecer ante de chegar aqui, essa thread dá um DOWN e o semáforo volta pra zero, e cria a próxima.
		//Se chegar nessa parte do código antes disso acontecer, o semáforo vai continuar com valor 0, e essa thread vai
		//dormir até q pontoOnibusRun pegue a semente, de UP, acorde essa thread e ele volte o semáforo pra 0 antes de 
		//criar a próxima.
		sem_wait(&semDepoisDePegarSeed);
	}
	for(int i=0; i<C; i++) {
		pthread_create(&threadsOnibus[i], NULL, onibusRun, cast(void *, i));
		sem_wait(&semDepoisDePegarSeed);
	}
	for(int i=0; i<P; i++) {
		//A função passageiroRun necessita de mais de um parâmetro, por isso...
		// devo passar uma strutura contendo o id do passageiro e
		// o ID do ponto de origem e de destino
		//O tipo passageiro_param_t está declarado em "passageiro.h"
		passageiro_param_t *param = (passageiro_param_t *) malloc(sizeof(passageiro_param_t));
		// vou precisar passar para  passageiroRun

		param->id = i;
		// definindo pontos de ônibus de forma aleatória
		int iPontoOrigem, iPontoDestino;
		while(true) {
			// procura um ponto que seja diferente de outro ponto
			//Como são S pontos de ônibus os ID vão de 0 a S-1
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
	// todas as threads foram criadas
	pthread_create(&threadTela, NULL, telaRun, NULL);	//Cria a thread que muda a tela.



	// finalizando as threads
	// devem seguir a ordem
	//Espera tds os passagerios terminarem
	for(int i=0; i<P; i++) {
		pthread_join(threadsPassageiro[i], NULL);
	}
	debug("Todos os passageiros terminaram.\n");

	// libera a tela antes dos pontos
	// pra n dar seg fault
	pthread_cancel(threadTela);	//Faz a thread de tela parar

	//Se não tem passagerios, faz as threads de ônibus e de pontos de ônibus pararem.
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


	//Libera os recursos alocados em cada uma das structs que representam os ônibus e os pontos de ônibus
	// liberando os recursos
	for(int i=0; i<C; i++) {
		onibusFinish(&onibusArray[i]);
	}
	for(int i=0; i<S; i++) {
		pontoOnibusFinish(&pontosOnibus[i]);
	}
	telaFinish();	//Finaliza a tela

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
	free(passageiros);	//As structs de passageiro não tem nd alocado, pois isso basta dar free em passageiros

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