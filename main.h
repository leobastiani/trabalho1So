#ifndef __MAIN_H__
#define __MAIN_H__

#include <pthread.h>

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