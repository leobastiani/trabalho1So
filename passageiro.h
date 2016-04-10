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


typedef struct pontoOnibus_t pontoOnibus_t;	//Struct que representa um ponto de ônibus
typedef struct onibus_t onibus_t;			//Struct que reprsenta um ônibus

//Struct que representa um passagerio (o q um passageiro tem)
typedef struct passageiro_t {
	int id;	//Número de identificação

	// arquivo de trace desse passageiro
	FILE *file;

	// ponteiro para o struct que representa o onibus que ele está
	onibus_t *onibus;
	// ponteiro para a struct que representa o ponto de origem e de destino, respectivamente
	pontoOnibus_t *pontoOrigem, *pontoDestino;
	// ponteiro para a struct que representa o ponto que o passageiro se encontra no momento
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

	// UP quando (?) estou no ponto e devo esperar o onibus
	sem_t semEsperarOnibusChegar;	//Semáforo (mutex?)
} passageiro_t;

//Struct de parâmetros que devem ser passador para passageiroRun
typedef struct passageiro_param_t {
	int id;				//ID do passageiro
	int iPontoOrigem;	//ID do ponto de origem
	int iPontoDestino;	//ID do ponto de destino
} passageiro_param_t;


/**
 * função necessária para ser chamada antes do Run
 * inicializa um passageiro, mas não define seu ponto de origem
 * e de destino
 * isso deve ser resgatado na função Run pelo parametro
 */
void passageiroInit(passageiro_t *this, int id);


/**
 * devo fazer free de param (instanciado dinamicamente em main)
 * função que deve ser utilizada como argumento em pthread_create
 * passos que o passageiro deve fazer
 */
void *passageiroRun(void *param);	//Atua sobre um passageiro (uma struct passageiro_t)

//Essa função usa as seguintes outras, passado o ponteiro para a struct do seu passageiro:


/**
 * função chamada por passageiroRun, assim q um passageiro se encerra 
 */
void passageiroFinish(passageiro_t *this);	//this eh um ponteiro para a struct que representa o passageiro que se quer finalizar



/**
 * requisita que um passageiro suba no onibus
 */
void subirNoOnibus(passageiro_t *this, onibus_t *onibus);	//Passa um ponteiro para a struct que representa o passageiro
															//que quer subir e um ponteiro para a struct que representa
															//o ônibus no qual o passageiro quer subir.



/**
 * simula tanto a chegada de um passageiro até o ponto
 * como a ociosidade do passageiro no ponto de trabalho dele
 * poe o passageiro num ponto de onibus
 */
void caminharAtePonto(passageiro_t *this, pontoOnibus_t *pontoOnibus);



/**
 * executa tudo o que um passageiro precisa para pegar um onibus até o trabalho
 * que é:
 * ir para um ponto
 * pegar um onibus
 * descer no destino
 */
void pegarOnibusOrigemDestino(passageiro_t *this);


/**
 * salva o estado atual do passageiro no arquivo de trace
 */
void atualizarStatus(passageiro_t *this);


#endif // __PASSAGEIRO_H__