#include "onibus.h"
#include "passageiro.h"
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
	int iPontoOnibus = randMinMax(0, S-1);
	// define que este ônibus está indo para este ponto
	this->proxPonto = &pontosOnibus[iPontoOnibus];

	// deixa o onibus aguradando o embarque quando ele chega no ponto
	sem_init(&this->semAguardaEmbarque, 0, 0);
	sem_init(&this->semTodosPassageirosConferiram, 0, 0);

	// imprimindo variaveis para teste
	debug("ônibus %2d iniciado, indo para %2d\n", id, iPontoOnibus);
}


void *onibusRun(void *param) {
	int id = cast(int, param);
	onibus_t *this = &onibusArray[id];
	debug("onibus executando: %d\n", id);

	// definindo a seed desse ponto
	srand(removeInicioList(seeds, int));

	onibusIrParaPonto(this, this->proxPonto);
	
	while(true) {
		if(todosPassageirosChegaram()) {
			// todos os passageiros foram concluidos
			break;
		}

		// e agora vai para o proximo ponto
		debug("onibus %2d aguradando embarque\n", this->id);
		sem_wait(&this->semAguardaEmbarque);
		debug("onibus %2d foi liberado\n", this->id);

		onibusIrParaPonto(this, getProxPonto(this->proxPonto));
	}


	debug("onibus %2d encerrado\n", this->id);
}



bool onibusCheio(onibus_t *this) {
	// cada onibus tem um limite de passageiros,
	// verifica se estou nesse limite
	return this->passageiros->length == A;
}


void onibusIrParaPonto(onibus_t *this, pontoOnibus_t *pontoOnibus) {
	if(todosPassageirosChegaram()) {
		// não preciso mais continuar
		return ;
	}

	// em segundos
	double tempoEspera = randMinMaxD(30, 40);
	debug("onibus %2d indo para o ponto %d em %g segundos\n", this->id, pontoOnibus->id, tempoEspera);

	// convertido em microssegundos
	usleep(tempoEspera * 1E6 * fatorTempo);

	// cheguei no ponto
	debug("onibus %2d chegou no ponto %2d, esperou por %g s\n", this->id, pontoOnibus->id, tempoEspera);
	// devo verificar se o ponto está ocupado
	if(pontoOnibus->onibus) {
		// estava ocupado
		debug("onibus %2d encontrou o ponto %2d ocupado por onibus %2d, se encaminhando para o proximo ponto\n", this->id, pontoOnibus->id, pontoOnibus->onibus->id);
		onibusIrParaPonto(this, getProxPonto(pontoOnibus));
		return ;
	}

	// consegui parar nesse ponto
	this->pontoOnibus = pontoOnibus;
	avisarQueOnibusChegou(pontoOnibus, this);
}