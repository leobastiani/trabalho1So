#ifndef __ONIBUS_H__
#define __ONIBUS_H__


#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>



// devem existir
typedef struct list_t list_t;
typedef struct pontoOnibus_t pontoOnibus_t;


typedef struct onibus_t {
	int id;
	// lista de passageiros no ônibus
	list_t *passageiros;

	// ponteiro para o ponto de ônibus atual
	pontoOnibus_t *pontoOnibus;
	pontoOnibus_t *proxPonto;

	sem_t semAguardaEmbarque;
	sem_t semTodosPassageirosConferiram;
} onibus_t;



void onibusInit(onibus_t *onibus, int id);
void *onibusRun(void *param);
bool onibusCheio(onibus_t *onibus);
void onibusIrParaPonto(onibus_t *this, pontoOnibus_t *pontoOnibus);

#endif // __ONIBUS_H__