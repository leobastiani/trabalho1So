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


void writeTela(char *str, int lin, int col) {
	char ch;
	int i = 0;
	while(ch = str[i]) {
		tela[lin][col+i] = ch;
		
		i++;
	}
}


void imprimirTodosPontosOnibus() {
	for(int i=0; i<S; i++) {
		imprimirPontoOnibus(&pontosOnibus[i]);
	}
}


void imprimirPontoOnibus(pontoOnibus_t *this) {
	char buffer[1000];

	// [S00 100]
	sprintf(buffer, "[S%02d %03d]", this->id, this->passageiros->length);
	writeTela(buffer, this->id * 4, 0);

	// [C00]
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

		
	forList(passageiro_t *, passageiro, this->passageiros) {
		if (i==30){
			break;
		}
		sprintf(buffer, "%03d", passageiro->id);
		writeTela(buffer, (this->id * 4) + i/10, 10 + 4 * (i % 10));
		i++;
	}

}

void imprimirTodosOnibus() {
	for(int i=0; i<C; i++) {
		imprimirOnibus(&onibusArray[i]);
	}
}

void imprimirOnibus(onibus_t *this) {
	char buffer[1000];

	//[C00 99]
	sprintf(buffer, "[C%02d %03d]", this->id, this->passageiros->length);
	#define colunaOnibus 55
	writeTela(buffer, this->id * 4, colunaOnibus);
	passageiro_t *passageiro;
	int i = 0;

	forList(passageiro_t*, passageiro, this->passageiros) {
		if (i==30){
			break;
		}
		sprintf(buffer, "%03d", passageiro->id);
		writeTela(buffer, this->id * 4 + i/10, colunaOnibus + 10 + 4 * (i % 10));
		i++;
	}

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