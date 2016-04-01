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
	// agora que eu sei o ponto em que estou
	// devo adicionar o passageiro a lista de passageiros
	inserirFinalList(this->pontoOrigem->passageiros, this);

	debug("passageiro %2d iniciado, origem %2d, destino %2d\n", this->id, pontoOrigem, pontoDestino);
}


void *passageiroRun(void *param) {
	int id = cast(int, param);
	debug("passageiro executando: %d\n", id);

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