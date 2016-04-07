#ifndef __MAIN_H__
#define __MAIN_H__

#include <pthread.h>
#include <semaphore.h>


// declarações
typedef struct onibus_t onibus_t;
typedef struct passageiro_t passageiro_t;
typedef struct pontoOnibus_t pontoOnibus_t;
typedef struct list_t list_t;


//----------------------------------------------------
// Variaveis
//----------------------------------------------------

/**
 * são 4 argumentos no formato:
 * Há S pontos de ônibus.
 * Há C carros que representam os ônibus.
 * Há P passageiros que desejam andar de ônibus.
 * Há A assentos em cada ônibus.
 * Garanta que P >> A >> C.
 */
int S, C, P, A;
// defines com nomes mais sujestivos
#define nPontosOnibus S
#define nOnibus       C
#define nPassageiros  P
#define nAssentos     A



// é uma lista com todas as seeds necessárias
// cada thread precisa de uma seed
list_t *seeds;
// variavel que serve para que eu deixe que eu
// crie uma nova thread se a seed foi obtida
sem_t semDepoisDePegarSeed;


// diz com ql velocidade o programa vai executar
// se é 1, roda na velocidade normal
// se é 0.2, roda em apenas 20% do tempo
// é sempre multiplicado quando vou dar um sleep
#ifdef TEST
	// estou em teste
	// menor tempo possível é 0.05, ainda apresenta indeterminações, mas é bom
	#define fatorTempo 0.05
#else
	#define fatorTempo 1
#endif // TEST



// os objetos
onibus_t *onibusArray; // nao tem plural :/
passageiro_t *passageiros;
pontoOnibus_t *pontosOnibus;


// as threads
// são vetores
pthread_t *threadsOnibus;
pthread_t *threadsPassageiro;
pthread_t *threadsPontoOnibus;
// não são vetores
pthread_t threadTela;


//----------------------------------------------------
// Funções
//----------------------------------------------------


void run(int S_param, int C_param, int P_param, int A_param);
double segundosFicticios();


#endif // __MAIN_H__