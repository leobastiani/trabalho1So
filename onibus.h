#ifndef __ONIBUS_H__
#define __ONIBUS_H__


#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>



// devem existir
typedef struct list_t list_t;
typedef struct pontoOnibus_t pontoOnibus_t;


typedef struct onibus_t {
	int id;
	// lista de passageiros no ônibus
	list_t *passageiros;

	// ponteiro para o ponto de ônibus atual
	pontoOnibus_t *pontoOnibus;
	// se estou andando, ou seja, não estou em nenhum ponto
	// mas sempre estou indo para algum ponto
	pontoOnibus_t *proxPonto;

	sem_t semAguardaEmbarque;
	sem_t semTodosPassageirosConferiram;
} onibus_t;




/**
 * função necessária para ser chamada antes do Run
 * inicializa um onibus
 * com o proximo ponto que ele deve ir
 * ou seja, todos os pontos começam circulando
 */
void onibusInit(onibus_t *this, int id);


/**
 * função que deve ser utilizada como argumento em pthread_create
 * contem os passos que um onibus deve fazer
 * que é:
 * ir para um ponto
 * esperar os passageiros embarcarem
 * ir para o próximo ponto
 */
void *onibusRun(void *param);


/**
 * função chamada assim que um onibus se encerra
 * serve para dar free nas variaveis ou simplesmente encerrar-la
 */
void onibusFinish(onibus_t *this);


/**
 * diz se um onibus está cheio
 */
bool onibusCheio(onibus_t *this);


/**
 * função que simula o trajeto de um onibus
 * calcula o tempo necessário para ele chegar no ponto
 * e aguarda até sua chegada
 */
void onibusIrParaPonto(onibus_t *this, pontoOnibus_t *pontoOnibus);




#endif // __ONIBUS_H__