#ifndef __TELA_H__
#define __TELA_H__

#include <unistd.h>

typedef struct pontoOnibus_t pontoOnibus_t;
typedef struct onibus_t onibus_t;
typedef struct passageiro_t passageiro_t;

#define telaLinha 29
// é 27 + 1 do \0
#define telaColuna 118
char tela[telaLinha][telaColuna+1];

// posições da tela pela linha e coluna
int telaLin, telaCol;

char *telaPtr;

/**
 * função que deve ser utilizada como argumento em pthread_create
 * faz a tela ser executada
 */
void *telaRun(void *param);
void imprimirTodosPontosOnibus();
void imprimirPontoOnibus(pontoOnibus_t *this);
void imprimirTodosOnibus ();
void imprimirOnibus (onibus_t *this);
void telaFinish();

#endif // __TELA_H__