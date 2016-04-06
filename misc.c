// define para dizer que este é o arquivo principal
#define __MISC_C__
#include "misc.h"

// 30.03.2016

// getchar que não deixa enter no buffer
char _getchar() {
	char str[5];
	fgets(str, 5, stdin);
	return *str;
}

// apenas imitando o pause de system
void system_pause() {
	puts("Pressione ENTER para continuar. . .");
	_getchar();
}

// scanf que retorna uma string
void _scanf_s(char *str, int size) {
	int i = 0;
	fgets(str, size, stdin);
	while(!iscntrl(str[i])) {
		i++;
	}
	str[i] = '\0';
}

// fgets que corta o \n no final
char *_fgets(char *str, int size, FILE *stream) {
	int i = 0;
	char *result = fgets(str, size, stream);
	while(!iscntrl(str[i])) {
		i++;
	}
	str[i] = '\0';
	return result;
}

// verifica se uma string é um número inteiro
bool is_int(char *str) {
	if(str[0] == '\0') {
		// se a string está vazia
		return false;
	}
	while(*str) {
		// para cada caracter de str
		char ch = *str;

		if(!isdigit(ch)) {
			// se ch não for um dígito, não é um número inteiro
			return false;
		}

		str++;
	}
	// todos são números
	return true;
}


// verifica se uma string é um número inteiro
bool is_float(char *str) {
	if(str[0] == '\0') {
		// se a string está vazia
		return false;
	}

	if(strcmp(str, ".") == 0) {
		// se é só um ponto
		return false;
	}

	bool jaUseiPonto = false;
	while(*str) {
		// para cada caracter de str
		char ch = *str;

		if(!isdigit(ch)) {
			// se ch não for um dígito, não é um número inteiro
			
			// devo testar se ele é o ponto
			if(ch == '.') {
				// se eu já usei o ponto, não é um float
				if(jaUseiPonto) {
					return false;
				}

				// nao usei o ponto ainda
				jaUseiPonto = true;
				str++;
				continue;
			}

			// não é um digito e não é o ponto
			return false;
		}

		str++;
	}

	// todos são números
	return true;
}

// scanf que retorna um int
int _scanf_int() {
	char str[11];
	fgets(str, 11, stdin);
	return atoi(str);
}

// scanf que retorna um double
double _scanf_double() {
	char str[11];
	fgets(str, 11, stdin);
	return atof(str);
}

// funcao malloc que fecha o programa assim que não há mais memória livre
void *_malloc(size_t size) {
	void *result = malloc(size);
	if(result == NULL) {
		fprintf(stderr, "_malloc: no memory available.\n");
		exit(1);
	}
	#ifdef CHECK_LEAK
		printf("MALLOC: %p => ", result);
	#endif // CHECK_LEAK
	return result;
}

// funcao calloc que fecha o programa assim que não há mais memória livre
void *_calloc(size_t count, size_t size) {
	void *result = calloc(count, size);
	if(result == NULL) {
		fprintf(stderr, "_calloc: no memory available.\n");
		exit(1);
	}
	#ifdef CHECK_LEAK
		printf("CALLOC: %p => ", result);
	#endif // CHECK_LEAK
	return result;
}

// funcao realloc que fecha o programa assim que não há mais memória livre
void *_realloc(void *ptr, size_t new_size) {
	void *result = realloc(ptr, new_size);
	if(result == NULL) {
		fprintf(stderr, "_realloc: no memory available.\n");
		exit(1);
	}
	#ifdef CHECK_LEAK
		if(ptr == NULL) {
			printf("REALLOC: %p =>", result);	
		} else if(ptr != result) {
			printf("REALLOCATION: %p => %p =>", ptr, result);
		}
	#endif // CHECK_LEAK
	return result;
}

// mesmo que free, mas não dá free em NULL
void _free(void *ptr) {
	#ifdef CHECK_LEAK
		printf("FREE: %p => ", ptr);
	#endif // CHECK_LEAK
	free(ptr);
}

