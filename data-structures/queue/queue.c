#include "queue.h"

#include <stdlib.h>

typedef struct no {
    int valor;
    struct no *prox;
} No;

struct fila {
    No *frente;
    No *fim;
    size_t tamanho;
};

Fila *fila_criar(void) {
    Fila *f = malloc(sizeof *f);
    if (f != NULL) {
        f->frente = NULL;
        f->fim = NULL;
        f->tamanho = 0;
    }
    return f;
}

void fila_destruir(Fila *f) {
    if (f == NULL)
        return;

    No *atual = f->frente;
    while (atual != NULL) {
        No *seguinte = atual->prox;
        free(atual);
        atual = seguinte;
    }
    free(f);
}

size_t fila_tamanho(const Fila *f) {
    return f->tamanho;
}

int fila_vazia(const Fila *f) {
    return f->tamanho == 0;
}

int fila_enfileirar(Fila *f, int valor) {
    No *n = malloc(sizeof *n);
    if (n == NULL)
        return -1;
    n->valor = valor;
    n->prox = NULL;

    if (f->fim != NULL)
        f->fim->prox = n;
    else
        f->frente = n;
    f->fim = n;
    f->tamanho++;
    return 0;
}

int fila_desenfileirar(Fila *f, int *saida) {
    if (f->frente == NULL)
        return -1;

    No *removido = f->frente;
    if (saida != NULL)
        *saida = removido->valor;

    f->frente = removido->prox;
    if (f->frente == NULL)
        f->fim = NULL;
    free(removido);
    f->tamanho--;
    return 0;
}

int fila_frente(const Fila *f, int *saida) {
    if (f->frente == NULL)
        return -1;
    if (saida != NULL)
        *saida = f->frente->valor;
    return 0;
}
