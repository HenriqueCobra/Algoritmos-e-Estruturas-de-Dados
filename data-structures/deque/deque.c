#include "deque.h"

#include <stdlib.h>

#define CAPACIDADE_INICIAL 8

struct deque {
    int *dados;
    size_t capacidade;
    size_t inicio;    /* índice do elemento da frente */
    size_t tamanho;
};

Deque *deque_criar(void) {
    Deque *d = malloc(sizeof *d);
    if (d == NULL)
        return NULL;

    d->dados = malloc(CAPACIDADE_INICIAL * sizeof *d->dados);
    if (d->dados == NULL) {
        free(d);
        return NULL;
    }
    d->capacidade = CAPACIDADE_INICIAL;
    d->inicio = 0;
    d->tamanho = 0;
    return d;
}

void deque_destruir(Deque *d) {
    if (d == NULL)
        return;
    free(d->dados);
    free(d);
}

size_t deque_tamanho(const Deque *d) {
    return d->tamanho;
}

int deque_vazio(const Deque *d) {
    return d->tamanho == 0;
}

/* Copia os elementos para um vetor maior, deixando a frente no índice 0. */
static int crescer(Deque *d) {
    size_t nova = d->capacidade * 2;
    int *maior = malloc(nova * sizeof *maior);
    if (maior == NULL)
        return -1;

    for (size_t i = 0; i < d->tamanho; i++)
        maior[i] = d->dados[(d->inicio + i) % d->capacidade];

    free(d->dados);
    d->dados = maior;
    d->capacidade = nova;
    d->inicio = 0;
    return 0;
}

int deque_inserir_inicio(Deque *d, int valor) {
    if (d->tamanho == d->capacidade && crescer(d) != 0)
        return -1;

    /* recua a frente uma posição, dando a volta no vetor */
    d->inicio = (d->inicio + d->capacidade - 1) % d->capacidade;
    d->dados[d->inicio] = valor;
    d->tamanho++;
    return 0;
}

int deque_inserir_fim(Deque *d, int valor) {
    if (d->tamanho == d->capacidade && crescer(d) != 0)
        return -1;

    size_t pos = (d->inicio + d->tamanho) % d->capacidade;
    d->dados[pos] = valor;
    d->tamanho++;
    return 0;
}

int deque_remover_inicio(Deque *d, int *saida) {
    if (d->tamanho == 0)
        return -1;

    if (saida != NULL)
        *saida = d->dados[d->inicio];
    d->inicio = (d->inicio + 1) % d->capacidade;
    d->tamanho--;
    return 0;
}

int deque_remover_fim(Deque *d, int *saida) {
    if (d->tamanho == 0)
        return -1;

    size_t pos = (d->inicio + d->tamanho - 1) % d->capacidade;
    if (saida != NULL)
        *saida = d->dados[pos];
    d->tamanho--;
    return 0;
}

int deque_frente(const Deque *d, int *saida) {
    if (d->tamanho == 0)
        return -1;
    if (saida != NULL)
        *saida = d->dados[d->inicio];
    return 0;
}

int deque_tras(const Deque *d, int *saida) {
    if (d->tamanho == 0)
        return -1;
    if (saida != NULL)
        *saida = d->dados[(d->inicio + d->tamanho - 1) % d->capacidade];
    return 0;
}