// mesmo que pthread_create, mas testa o status
int _pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg) {
	int result = pthread_create(thread, attr, start_routine, arg);
	if(result != 0) {
		fprintf(stderr, "_pthread_create: returned %d.\n", result);
		exit(1);
	}
	return result;
}

// funcao fopen que fecha o programa se fopen retornar NULL
FILE *_fopen(char *filename, char *mode) {
	FILE *result = fopen(filename, mode);
	if(result == NULL) {
		fprintf(stderr, "_fopen: error opening file '%s'\n", filename);
		exit(1);
	}
	return result;
}

// funcao fclose que testa se o ponteiro é nulo
int _fclose(FILE *stream) {
	if(stream == NULL) {
		return 0;
	}
	return fclose(stream);
}

// funcao que retorna o tamanho de um arquivo
long int _file_size(FILE *file) {
	if(file == NULL) {
		return 0;
	}
	fseek(file, 0, SEEK_END);
	long int result = ftell(file);
	rewind(file);
	return result;
}

#ifndef WIN32
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
// funcao que retorna true ou false se o arquivo existe, retorna um dos três defines
char file_exists(char const *path_file) {
	struct stat status;
	if(stat(path_file, &status) != 0) {
		return FILE_NOT_FOUND;
	}
	if(S_ISDIR(status.st_mode)) {
		return IS_DIR;
	}
	return IS_FILE;
}
#endif

// remove um diretorio não vazio por completo
#ifndef WIN32
int remove_directory(const char *path) {
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;
   if(d) {
      struct dirent *p;
      r = 0;
      while(!r && (p=readdir(d))) {
          int r2 = -1;
          char *buf;
          size_t len;
          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if(!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
             continue;
          }
          len = path_len + strlen(p->d_name) + 2; 
          buf = (char *) _malloc(len);
          if(buf) {
             struct stat statbuf;
             snprintf(buf, len, "%s/%s", path, p->d_name);
             if (!stat(buf, &statbuf)) {
                if (S_ISDIR(statbuf.st_mode)) {
                   r2 = remove_directory(buf);
                } else {
                   r2 = unlink(buf);
                }
             }
             free(buf);
          }
          r = r2;
      }
      closedir(d);
   }
   if(!r) {
      r = rmdir(path);
   }
   return r;
}
#endif


#ifndef WIN32
// obtem comandos do teclado sem armazenar buffer
void setBufferedInput(bool enable) {
	static bool enabled = true;
	static struct termios old;
	struct termios new_termios;

	if (enable && !enabled) {
		// restore the former settings
		tcsetattr(STDIN_FILENO,TCSANOW,&old);
		// set the new_termios state
		enabled = true;
	} else if (!enable && enabled) {
		// get the terminal settings for standard input
		tcgetattr(STDIN_FILENO,&new_termios);
		// we want to keep the old setting to restore them at the end
		old = new_termios;
		// disable canonical mode (buffered i/o) and local echo
		new_termios.c_lflag &=(~ICANON & ~ECHO);
		// set the new_termios settings immediately
		tcsetattr(STDIN_FILENO,TCSANOW,&new_termios);
		// set the new_termios state
		enabled = false;
	}
}
#endif

// apaga o conteudo da tela
void clearScreen() {
	#ifdef TEST
		printf("clearScreen();\n");
		return ;
	#endif // TEST
	#ifdef WIN32
		system("cls");
	#else
		printf("\033[2J");
		printf("\033[H");
	#endif
}

// esconde o cursor
void hide_cursor() {
	#ifdef TEST
		printf("hide_cursor();\n");
		return ;
	#endif // TEST
	printf("\033[?25l");
}

// mostra o cursor dps de escondido
void show_cursor() {
	#ifdef TEST
		printf("show_cursor();\n");
		return ;
	#endif // TEST
	printf("\033[?25h");
}

// define a posição do cursor iniciando pela posição 0,0
void setCursor(int lin, int col) {
	#ifdef TEST
		printf("setCursor(%d, %d);\n", lin, col);
		return ;
	#endif // TEST
	printf("\033[%d;%dH", lin+1, col+1);
}

