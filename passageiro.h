#ifndef __PASSAGEIRO_H__
#define __PASSAGEIRO_H__


/**
 * Vamos fazer uma analogia em um dia de serviço
 * todo passageiro começa em casa
 * vai para o ponto proximo de sua casa
 * chega no trabalho
 * volta para o ponto proximo do trabalho
 * e volta para casa
 */


#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>


typedef struct pontoOnibus_t pontoOnibus_t;
typedef struct onibus_t onibus_t;


typedef struct passageiro_t {
	int id;

	// arquivo de trace desse passageiro
	FILE *file;

	// onibus que estou
	onibus_t *onibus;
	pontoOnibus_t *pontoOrigem, *pontoDestino;
	// ponto que o passageiro se encontra
	pontoOnibus_t *pontoOnibus;

	// é o status de cada passageiro, ele pode estar esperando, entrando no onibus, descendo
	int status;
	#define STATUS_emCasa             0
	#define STATUS_noPontoDeCasa      1
	#define STATUS_aCaminhoDoTrabalho 2
	#define STATUS_noTrabalho         3
	#define STATUS_noPontoDoTrabalho  4
	#define STATUS_aCaminhoDeCasa     5
	#define STATUS_deVoltaPontoEmCasa 6

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
void pegarOnibusOrigemDestino(passageiro_t *this);
void gravarTrace(passageiro_t *this);
void passageiroFinish(passageiro_t *this);

#endif // __PASSAGEIRO_H__