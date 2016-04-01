#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "misc.h"
#include "main.h"



void testList() {
	list_t *list = createList();
	int removido;

	inserirInicioList(list, 3);             printf("Inserindo no começo: %d\n", 3);
	inserirFinalList(list, 14);             printf("Inserindo no final: %d\n", 14);
	inserirInicioList(list, 8);             printf("Inserindo no começo: %d\n", 8);
	removido = removeUltimoList(list, int); printf("Revovendo ultimo: %d\n", removido);
	inserirInicioList(list, 6);             printf("Inserindo no começo: %d\n", 6);
	inserirInicioList(list, 11);            printf("Inserindo no começo: %d\n", 11);
	removido = removeInicioList(list, int); printf("Removendo primeiro: %d\n", removido);
	removido = removeUltimoList(list, int); printf("Revovendo ultimo: %d\n", removido);
	inserirInicioList(list, 9);             printf("Inserindo no começo: %d\n", 9);
	removido = removeUltimoList(list, int); printf("Revovendo ultimo: %d\n", removido);
	inserirFinalList(list, 1);              printf("Inserindo no final: %d\n", 1);
	removido = removeUltimoList(list, int); printf("Revovendo ultimo: %d\n", removido);
	removido = removeUltimoList(list, int); printf("Revovendo ultimo: %d\n", removido);
	inserirInicioList(list, 13);            printf("Inserindo no começo: %d\n", 13);
	removido = removeInicioList(list, int); printf("Removendo primeiro: %d\n", removido);
	inserirFinalList(list, 5);              printf("Inserindo no final: %d\n", 5);
	inserirFinalList(list, 12);             printf("Inserindo no final: %d\n", 12);
	inserirFinalList(list, 2);              printf("Inserindo no final: %d\n", 2);
	removido = removeInicioList(list, int); printf("Removendo primeiro: %d\n", removido);
	removido = removeInicioList(list, int); printf("Removendo primeiro: %d\n", removido);
	inserirInicioList(list, 4);             printf("Inserindo no começo: %d\n", 4);
	inserirFinalList(list, 10);             printf("Inserindo no final: %d\n", 10);
	inserirFinalList(list, 7);              printf("Inserindo no final: %d\n", 7);
	removido = removeUltimoList(list, int); printf("Revovendo ultimo: %d\n", removido);
	list_node_t *node = getNode(list, -1);
	printf("node->elem: %d\n", cast(int, node->elem));
	node = getNode(list, -2);
	printf("node->elem: %d\n", cast(int, node->elem));
	printf("list->length: %d\n", list->length);

	removido = removeUltimoList(list, int); printf("Revovendo ultimo: %d\n", removido);
	removido = removeUltimoList(list, int); printf("Revovendo ultimo: %d\n", removido);
	removido = removeUltimoList(list, int); printf("Revovendo ultimo: %d\n", removido);
	removido = removeUltimoList(list, int); printf("Revovendo ultimo: %d\n", removido);

	freeList(list, NULL);
	exit(0);
}


#ifdef TEST
int main(int argc, char *argv[]) {
	srand(1);

	init(2, 3, 20, 5);

	return 0;
}
#endif // TEST