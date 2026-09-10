#include "binary_heap.h"

#include <stdlib.h>

#define CAPACIDADE_INICIAL 8

struct heap {
    int *dados;
    size_t tamanho;
    size_t capacidade;
};

static void troca(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/* Sobe o elemento em `i` enquanto for menor que o pai. */
static void subir(int *v, size_t i) {
    while (i > 0) {
        size_t pai = (i - 1) / 2;
        if (v[pai] <= v[i])
            break;
        troca(&v[pai], &v[i]);
        i = pai;
    }
}

/* Desce o elemento em `i` enquanto for maior que algum filho. */
static void descer(int *v, size_t n, size_t i) {
    for (;;) {
        size_t menor = i;
        size_t esq = 2 * i + 1;
        size_t dir = 2 * i + 2;

        if (esq < n && v[esq] < v[menor])
            menor = esq;
        if (dir < n && v[dir] < v[menor])
            menor = dir;

        if (menor == i)
            return;
        troca(&v[i], &v[menor]);
        i = menor;
    }
}

static Heap *alocar(size_t capacidade) {
    Heap *h = malloc(sizeof *h);
    if (h == NULL)
        return NULL;

    h->dados = malloc(capacidade * sizeof *h->dados);
    if (h->dados == NULL) {
        free(h);
        return NULL;
    }
    h->tamanho = 0;
    h->capacidade = capacidade;
    return h;
}

Heap *heap_criar(void) {
    return alocar(CAPACIDADE_INICIAL);
}

Heap *heap_criar_de(const int *v, size_t n) {
    size_t cap = CAPACIDADE_INICIAL;
    while (cap < n)
        cap *= 2;

    Heap *h = alocar(cap);
    if (h == NULL)
        return NULL;

    for (size_t i = 0; i < n; i++)
        h->dados[i] = v[i];
    h->tamanho = n;

    /* heapify de baixo para cima: O(n), não O(n log n) */
    for (size_t i = n / 2; i-- > 0; )
        descer(h->dados, h->tamanho, i);

    return h;
}

void heap_destruir(Heap *h) {
    if (h == NULL)
        return;
    free(h->dados);
    free(h);
}

size_t heap_tamanho(const Heap *h) {
    return h->tamanho;
}

int heap_vazio(const Heap *h) {
    return h->tamanho == 0;
}

int heap_inserir(Heap *h, int valor) {
    if (h->tamanho == h->capacidade) {
        size_t nova = h->capacidade * 2;
        int *maior = realloc(h->dados, nova * sizeof *maior);
        if (maior == NULL)
            return -1;
        h->dados = maior;
        h->capacidade = nova;
    }

    h->dados[h->tamanho] = valor;
    subir(h->dados, h->tamanho);
    h->tamanho++;
    return 0;
}

int heap_remover_min(Heap *h, int *saida) {
    if (h->tamanho == 0)
        return -1;

    if (saida != NULL)
        *saida = h->dados[0];

    h->tamanho--;
    h->dados[0] = h->dados[h->tamanho];   /* traz o último para a raiz */
    descer(h->dados, h->tamanho, 0);
    return 0;
}

int heap_min(const Heap *h, int *saida) {
    if (h->tamanho == 0)
        return -1;
    if (saida != NULL)
        *saida = h->dados[0];
    return 0;
}
