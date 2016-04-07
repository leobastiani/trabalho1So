#ifndef __PONTOONIBUS_H__
#define __PONTOONIBUS_H__

#include <pthread.h>
#include <semaphore.h>

typedef struct list_t list_t;
typedef struct onibus_t onibus_t;

typedef struct pontoOnibus_t {
	int id;
	// onibus presente nesse ponto atualmente
	onibus_t *onibus;

	// lista de passageiros presentes nesse ponto
	list_t *passageiros;

	// semáforo que fica aguardando um ônibus
	sem_t semAguardandoOnibus;


	/**
	 * variavel que faz um único por vez onibus conseguir
	 * verificar se tem um onibus naquele ponto
	 */
	pthread_mutex_t mutexVerificarOnibusPonto;

} pontoOnibus_t;


void *pontoOnibusRun(void *param);
void pontoOnibusInit(pontoOnibus_t *pontoOnibus, int id);
pontoOnibus_t *getProxPonto(pontoOnibus_t *pontoAnterior);
void avisarQueOnibusChegou(pontoOnibus_t *this, onibus_t *onibus);
void pontoOnibusFinish(pontoOnibus_t *this);

#endif // __PONTOONIBUS_H__