// move o cursor de acordo com as linhas e colunas
void moveCursor(int lin, int col) {
	#ifdef TEST
		printf("moveCursor(%d, %d);\n", lin, col);
		return ;
	#endif // TEST
	if(lin < 0) {
		printf("\033[%dA", -lin); // Move up lin lines;	
	} else if(lin > 0) {
		printf("\033[%dB", lin); // Move down lin lines;
	}

	if(col < 0) {
		printf("\033[%dD", -col); // Move left col column;
	}
	else if(col > 0) {
		printf("\033[%dC", col); // Move right col column;
	}
}

// apaga a linha que o cursor está
void apagaLinha() {
	#ifdef TEST
		printf("apagaLinha();\n");
		return ;
	#endif // TEST
	printf("\033[2K");
}

#ifndef WIN32
// devolve em microssegundos o tempo passado de starthere = true e starthere = false
double timediff(bool starthere) {
	static struct timeval tempoInicial;
	if(starthere) {
		gettimeofday(&tempoInicial, NULL);
		return 0;
	}
	struct timeval tempoAtual;
	gettimeofday(&tempoAtual, NULL);
	// já possuo os dois tempos, o atual e o inicial
	
	double tempoPassado; // em microssegundos
	tempoPassado = (tempoAtual.tv_sec - tempoInicial.tv_sec) * 1000.0;      // sec to ms
	tempoPassado += (tempoAtual.tv_usec - tempoInicial.tv_usec) / 1000.0;   // us to ms
	return tempoPassado;
}
#endif

// funcao boa para debug, imprime strings verticalmente
int printfVerticaly(char *str) {
	printf("===================\n");
	printf("Imprimindo verticalmente: '%s'\n", str);
	int i = 0;
	while(str[i]) {
		printf("char %2d: '%c', %d\n", i, str[i], str[i]);
		i++;
	}
	printf("==================\n");
	return strlen(str);
}

// cria uma seção para facilitar o debug da saida
void section(char *str, ...) {
	printf("/* =============================\n");
	va_list args;
	va_start(args, str);
	printf("   ");
	vprintf(str, args);
	printf("\n");
  va_end(args);
	printf("   ============================= */\n");
}

// inicializa uma seed para gerar números randômicos
time_t rand_init() {
	time_t seed = time(NULL);
	srand(seed);
	return seed;
}

// gera um número aleatório entre 0 e 1
double Math_random() {
	return rand() / (double) RAND_MAX;
}

// gera um número aleatório x, min <= x <= max
int randMinMax(int min, int max) {
	return rand()%(max - min + 1)+min;
}

// gera um número aleatório double x, min <= x <= max
double randMinMaxD(double min, double max) {
	return ((max - min) * Math_random()) + min;
}


/******************************************************
 * Implementação de list_t
 ******************************************************/
void listInit(list_t *list) {
	// define os valores padrões da lista
	memset(list, 0, sizeof(list_t));
}


list_t *createList() {
	list_t *result = (list_t *) _malloc(sizeof(list_t));
	listInit(result);
	return result;
}


// diz se uma lista está vazia
bool emptyList(list_t *list) {
	return list->length == 0;
}

bool posValidaList(list_t *list, int pos) {
	if(pos < 0) {
		pos = list->length + pos;
	}

	return (0 <= pos) && (pos < list->length);
}


// insere o nó entre o prev e o prox
list_node_t *novoNo(list_t *list, list_node_t *prev, list_node_t *prox, list_elem_t elem) {
	// aloco o espaço
	list_node_t *result = (list_node_t *) _calloc(1, sizeof(list_node_t));
	// padronizei pelo calloc, deixei tudo como NULL
	result->elem = elem;

	// inserindo entre prev e prox
	bool isFirst = list->first == prox;
	bool isLast = list->last == prev;

	result->prev = prev;
	result->prox = prox;

	if(prev) {
		prev->prox = result;
	}
	if(prox) {
		prox->prev = result;
	}

	// insiro ele na lista
	if(isFirst) {
		list->first = result;
	}
	if(isLast) {
		list->last = result;
	}
	list->length++;
	return result;
}


/**
 * insere no final
 * essa função recebe o void *
 * a função sem o underline recebe qlqr parametro
 */
