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


/**
 * função necessária para ser chamada antes do Run
 * inicializa a um ponto de onibus
 */
void pontoOnibusInit(pontoOnibus_t *this, int id);


/**
 * função que deve ser utilizada como argumento em pthread_create
 * o que cada ponto de onibus deve fazer
 */
void *pontoOnibusRun(void *param);


/**
 * função chamada assim que um ponto de onibus se encerra
 * serve para dar free nas variaveis ou simplesmente encerrar-la
 */
void pontoOnibusFinish(pontoOnibus_t *this);


/**
 * obtem o proximo ponto de onibus a partir do anterior ou atual
 */
pontoOnibus_t *getProxPonto(pontoOnibus_t *pontoAnterior);


/**
 * permite a um ponto de onibus voltar a execução
 * e assim executar suas funções
 */
void avisarQueOnibusChegou(pontoOnibus_t *this, onibus_t *onibus);



#endif // __PONTOONIBUS_H__