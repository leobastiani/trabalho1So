#include "pontoOnibus.h"
#include "onibus.h"
#include "passageiro.h"
#include "misc.h"
#include "main.h"


void pontoOnibusInit(pontoOnibus_t *this, int id) {
	debug("ponto de ônibus %d iniciado.\n", id);
	// zero todo o ponto
	memset(this, 0, sizeof(pontoOnibus_t));
	// defino o id do pontoOnibus
	this->id = id;
	// crio a lista de passageiros desse ponto
	this->passageiros = createList();
}



void *pontoOnibusRun(void *param) {
	int id = cast(int, param);
	debug("pontoOnibus executando: %d\n", id);

	// pega todos os passageiros e fazem eles subirem no onibus
	pontoOnibus_t *this = &pontosOnibus[id];
	
	if(this->onibus) {
		// se tem um onibus nesse ponto
		passageiro_t *passageiro;
		forList(passageiro_t *, passageiro, this->passageiros) {
			debug("analisando passageiro %d\n", passageiro->id);
			subirNoOnibus(passageiro, this->onibus);
		}
	}

}


int getProxPonto(int pontoAnterior) {
	// lista circular
	// exemplo:
	// S = 3
	// resultado: 0, 1, 2, 0, 1, ...
	return (pontoAnterior + 1) % S;
}