#include "trace.h"
#include "misc.h"


char *horaAtual() {
	static char result[30] = "11h15m30s";
	time_t timeAtual = time(NULL);
	struct tm *tmAtual = localtime(&timeAtual);

	sprintf(result, "%02dh%02dm%02ds", tmAtual->tm_hour, tmAtual->tm_min, tmAtual->tm_sec);
	return result;
}