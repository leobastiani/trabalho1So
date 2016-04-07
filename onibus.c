#include "onibus.h"
#include "passageiro.h"
#include "pontoOnibus.h"
#include "misc.h"
#include "main.h"


/**
 * função necessária para ser chamada antes do Run
 * inicializa um onibus
 * com o proximo ponto que ele deve ir
 * ou seja, todos os pontos começam circulando
 */
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
	debug("onibus %2d iniciado, indo para %2d\n", id, iPontoOnibus);
}


/**
 * função que deve ser utilizada como argumento em pthread_create
 * contem os passos que um onibus deve fazer
 * que é:
 * ir para um ponto
 * esperar os passageiros embarcarem
 * ir para o próximo ponto
 */
void *onibusRun(void *param) {
	int id = cast(int, param);
	onibus_t *this = &onibusArray[id];
	debug("onibus executando: %d\n", id);

	// definindo a seed desse ponto
	srand(filaGet(seeds, unsigned int));
	sem_post(&semDepoisDePegarSeed);

	while(true) {
		onibusIrParaPonto(this, this->proxPonto);

		// e agora vai para o proximo ponto
		debug("onibus %2d aguradando embarque\n", this->id);
		sem_wait(&this->semAguardaEmbarque);
		debug("onibus %2d foi liberado\n", this->id);
	}

}


/**
 * função chamada assim que um onibus se encerra
 * serve para dar free nas variaveis ou simplesmente encerrar-la
 */
void onibusFinish(onibus_t *this) {
	// libero a lista de passageiros
	freeList(this->passageiros);

	debug("onibus %2d encerrado\n", this->id);
}


/**
 * diz se um onibus está cheio
 */
bool onibusCheio(onibus_t *this) {
	// cada onibus tem um limite de passageiros,
	// verifica se estou nesse limite
	return this->passageiros->length == A;
}


/**
 * função que simula o trajeto de um onibus
 * calcula o tempo necessário para ele chegar no ponto
 * e aguarda até sua chegada
 */
void onibusIrParaPonto(onibus_t *this, pontoOnibus_t *pontoOnibus) {

	// em segundos
	double tempoEspera = randMinMaxD(5, 7);
	debug("onibus %2d indo para o ponto %2d, demorará %g segundos\n", this->id, pontoOnibus->id, tempoEspera);

	// convertido em microssegundos
	usleep(tempoEspera * 1E6 * fatorTempo);

	// cheguei no ponto
	debug("onibus %2d chegou no ponto %2d em %g s\n", this->id, pontoOnibus->id, segundosFicticios());
	// devo verificar se o ponto está ocupado
	// dois onibus nao podem verificar esse ponto simultaneamente
	pthread_mutex_lock(&pontoOnibus->mutexVerificarOnibusPonto);
	if(pontoOnibus->onibus) {
		// estava ocupado
		debug("onibus %2d encontrou o ponto %2d ocupado por onibus %2d, se encaminhando para o proximo ponto %2d, em %g segundos\n", this->id, pontoOnibus->id, pontoOnibus->onibus->id, getProxPonto(pontoOnibus)->id, segundosFicticios());

		// já tomei minha decisão
		pthread_mutex_unlock(&pontoOnibus->mutexVerificarOnibusPonto);
		onibusIrParaPonto(this, getProxPonto(pontoOnibus));
		return ;
	}

	// consegui parar nesse ponto
	this->pontoOnibus = pontoOnibus;
	this->proxPonto = getProxPonto(pontoOnibus);
	
	debug("onibus %2d conseguiu parar no ponto %2d de fato em %g s, proximo ponto %2d\n", this->id, pontoOnibus->id, segundosFicticios(), this->proxPonto->id);

	avisarQueOnibusChegou(pontoOnibus, this);
}