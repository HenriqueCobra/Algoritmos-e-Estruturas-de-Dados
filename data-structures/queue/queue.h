#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

/* Fila (FIFO) de inteiros implementada como lista encadeada com ponteiros para
   a frente e para o fim. Enfileira no fim, desenfileira na frente. */
typedef struct fila Fila;

Fila *fila_criar(void);
void  fila_destruir(Fila *f);

size_t fila_tamanho(const Fila *f);
int    fila_vazia(const Fila *f);

/* Insere no fim. 0 em sucesso, -1 em falha de alocação. */
int fila_enfileirar(Fila *f, int valor);
/* Remove da frente para `saida` (se não NULL). 0 em sucesso, -1 se vazia. */
int fila_desenfileirar(Fila *f, int *saida);
/* Lê a frente sem remover. 0 em sucesso, -1 se vazia. */
int fila_frente(const Fila *f, int *saida);

#endif
