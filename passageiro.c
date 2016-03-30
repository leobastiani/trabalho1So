#include "passageiro.h"
#include "misc.h"

void *passageiroRun(void *param) {
	int id = cast(int, param);
	debug("passageiro iniciado: %d\n", id);
}