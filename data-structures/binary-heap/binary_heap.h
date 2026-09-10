#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <stddef.h>

/* Min-heap binário sobre vetor dinâmico: o menor elemento fica sempre na raiz.
   Serve de fila de prioridade — é a estrutura por trás do Dijkstra e do Prim. */
typedef struct heap Heap;

Heap *heap_criar(void);
/* Constrói um heap já contendo os `n` valores de `v`, em O(n). NULL se falhar. */
Heap *heap_criar_de(const int *v, size_t n);
void  heap_destruir(Heap *h);

size_t heap_tamanho(const Heap *h);
int    heap_vazio(const Heap *h);

/* Insere `valor`. 0 em sucesso, -1 em falha de alocação. */
int heap_inserir(Heap *h, int valor);
/* Remove e devolve o menor elemento em `saida` (se não NULL). 0 ok, -1 se vazio. */
int heap_remover_min(Heap *h, int *saida);
/* Lê o menor sem remover. 0 ok, -1 se vazio. */
int heap_min(const Heap *h, int *saida);

#endif
