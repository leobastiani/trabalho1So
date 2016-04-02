#ifndef __PASSAGEIRO_H__
#define __PASSAGEIRO_H__


#include <semaphore.h>
#include <unistd.h>


typedef struct pontoOnibus_t pontoOnibus_t;
typedef struct onibus_t onibus_t;


typedef struct passageiro_t {
	int id;

	pontoOnibus_t *pontoOrigem, *pontoDestino;

	// é o status de cada passageiro, ele pode estar esperando, entrando no onibus, descendo
	int status;

	// estou no ponto e devo esperar o onibus
	sem_t semEsperarOnibusChegar;
} passageiro_t;


void passageiroInit(passageiro_t *passageiro, int id);
void *passageiroRun(void *param);
void subirNoOnibus(passageiro_t *passageiro, onibus_t *onibus);
void irParaPonto(passageiro_t *this);

#endif // __PASSAGEIRO_H__