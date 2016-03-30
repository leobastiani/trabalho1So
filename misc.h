#ifndef __MISC_H__
#define __MISC_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <pthread.h>

// estrutura para se usar quando precisa-se retornar um
// resultado com um status de erro ou de outro tipo
typedef struct resultado_t {
	int status;
	void *result;
} resultado_t;


// defina a variável abaixo para que fique fácil de descobrir onde existe Memory Leak
// #define CHECK_LEAK

char _getchar(); // getchar que não deixa enter no buffer
void system_pause(); // apenas imitando o pause de system
void _scanf_s(char *str, int size); // scanf que retorna uma string
char *_fgets(char *str, int size, FILE *stream); // fgets que corta o \n no final
bool is_int(char *str); // verifica se uma string é um número inteiro
bool is_float(char *str); // verifica se uma string é um número inteiro
int _scanf_int(); // scanf que retorna um int
double _scanf_double(); // scanf que retorna um double
void *_malloc(size_t size); // funcao malloc que fecha o programa assim que não há mais memória livre
void *_calloc(size_t count, size_t size); // funcao calloc que fecha o programa assim que não há mais memória livre
void *_realloc(void *ptr, size_t new_size); // funcao realloc que fecha o programa assim que não há mais memória livre
void _free(void *ptr); // mesmo que free, mas não dá free em NULL
int _pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg); // mesmo que pthread_create, mas testa o status
int _fclose(FILE *stream); // funcao fclose que testa se o ponteiro é nulo
FILE *_fopen(char *filename, char *mode); // funcao fopen que fecha o programa se fopen retornar NULL
long int _file_size(FILE *file); // funcao que retorna o tamanho de um arquivo

#define FILE_NOT_FOUND 0
#define IS_FILE 1
#define IS_DIR 2
char file_exists(char const *str); // funcao que retorna true ou false se o arquivo existe, retorna um dos três defines
int remove_directory(const char *path); // remove um diretorio não vazio por completo

void setBufferedInput(bool enable); // obtem comandos do teclado sem armazenar buffer
void clearScreen(); // apaga o conteudo da tela
void hide_cursor(); // esconde o cursor
void show_cursor(); // mostra o cursor dps de escondido
void setCursor(int lin, int col); // define a posição do cursor iniciando pela posição 0,0
void moveCursor(int lin, int col); // move o cursor de acordo com as linhas e colunas
void apagaLinha(); // apaga a linha que o cursor está
double timediff(bool starthere); // devolve em segundos o tempo passado de starthere = true e starthere = false
int printfVerticaly(char *str); // funcao boa para debug, imprime strings verticalmente
void section(char *str, ...); // cria uma seção para facilitar o debug da saida

void rand_init(); // inicializa uma seed para gerar números randômicos
double Math_random(); // gera um número aleatório entre 0 e 1
int randMinMax(int min, int max); // gera um número aleatório x, min <= x <= max

// troca as funções de alocação de memória, menos no arquivo .c
#ifndef __MISC_C__
	// aqui vão as informações no caso em que não estou editando o misc.c
	#define pthread_create _pthread_create

	#ifdef CHECK_LEAK
		#define malloc(x)     _malloc(x);     printf("ALLOC_LINE: %d, FILE: %s\n", __LINE__, __FILE__);
		#define realloc(x, y) _realloc(x, y); printf("ALLOC_LINE: %d, FILE: %s\n", __LINE__, __FILE__);
		#define calloc(x, y)  _calloc(x, y);  printf("ALLOC_LINE: %d, FILE: %s\n", __LINE__, __FILE__);
		#define free(x)       _free(x);       printf("FREE_LINE: %d, FILE: %s\n", __LINE__, __FILE__);
	#else
		#define malloc _malloc
		#define realloc _realloc
		#define calloc _calloc
		#define free _free
	#endif // CHECK_LEAK
#endif

// definindo a função debug que faz a msma coisa que o printf
#ifdef DEBUG
	#define debug(str, ...) printf(str, ##__VA_ARGS__)
	#define sectionDebug(str, ...) section(str, ##__VA_ARGS__)
#else
	#define debug(str, ...) ()
	#define sectionDebug(str, ...) ()
#endif // DEBUG

/**
 * Defines para facilitar trabalhar com cores
 */
// strings das cores
#define COR_FORE_RED     "\033[31m"
#define COR_FORE_GREEN   "\033[32m"
#define COR_FORE_YELLOW  "\033[33m"
#define COR_FORE_BLUE    "\033[34m"
#define COR_FORE_MAGENTA "\033[35m"
#define COR_FORE_CYAN    "\033[36m"
#define COR_BG_RED       "\033[41m"
#define COR_BG_GREEN     "\033[42m"
#define COR_BG_YELLOW    "\033[43m"
#define COR_BG_BLUE      "\033[44m"
#define COR_BG_MAGENTA   "\033[45m"
#define COR_BG_CYAN      "\033[46m"
#define NEGRITO          "\033[1m"
#define COR_BG_RESET     "\033[49m"
#define RESET            "\033[0m"

#ifndef TEST
	// definindo as funções para serem utilizadas
	#define negrito()       printf(NEGRITO)
	// cor do texto
	#define vermelho()      printf(COR_FORE_RED)
	#define verde()         printf(COR_FORE_GREEN)
	#define amarelo()       printf(COR_FORE_YELLOW)
	#define azul()          printf(COR_FORE_BLUE)
	#define magenta()       printf(COR_FORE_MAGENTA)
	#define ciano()         printf(COR_FORE_CYAN)
	// cor do fundo
	#define fundoVermelho() printf(COR_BG_RED)
	#define fundoVerde()    printf(COR_BG_GREEN)
	#define fundoAmarelo()  printf(COR_BG_YELLOW)
	#define fundoAzul()     printf(COR_BG_BLUE)
	#define fundoMagenta()  printf(COR_BG_MAGENTA)
	#define fundoCiano()    printf(COR_BG_CYAN)
  
  // resets
	#define fundoPadrao()   printf(COR_BG_RESET)
	#define corPadrao()     printf(RESET)
#else
	// caso em que estou depurando o programa, não preciso
	// imprimir com cores, pois as vezes estou lendo um arquivo
	// e não o terminal
	#define negrito()       ()
	// cor do texto
	#define vermelho()      ()
	#define verde()         ()
	#define amarelo()       ()
	#define azul()          ()
	#define magenta()       ()
	#define ciano()         ()
	// cor do fundo
	#define fundoVermelho() ()
	#define fundoVerde()    ()
	#define fundoAmarelo()  ()
	#define fundoAzul()     ()
	#define fundoMagenta()  ()
	#define fundoCiano()    ()
#endif // TEST

// resolve problemas como converter (void *) para (int)
#define cast(tipo, variavel) ((tipo) (intptr_t) variavel)
// dá um cast para o tipo void *
#define voidptr_t(variavel) (cast(void *, variavel))

#endif