void _inserirFinalList(list_t *list, list_elem_t elem) {
	novoNo(list, list->last, NULL, elem);
}


// essa função recebe o void *, a função sem o underline recebe qlqr parametro
void _inserirInicioList(list_t *list, list_elem_t elem) {
	novoNo(list, NULL, list->first, elem);
}


// essa função recebe o void *, a função sem o underline recebe qlqr parametro
void _inserirList(list_t *list, list_elem_t elem, int pos) {
	list_node_t *prev = getNode(list, pos);
	list_node_t *prox = prev->prox;

	novoNo(list, prev, prox, elem);
}


list_node_t *getNode(list_t *list, int pos) {
	// se passei do limite da lista, devo sair para não dar segfault
	if(!posValidaList(list, pos)) {
		return NULL;
	}

	// se pos for negativo, copmeça a contagem de trás para frente
	// exemplo: uma lista com 9 elementos
	// pos = -1, pego o list[8]
	bool contagemReversa = false;
	if(pos < 0) {
		pos = list->length + pos;
		// vou fazer uma contagem reversa
		contagemReversa = true;
	}

	list_node_t *result;

	// caso da contagem reversa
	if(contagemReversa) {
		result = list->last;
		for(int i=list->length-1; i>pos; i--) {
			// supor uma lista de tamaho 9
			// meu i começa valendo 8
			// se meu pos é 8
			// nem entra nesse loop
			// se for 7, roda o loop uma vez
			result = result->prev;
		}
		return result;
	}


	// só obter o elemento
	result = list->first;
	for(int i=0; i<pos; i++) {
		// vo avançando até encontrar
		result = result->prox;
	}
	return result;
}


list_elem_t _getList(list_t *list, int pos) {
	// pra pegar o elemento, pego o nó e trago o elemento
	return getNode(list, pos)->elem;
}


/**
 * remove um nó da lista
 */
void removeNode(list_t *list, list_node_t *node) {
	if(node == NULL) {
		return ;
	}

	if(list->length == 0) {
		printf("não posso remover de uma lista vazia. exiting\n");
		exit(EXIT_FAILURE);
	}

	bool isFirst = list->first == node;
	bool isLast = list->last == node;
	list_node_t *prev = node->prev;
	list_node_t *prox = node->prox;

	// se tem anterior
	if(prev) {
		prev->prox = node->prox;
	}

	// se tem proximo
	if(prox) {
		prox->prev = node->prev;
	}


	if(isFirst) {
		// faz o segundo ser o novo primeiro
		list->first = prox;
	}

	if(isLast) {
		// faz o penultimo ser o novo ultimo
		list->last = prev;
	}


	list->length--;

	// libera esse nó
	freeNode(node);
}


list_elem_t *_removeUltimoList(list_t *list) {
	if(list->length == 0) {
		printf("não posso remover de uma lista vazia. exiting\n");
		exit(EXIT_FAILURE);
	}

	list_node_t *nodeUltimo = list->last;
	list_elem_t result = nodeUltimo->elem;
	
	removeNode(list, nodeUltimo);

	return result;
}


list_elem_t *_removeInicioList(list_t *list) {
	if(list->length == 0) {
		printf("não posso remover de uma lista vazia. exiting\n");
		exit(EXIT_FAILURE);
	}

	list_node_t *nodeFirst = list->first;
	list_elem_t result = nodeFirst->elem;

	removeNode(list, nodeFirst);

	return result;
}


list_elem_t *_removeList(list_t *list, int pos) {
	if(list->length == 0) {
		printf("não posso remover de uma lista vazia. exiting\n");
		exit(EXIT_FAILURE);
	}

	list_node_t *node = getNode(list, pos);
	list_elem_t result = node->elem;

	removeNode(list, node);

	return result;
}


void freeNode(list_node_t *node) {
	if(node == NULL) {
		// não faço nada
		return ;
	}

	_free(node);
}


// chame freeList(list, NULL) para realizar um free normal
void freeList(list_t *list) {
	void *elem;
	forList(list_elem_t, elem, list) {
		freeNode(node);
	}

	_free(list);
}