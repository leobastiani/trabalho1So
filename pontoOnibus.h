#ifndef __PONTOONIBUS_H__
#define __PONTOONIBUS_H__

typedef struct list_t list_t;
typedef struct onibus_t onibus_t;

typedef struct pontoOnibus_t {
	// onibus presente nesse ponto atualmente
	onibus_t *onibus;

	// lista de passageiros presentes nesse ponto
	list_t *passageiros;
} pontoOnibus_t;


void *pontoOnibusRun(void *param);

#endif // __PONTOONIBUS_H__