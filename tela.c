#include "pontoOnibus.h"
#include "onibus.h"
#include "passageiro.h"
#include "tela.h"
#include "misc.h"
#include "main.h"

/**
 * função que deve ser utilizada como argumento em pthread_create
 * faz a tela ser executada
 */
void *telaRun(void *param) {
	debug("Tela iniciado!\n");

	#ifdef DEBUG
		// não quero ver a tela se estou no debug
		return NULL;
	#endif // DEBUG

	int nLoops = 0;

	while(true) {
		debug("Tela executando\n");

		// vamos apagar tudo
		for(int lin=0; lin<telaLinha; lin++) {
			memset(tela[lin], ' ', telaColuna);
			tela[lin][telaColuna] = '\0';
		}


		imprimirTodosPontosOnibus();
		imprimirTodosOnibus();

		// imprimindo
		clearScreen();
		for(int i=0; i<telaLinha; i++) {
			printf("%s\n", tela[i]);
		}

		nLoops++;
		fflush(stdout);
		usleep(0.5 * 1E6);
	}
}

//funçao para escrever na tela
void writeTela(char *str, int lin, int col) {
	if(lin >= telaLinha || col >= telaColuna) {
		// não escreve na parte da tela q não foi alocada
		return ;
	}

	char ch;
	int i = 0;
	while(ch = str[i]) {
		tela[lin][col+i] = ch;
		
		i++;
	}
}

//funçao para executar a função 'imprimirPontoOnibus' para todos os onibus
void imprimirTodosPontosOnibus() {
	for(int i=0; i<S; i++) {
		imprimirPontoOnibus(&pontosOnibus[i]);
	}
}

//função para imprimir os dados do ponto de onibus como id do ponto, passageiro na fila de espera e onibus que esta no ponto
void imprimirPontoOnibus(pontoOnibus_t *this) {
	char buffer[1000];

	// [S00 100]
	//escreve na variavel buffer
	sprintf(buffer, "[S%02d %03d]", this->id, this->passageiros->length);
	//escreve na tela no local adequado
	writeTela(buffer, this->id * 4, 0);

	// [C00]
	//se tiver onibus neste ponto imprime o id dele, se não tiver imprime apenas colchetes vazios para indicar que não há onibus neste ponto
	if(this->onibus) {
		sprintf(buffer, "[C%02d]", this->onibus->id);
		writeTela(buffer, this->id * 4 + 2, 1);
	}
	// não tem onibus
	else {
		writeTela("[   ]", this->id * 4 + 2, 1);
	}


	int i = 0;
	passageiro_t *passageiro;

	//percorre todos os passageiros neste ponto 	
	forList(passageiro_t *, passageiro, this->passageiros) {
		//não imprime mais que 30 passageiros, apesar de inseri-los no ponto onibus
		if (i==30){
			break;
		}
		//imprime o passageiro no local adequado
		sprintf(buffer, "%03d", passageiro->id);
		writeTela(buffer, (this->id * 4) + i/10, 10 + 4 * (i % 10));
		i++;
	}

}

//funçao para executa a funçao imprimirOnibus para todos os onibus
void imprimirTodosOnibus() {
	for(int i=0; i<C; i++) {
		imprimirOnibus(&onibusArray[i]);
	}
}

//funçao para imprimir os dados do ponto de onibus como por exemplo id do onibus, situaçao do onibus (parado ou andando), onde esta, para onde vai e passageiro no onibus
void imprimirOnibus(onibus_t *this) {
	char buffer[1000];

	//[C00 99]
	//escreve no buffer o id do onibus e o numero de passageiros no onibus
	sprintf(buffer, "[C%02d %03d]", this->id, this->passageiros->length);
	#define colunaOnibus 55
	//escreve na tela o que esta no buffer
	writeTela(buffer, this->id * 4, colunaOnibus);
	passageiro_t *passageiro;
	int i = 0;

	//percorre os passageiros do onibus
	forList(passageiro_t*, passageiro, this->passageiros) {
		//nao imprime mais que 30 passageiros, apesar de inseri-los no onibus
		if (i==30){
			break;
		}
		//imprime os passageiros
		sprintf(buffer, "%03d", passageiro->id);
		writeTela(buffer, this->id * 4 + i/10, colunaOnibus + 10 + 4 * (i % 10));
		i++;
	}

	//se o onibus estiver parado num ponto de onibus imprime == antes do id do ponto, senão imprime >> antes do id do ponto
	//sao impressos no local adequado
	if (this->pontoOnibus){
		sprintf(buffer, "== [S%02d]", this->pontoOnibus->id);
	}
	else{
		sprintf(buffer, ">> [S%02d]", this->proxPonto->id);
	}
	writeTela(buffer, this->id * 4 + 1, colunaOnibus);
	
}

void telaFinish() {
	clearScreen();
	printf("Todos os passageiros concluiram seus trajetos!\n");
	printf("Tempo: %g segundos\n", segundosFicticios());
}