#ifndef __PASSAGEIRO_H__
#define __PASSAGEIRO_H__


#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>


typedef struct pontoOnibus_t pontoOnibus_t;
typedef struct onibus_t onibus_t;


typedef struct passageiro_t {
	int id;

	// onibus que estou
	onibus_t *onibus;
	pontoOnibus_t *pontoOrigem, *pontoDestino;
	// ponto que o passageiro se encontra
	pontoOnibus_t *pontoOnibus;

	// é o status de cada passageiro, ele pode estar esperando, entrando no onibus, descendo
	int status;

	// estou no ponto e devo esperar o onibus
	sem_t semEsperarOnibusChegar;
} passageiro_t;


typedef struct passageiro_param_t {
	int id;
	int iPontoOrigem;
	int iPontoDestino;
} passageiro_param_t;


void passageiroInit(passageiro_t *passageiro, int id);
void *passageiroRun(void *param);
void subirNoOnibus(passageiro_t *passageiro, onibus_t *onibus);
void caminharAtePonto(passageiro_t *this, pontoOnibus_t *pontoOnibus);
bool devoDescerDoOnibus(passageiro_t *this, pontoOnibus_t *pontoOnibus);
bool todosPassageirosChegaram();
void pegarOnibusOrigemDestino(passageiro_t *this);

#endif // __PASSAGEIRO_H__