#include "passageiro.h"
#include "pontoOnibus.h"
#include "onibus.h"
#include "misc.h"
#include "main.h"


/**
 * função necessária para ser chamada antes do Run
 * inicializa um passageiro, mas não define seu ponto de origem
 * e de destino
 * isso deve ser resgatado na função Run pelo parametro
 */
void passageiroInit(passageiro_t *this, int id) {	//Coloca na struct o ID do passageiro e o arquivo de Trace
													//e inicia semáforo semEsperarOnibusChegar com 0
	// zero todo o ponto
	memset(this, 0, sizeof(passageiro_t));
	// defino o id do passageiro
	this->id = id;

	// todo passageiro se inicia em casa
	this->status = STATUS_emCasa;

	// abre seu arquivo de trace
	// obtem primeiro o nome do trace
	char fileName[30] = "passageiroXX.trace";
	sprintf(fileName, "passageiro%02d.trace", id);
	#ifdef DEBUG
		// estou no debug
		this->file = NULL;
	#else
		// não estou no debug
		this->file = fopen(fileName, "w");
	#endif // DEBUG

	// assim que eu dou um down, fico esperando
	// começa em zero, pq o próximo wait eu fico bloqueado
	sem_init(&this->semEsperarOnibusChegar, 0, 0);	//Semáforo iniciado em 0

	debug("passageiro %2d iniciado\n", this->id);
}


/**
 * devo fazer free de param
 * função que deve ser utilizada como argumento em pthread_create
 * passos que o passageiro deve fazer
 */
void *passageiroRun(void *param) {
	// vamos trabalhar com o parametro rapidamente para podermos liberá-lo
	passageiro_param_t *paramConvertido = cast(passageiro_param_t *, param);	//Cast, pois pthread_create soh passa como
																				//parâmetro ponteiro para void
	passageiro_t *this = &passageiros[paramConvertido->id];

	// defino todas as variaveis que depende do parametro
	this->pontoOrigem = &pontosOnibus[paramConvertido->iPontoOrigem];
	this->pontoDestino = &pontosOnibus[paramConvertido->iPontoDestino];
	// ja posso liberá-lo
	free(paramConvertido);

	// cada thread deve fazer um srand
	// resgatando da lista de seeds
	srand(filaGet(seeds, unsigned int));
	sem_post(&semDepoisDePegarSeed);


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



	passageiroFinish(this);
}


/**
 * função chamada assim q um passageiro se encerra
 */
void passageiroFinish(passageiro_t *this) {
	// fechando o arquivo
	fclose(this->file);

	debug("passageiro %2d encerrado\n", this->id);
}



/**
 * requisita que um passageiro suba no onibus
 */
void subirNoOnibus(passageiro_t *this, onibus_t *onibus) {
	// insere este passageiro na fila do onibus
	filaPush(onibus->passageiros, this);

	this->onibus = onibus;
	// agora que eu subi, avanço no status
	atualizarStatus(this);
}



/**
 * simula tanto a chegada de um passageiro até o ponto
 * como a ociosidade do passageiro no ponto de trabalho dele
 * poe o passageiro num ponto de onibus
 */
void caminharAtePonto(passageiro_t *this, pontoOnibus_t *pontoOnibus) {
	// em segundos
	double tempoEspera = randMinMaxD(5, 15);
	debug("passageiro %2d indo para o ponto %d, demorara %g segundos\n", this->id, pontoOnibus->id, tempoEspera);

	// convertido em microssegundos
	usleep(tempoEspera * 1E6 * fatorTempo);

	// cheguei no ponto
	debug("passageiro %2d chegou no ponto %2d, indo para %2d, agora sao %g s\n", this->id, pontoOnibus->id, this->pontoDestino->id, segundosFicticios());
	// devo me por na fila de passageiros dentro do ponto de onibus
	filaPush(pontoOnibus->passageiros, this);
	this->pontoOnibus = pontoOnibus;
}



/**
 * executa tudo o que um passageiro precisa para pegar um onibus até o trabalho
 * que é:
 * ir para um ponto
 * pegar um onibus
 * descer no destino
 */
void pegarOnibusOrigemDestino(passageiro_t *this) {
	// primeiro, vai para o ponto de origem
	caminharAtePonto(this, this->pontoOrigem);
	// avanço na etapa do status
	atualizarStatus(this);
	// vou esperar o onibus e pegá-lo
	// depois vou descer e esperar receber um sinal para descer
	// por tanto, enquanto não chegar no ponto, eu fico no onibus
	while(true) {
		if(this->onibus) {
			// se estou num onibus
			debug("passageiro %2d esperando o onibus %2d andar\n", this->id, this->onibus->id);
		} else {
			// não estou num ônibus
			debug("passageiro %2d esperando o onibus chegar, estou no ponto %2d\n", this->id, this->pontoOnibus->id);
		}
		sem_wait(&this->semEsperarOnibusChegar);
		debug("passageiro %2d percebeu que o onibus\n", this->id);
		// o onibus chegou a um ponto
		// this->onibus foi definido em subirNoOnibus, que está sendo chamado em pontoOnibus.c
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
		debug("passageiro %2d chegou no destino %2d, desceu do onibus %2d\n", this->id, pontoOnibus->id, onibus->id);
		this->pontoOnibus = pontoOnibus;
		// incrimento o status
		atualizarStatus(this);

		// devo sair desse ônibus
		passageiro_t *passageiro;
		pthread_mutex_lock(&onibus->passageiros->mutexForLista);

		// simula um passageiro descendo
		usleep(0.8 * 1E6 * fatorTempo);

		forList(passageiro_t *, passageiro, onibus->passageiros) {
			if(this == passageiro) {
				removeListInFor(onibus->passageiros);
				break;
			}
		}
		pthread_mutex_unlock(&onibus->passageiros->mutexForLista);
		

		sem_post(&onibus->semTodosPassageirosConferiram);
		break;
	}

}


/**
 * salva o estado atual do passageiro no arquivo de trace
 */
void atualizarStatus(passageiro_t *this) {
	// arquivo que devo salvar
	FILE *file = this->file;

	// atualizo o status
	this->status++;
	debug("passageiro %2d status alterado para: %d\n", this->id, this->status);

	#ifdef DEBUG
		// estou no debug
		// não quero ler os arquivos de trace
		return ;
	#endif // DEBUG

	int status = this->status;

	// se eu cheguei num ponto
	if(status == STATUS_noPontoDeCasa) {
		fprintf(file, "Chegou no ponto de ônibus de origem %02d às %s.\n", this->pontoOnibus->id, horaAtual());
	}

	if(status == STATUS_noPontoDoTrabalho || status == STATUS_deVoltaPontoEmCasa) {
		fprintf(file, "Desceu do ônibus %02d às %s.\n", this->onibus->id, horaAtual());
		fprintf(file, "Desceu no ponto de ônibus %02d.\n", this->pontoOnibus->id);
	}

	// se eu subi num ônibus
	else if(status == STATUS_aCaminhoDeCasa || status == STATUS_aCaminhoDoTrabalho) {
		fprintf(file, "Entrou no ônibus %02d às %s.\n", this->onibus->id, horaAtual());
	}

}