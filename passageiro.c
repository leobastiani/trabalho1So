#include "passageiro.h"
#include "pontoOnibus.h"
#include "onibus.h"
#include "misc.h"
#include "main.h"



void passageiroInit(passageiro_t *this, int id) {
	// zero todo o ponto
	memset(this, 0, sizeof(passageiro_t));
	// defino o id do passageiro
	this->id = id;

	// devo definir seu ponto de partida e seu destino
	int pontoOrigem, pontoDestino;
	do {
		// procura um ponto que seja diferente de outro ponto
		pontoOrigem = randMinMax(0, S-1);
		pontoDestino = randMinMax(0, S-1);
		// se eles forem diferentes, posso sair
	} while(pontoOrigem == pontoDestino);
	this->pontoOrigem = &pontosOnibus[pontoOrigem];
	this->pontoDestino = &pontosOnibus[pontoDestino];

	debug("passageiro %2d iniciado, origem %2d, destino %2d\n", this->id, pontoOrigem, pontoDestino);
}


/**
 * devo fazer free de param
 */
void *passageiroRun(void *param) {
	int id = cast(int, param);
	passageiro_t *this = &passageiros[id];

	// cada thread deve fazer um srand
	// resgatando da lista de seeds
	int seed = removeInicioList(seeds);
	srand(seed);


	debug("passageiro executando: %d\n", id);

	// o que cada passageiro deve fazer:
	// primeiro, vai para o ponto de origem
	irParaPonto(this);


	debug("passageiro %2d encerrado\n", this->id);
}



/**
 * requisita que um passageiro suba no onibus
 */
void subirNoOnibus(passageiro_t *this, onibus_t *onibus) {
	if(!onibusCheio(onibus)) {
		// se este ônibus não está cheio, só subir
		debug("passageiro %d subindo no onibus %d\n", this->id, onibus->id);
		inserirFinalList(onibus->passageiros, this);
	} else {
		// se o onibus esta cheio
		debug("passageiro %d do ponto %d tentou subir no onibus %d que possuia %d passageiros. e voltou a aguardar\n",
			this->id, this->pontoOrigem->id, onibus->id, onibus->passageiros->length);
	}
}



/**
 * só dar um sleep e simula a caminhada até o ponto
 */
void irParaPonto(passageiro_t *this) {
	// em segundos
	double tempoEspera = randMinMaxD(0, 20);
	debug("passageiro %d indo para o ponto %d em %g segundos\n", this->id, this->pontoOrigem->id, tempoEspera);

	// convertido em microssegundos
	usleep(tempoEspera * 1000000 * fatorTempo);

	debug("passageiro %2d chegou no ponto %2d, esperou por %g s\n", this->id, this->pontoOrigem->id, tempoEspera);
}