#ifndef __MAIN_H__
#define __MAIN_H__

#include <pthread.h>
#include <semaphore.h>


// declarações
typedef struct onibus_t onibus_t;			//Define um ônibus
typedef struct passageiro_t passageiro_t;	//Define um passageiro
typedef struct pontoOnibus_t pontoOnibus_t;	//Define um ponto de ônibus
typedef struct list_t list_t;


//----------------------------------------------------
// Variaveis
//----------------------------------------------------

/**
 * são 4 argumentos no formato:
 * Há S pontos de ônibus.
 * Há C carros que representam os ônibus.
 * Há P passageiros que desejam andar de ônibus.
 * Há A assentos em cada ônibus.
 * Garanta que P >> A >> C.
 */
int S, C, P, A;
// defines com nomes mais sujestivos
#define nPontosOnibus S
#define nOnibus       C
#define nPassageiros  P
#define nAssentos     A



// é uma lista com todas as seeds necessárias
// cada thread precisa de uma seed
list_t *seeds;	//Sementes para o Rand
// variavel que serve para que eu deixe que eu
// crie uma nova thread se a seed foi obtida
sem_t semDepoisDePegarSeed;	//Semáforo mutex para acessar a liste de seeds


// diz com ql velocidade o programa vai executar
// se é 1, roda na velocidade normal
// se é 0.2, roda em apenas 20% do tempo
// é sempre multiplicado quando vou dar um sleep
#ifdef DEBUG
	// estou em teste
	// menor tempo possível é 0.05, ainda apresenta indeterminações, mas é bom
	#define fatorTempo 0.05
#else
	#define fatorTempo 1
#endif // DEBUG



// os objetos
//Lista de structs que representam os ônibus, passageiros e de pontos
onibus_t *onibusArray; // nao tem plural :/
passageiro_t *passageiros;
pontoOnibus_t *pontosOnibus;


//Listas de identificadores da threads (pthread_t) de ônibus, passageiros e de pontos de ônibus
// as threads
// são vetores
pthread_t *threadsOnibus;
pthread_t *threadsPassageiro;
pthread_t *threadsPontoOnibus;
// não são vetores
// thread da tela que fica executando de tempos em tempos
pthread_t threadTela;


//----------------------------------------------------
// Funções
//----------------------------------------------------


/**
 * retorna o momento em segundos da aplicação
 * retorna uma resposta boa msmo se eu tentar acelearar a aplicação
 * com o fatorTempo
 */
double segundosFicticios();


/**
 * retorna uma string com a hora atual do sistema, não precisa de free
 * @return formato "11h15m30s"
 */
char *horaAtual();


/**
 * Inicio da aplicação
 * os parametros são iguais aos especificados pelo pdf
 */
void run(int S_param, int C_param, int P_param, int A_param);


void usage(char *argv0);

int main(int argc, char *argv[]);

void leo(int i);

#endif // __MAIN_H__
