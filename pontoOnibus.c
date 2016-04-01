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

	pontoOnibus_t *this = &pontosOnibus[id];
}


int getProxPonto(int pontoAnterior) {
	// lista circular
	// exemplo:
	// S = 3
	// resultado: 0, 1, 2, 0, 1, ...
	return (pontoAnterior + 1) % S;
}