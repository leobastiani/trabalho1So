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

	// assim que eu dou um down, fico esperando
	// começa em zero, pq o próximo wait eu fico bloqueado
	sem_init(&this->semEsperarOnibusChegar, 0, 0);

	debug("passageiro %2d iniciado\n", this->id);
}


/**
 * devo fazer free de param
 */
void *passageiroRun(void *param) {
	passageiro_param_t *paramConvertido = cast(passageiro_param_t *, param);
	passageiro_t *this = &passageiros[paramConvertido->id];

	// defino todas as variaveis que depende do parametro
	this->pontoOrigem = &pontosOnibus[paramConvertido->iPontoOrigem];
	this->pontoDestino = &pontosOnibus[paramConvertido->iPontoDestino];
	// ja posso liberá-lo
	free(paramConvertido);

	// cada thread deve fazer um srand
	// resgatando da lista de seeds
	srand(removeInicioList(seeds, int));


	debug("passageiro executando: %d, origem: %2d, destino: %2d\n", this->id, this->pontoOrigem->id, this->pontoDestino->id);

	// o que cada passageiro deve fazer:
	pegarOnibusOrigemDestino(this);
	
	// estou fora da fila do ponto
	// devo trocar o ponto de origem com o de destino
	{
		pontoOnibus_t *swap;
		swap = this->pontoDestino;
		this->pontoDestino = this->pontoOrigem;
		this->pontoOrigem = swap;
	}

	// começo tudo de novo
	pegarOnibusOrigemDestino(this);



	passageirosConcluidos++;
	debug("passageiro %2d encerrado\n", this->id);
}



/**
 * requisita que um passageiro suba no onibus
 */
void subirNoOnibus(passageiro_t *this, onibus_t *onibus) {
	// insere este passageiro na fila do onibus
	filaPush(onibus->passageiros, this);

	this->onibus = onibus;
}



/**
 * só dar um sleep e simula a caminhada até o ponto
 */
void caminharAtePonto(passageiro_t *this, pontoOnibus_t *pontoOnibus) {
	// em segundos
	double tempoEspera = randMinMaxD(0, 60);
	debug("passageiro %2d indo para o ponto %d em %g segundos\n", this->id, pontoOnibus->id, tempoEspera);

	// convertido em microssegundos
	usleep(tempoEspera * 1E6 * fatorTempo);

	// cheguei no ponto
	debug("passageiro %2d chegou no ponto %2d, esperou por %g s\n", this->id, pontoOnibus->id, tempoEspera);
	// devo me por na fila de passageiros dentro do ponto de onibus
	filaPush(pontoOnibus->passageiros, this);
	this->pontoOnibus = pontoOnibus;
}



bool todosPassageirosChegaram() {
	return passageirosConcluidos == P;
}


void pegarOnibusOrigemDestino(passageiro_t *this) {
	// primeiro, vai para o ponto de origem
	caminharAtePonto(this, this->pontoOrigem);
	// vou esperar o onibus e pegá-lo
	// depois vou descer e esperar receber um sinal para descer
	// por tanto, enquanto não chegar no ponto, eu fico no onibus
	while(true) {
		debug("passageiro %2d esperando o onibus andar\n", this->id);
		sem_wait(&this->semEsperarOnibusChegar);
		// o onibus chegou a um ponto
		onibus_t *onibus = this->onibus;
		pontoOnibus_t *pontoOnibus = onibus->pontoOnibus;
		if(this->pontoDestino != pontoOnibus) {
			// não quero descer nesse ponto
			debug("passageiro %2d chegou no ponto %2d, mas não saiu do bus, ele quer ir para %2d\n", this->id, pontoOnibus->id, this->pontoDestino->id);
			// terminada a verificação
			sem_post(&onibus->semTodosPassageirosConferiram);
			continue;
		}

		// eu qro descer nesse ponto
		debug("passageiro %2d chegou no destino %2d\n", this->id, pontoOnibus->id);
		// devo sair desse ônibus
		passageiro_t *passageiro;
		forList(passageiro_t *, passageiro, onibus->passageiros) {
			if(this == passageiro) {
				removeListInFor(onibus->passageiros);
			}
		}
		
		sem_post(&onibus->semTodosPassageirosConferiram);
		break;
	}

}