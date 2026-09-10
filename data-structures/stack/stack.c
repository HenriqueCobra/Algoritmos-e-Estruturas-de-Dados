#include "stack.h"

#include <stdlib.h>

#define CAPACIDADE_INICIAL 8

struct pilha {
    int *dados;
    size_t tamanho;
    size_t capacidade;
};

Pilha *pilha_criar(void) {
    Pilha *p = malloc(sizeof *p);
    if (p == NULL)
        return NULL;

    p->dados = malloc(CAPACIDADE_INICIAL * sizeof *p->dados);
    if (p->dados == NULL) {
        free(p);
        return NULL;
    }
    p->tamanho = 0;
    p->capacidade = CAPACIDADE_INICIAL;
    return p;
}

void pilha_destruir(Pilha *p) {
    if (p == NULL)
        return;
    free(p->dados);
    free(p);
}

size_t pilha_tamanho(const Pilha *p) {
    return p->tamanho;
}

int pilha_vazia(const Pilha *p) {
    return p->tamanho == 0;
}

int pilha_empilhar(Pilha *p, int valor) {
    if (p->tamanho == p->capacidade) {
        size_t nova = p->capacidade * 2;
        int *maior = realloc(p->dados, nova * sizeof *maior);
        if (maior == NULL)
            return -1;
        p->dados = maior;
        p->capacidade = nova;
    }
    p->dados[p->tamanho++] = valor;
    return 0;
}

int pilha_desempilhar(Pilha *p, int *saida) {
    if (p->tamanho == 0)
        return -1;
    p->tamanho--;
    if (saida != NULL)
        *saida = p->dados[p->tamanho];
    return 0;
}

int pilha_topo(const Pilha *p, int *saida) {
    if (p->tamanho == 0)
        return -1;
    if (saida != NULL)
        *saida = p->dados[p->tamanho - 1];
    return 0;
}
