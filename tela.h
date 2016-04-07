#ifndef __TELA_H__
#define __TELA_H__

/**
 * função que deve ser utilizada como argumento em pthread_create
 * faz a tela ser executada
 */
void *telaRun(void *param);

#endif // __TELA_H__