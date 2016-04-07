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




void onibusInit(onibus_t *onibus, int id);
void *onibusRun(void *param);
bool onibusCheio(onibus_t *onibus);
void onibusIrParaPonto(onibus_t *this, pontoOnibus_t *pontoOnibus);
void onibusFinish(onibus_t *this);

#endif // __ONIBUS_H__