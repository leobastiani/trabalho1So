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

	// inicializa os semaphores
	sem_init(&this->semAguardandoOnibus, 0, 0);
}



void *pontoOnibusRun(void *param) {
	int id = cast(int, param);
	debug("pontoOnibus executando: %d\n", id);

	pontoOnibus_t *this = &pontosOnibus[id];

	srand(removeInicioList(seeds, int));

	while(true) {
		if(todosPassageirosChegaram()) {
			// todos os passageiros foram concluidos
			break;
		}

		// fica aguardando um ônibus chegar
		debug("ponto de onibus %2d aguardando um onibus chegar\n", this->id);
		sem_wait(&this->semAguardandoOnibus);

		// onibus atual no ponto
		onibus_t *onibus = this->onibus;

		debug("ponto de onibus %2d percebeu que chegou o onibus %2d com %2d passageiros\n", this->id, onibus->id, onibus->passageiros->length);

		// fica um tempinho esperando
		// simulando os passageiros subindo
		// se não tem ngm pra subir, nem ngm pra descer
		bool temAlguem = onibus->passageiros->length != 0 || this->passageiros->length != 0;
		if(temAlguem) {
			double tempoEspera = randMinMaxD(3, 8);
			usleep(tempoEspera * 1E6 * fatorTempo);
		}


		passageiro_t *passageiro;
		// descarrego os passageiros nesse ponto
		int nPassageirosEsperar = onibus->passageiros->length;
		forList(passageiro_t *, passageiro, onibus->passageiros) {
			// aviso ao passageiro para descer
			sem_post(&passageiro->semEsperarOnibusChegar);
			debug("despachando do ponto %2d o passageiro %2d\n", this->id, passageiro->id);
		}

		// espero todos os passageiros descerem
		for(int i=0; i<nPassageirosEsperar; i++) {
			sem_wait(&onibus->semTodosPassageirosConferiram);
		}
		debug("ponto de onibus %2d recebeu a resposta de todos os passageiros\n", this->id);
		

		// enche o onibus com os passageiros presentes nesse ponto
		if(this->passageiros->length == 0) {
			debug("não havia ninguem no ponto de onibus %2d para subir\n", this->id);
		}
		forList(passageiro_t *, passageiro, this->passageiros) {
			// aviso ao passageiro que um ônibus chegou
			if(onibusCheio(onibus)) {
				// não posso mais enchê-lo
				break;
			}

			// remove ele da fila do onibus
			removeListInFor(this->passageiros);
			// manda ele subir no bus
			subirNoOnibus(passageiro, onibus);
		}
		if(onibus->passageiros->length != 0) {
			// onibus está partindo com pessoas
			#ifdef DEBUG
				debug("onibus %2d partindo do ponto %2d com %2d passageiros: ", onibus->id, this->id, onibus->passageiros->length);
				printList(passageiro_t *, passageiro, onibus->passageiros, printf("%d", passageiro->id), ", ", "\n");
				fflush(stdout);
			#endif // DEBUG
		}

		// liberando o onibus
		debug("liberando o onibus %2d do ponto %2d\n", onibus->id, this->id);
		sem_post(&onibus->semAguardaEmbarque);
		onibus->pontoOnibus = NULL;
		this->onibus = NULL;
		debug("ponto de onibus %2d liberou o onibus %2d com %2d passageiros\n", this->id, onibus->id, onibus->passageiros->length);
	}



	debug("ponto de onibus %2d encerrado\n", this->id);

}


pontoOnibus_t *getProxPonto(pontoOnibus_t *pontoAnterior) {
	// lista circular
	// exemplo:
	// S = 3
	// resultado: 0, 1, 2, 0, 1, ...
	return &pontosOnibus[(pontoAnterior->id + 1) % S];
}


void avisarQueOnibusChegou(pontoOnibus_t *this, onibus_t *onibus) {
	// diz para o ponto que esse onibus está lá
	this->onibus = onibus;
	// avisa ao ponto que um ônibus chegou
	sem_post(&this->semAguardandoOnibus);
}