#ifndef __ONIBUS_H__
#define __ONIBUS_H__


// devem existir
typedef struct list_t list_t;
typedef struct pontoOnibus_t pontoOnibus_t;


typedef struct onibus_t {
	// lista de passageiros no ônibus
	list_t *passageiros;

	// ponteiro para o ponto de ônibus atual
	pontoOnibus_t *pontoAtual;
} onibus_t;



void *onibusRun(void *param);

#endif // __ONIBUS_H__