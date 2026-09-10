#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h>

/* Deque (double-ended queue): fila de duas pontas. Inserir e remover tanto no
   início quanto no fim custa O(1) amortizado. Implementado como buffer circular
   sobre um vetor que dobra de capacidade quando enche. */
typedef struct deque Deque;

Deque *deque_criar(void);
void   deque_destruir(Deque *d);

size_t deque_tamanho(const Deque *d);
int    deque_vazio(const Deque *d);

/* Inserções: 0 em sucesso, -1 em falha de alocação. */
int deque_inserir_inicio(Deque *d, int valor);
int deque_inserir_fim(Deque *d, int valor);

/* Remoções: copiam o valor para `saida` (se não NULL). 0 em sucesso, -1 se vazio. */
int deque_remover_inicio(Deque *d, int *saida);
int deque_remover_fim(Deque *d, int *saida);

/* Leem uma das pontas sem remover. 0 em sucesso, -1 se vazio. */
int deque_frente(const Deque *d, int *saida);
int deque_tras(const Deque *d, int *saida);

#endif
