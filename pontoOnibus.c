#include "pontoOnibus.h"
#include "misc.h"

void *pontoOnibusRun(void *param) {
	int id = cast(int, param);
	debug("pontoOnibus iniciado: %d\n", id);
}