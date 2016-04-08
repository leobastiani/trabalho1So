#ifndef __TELA_H__
#define __TELA_H__

#include <unistd.h>

typedef struct pontoOnibus_t pontoOnibus_t;
typedef struct onibus_t onibus_t;
typedef struct passageiro_t passageiro_t;

#define telaLinha 29
#define telaColuna 118
// é 118 + 1 do \0
char tela[telaLinha][telaColuna+1];

// posições da tela pela linha e coluna
int telaLin, telaCol;

char *telaPtr;

/**
 * função que deve ser utilizada como argumento em pthread_create
 * faz a tela ser executada
 */
void *telaRun(void *param);

//funçao para escrever na tela
void writeTela(char *str, int lin, int col);

//funçao para executar a função 'imprimirPontoOnibus' para todos os onibus
void imprimirTodosPontosOnibus();

//função para imprimir os dados do ponto de onibus como id do ponto, passageiro na fila de espera e onibus que esta no ponto
void imprimirPontoOnibus(pontoOnibus_t *this);

//funçao para executa a funçao imprimirOnibus para todos os onibus
void imprimirTodosOnibus();

//funçao para imprimir os dados do ponto de onibus como por exemplo id do onibus, situaçao do onibus (parado ou andando), onde esta, para onde vai e passageiro no onibus
void imprimirOnibus(onibus_t *this);

void telaFinish();




#endif // __TELA_H__