#include "onibus.h"
#include "pontoOnibus.h"
#include "misc.h"
#include "main.h"



void onibusInit(onibus_t *this, int id) {
	// zero todo o ponto
	memset(this, 0, sizeof(onibus_t));
	// defino o id do onibus
	this->id = id;
	// crio a lista de passageiros desse ponto
	this->passageiros = createList();

	// tenho que por cada ônibus em um ponto especifico
	int pontoAtualInt = randMinMax(0, S-1);
	int proxPonto = 0;
	pontoOnibus_t *pontoAtual = &pontosOnibus[pontoAtualInt];
	// tenta por esse ônibus nesse ponto
	if(pontoAtual->onibus == NULL) {
		// se este ponto não tem onibus
		pontoAtual->onibus = this;
		this->pontoOnibus = pontoAtual;
	} else {
		// se aquele ponto ja estava ocupado
		pontoAtual = NULL;
		this->pontoOnibus = NULL;
		// coloca ele se locomovendo
		proxPonto = getProxPonto(pontoAtualInt);
		this->proxPonto = &pontosOnibus[proxPonto];
	}


	// imprimindo variaveis para teste
	#ifdef DEBUG
		if(pontoAtual) {
			debug("ônibus %2d iniciado, parado em %2d\n", id, pontoAtualInt);
		} else {
			debug("ônibus %2d iniciado, %d ocupado, se locomovendo para %d\n", id, pontoAtualInt, 0);
		}
	#endif // DEBUG
}


void *onibusRun(void *param) {
	int id = cast(int, param);
	debug("onibus executando: %d\n", id);
}



bool onibusCheio(onibus_t *this) {
	// cada onibus tem um limite de passageiros,
	// verifica se estou nesse limite
	return this->passageiros->length == A;
}