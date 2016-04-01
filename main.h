#ifndef __MAIN_H__
#define __MAIN_H__

#include <pthread.h>


// declarações
typedef struct onibus_t onibus_t;
typedef struct passageiro_t passageiro_t;
typedef struct pontoOnibus_t pontoOnibus_t;


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


void init(int S_param, int C_param, int P_param, int A_param);



#endif // __MAIN_H__