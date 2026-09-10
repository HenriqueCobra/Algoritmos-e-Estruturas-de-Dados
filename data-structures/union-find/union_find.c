#include "union_find.h"

#include <stdlib.h>

struct dsu {
    size_t *pai;        /* pai[i] = pai de i na floresta; raiz aponta para si */
    size_t *tam;        /* tam[i] só é válido quando i é raiz */
    size_t n;
    size_t num_conjuntos;
};

DSU *dsu_criar(size_t n) {
    DSU *d = malloc(sizeof *d);
    if (d == NULL)
        return NULL;

    d->pai = malloc(n * sizeof *d->pai);
    d->tam = malloc(n * sizeof *d->tam);
    if ((n > 0 && (d->pai == NULL || d->tam == NULL))) {
        free(d->pai);
        free(d->tam);
        free(d);
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        d->pai[i] = i;
        d->tam[i] = 1;
    }
    d->n = n;
    d->num_conjuntos = n;
    return d;
}

void dsu_destruir(DSU *d) {
    if (d == NULL)
        return;
    free(d->pai);
    free(d->tam);
    free(d);
}

size_t dsu_encontrar(DSU *d, size_t x) {
    /* compressão de caminho: aponta todo nó do caminho direto para a raiz */
    while (d->pai[x] != x) {
        d->pai[x] = d->pai[d->pai[x]];   /* encurta pela metade */
        x = d->pai[x];
    }
    return x;
}

int dsu_unir(DSU *d, size_t a, size_t b) {
    size_t ra = dsu_encontrar(d, a);
    size_t rb = dsu_encontrar(d, b);
    if (ra == rb)
        return 0;

    /* pendura a árvore menor na maior */
    if (d->tam[ra] < d->tam[rb]) {
        size_t tmp = ra;
        ra = rb;
        rb = tmp;
    }
    d->pai[rb] = ra;
    d->tam[ra] += d->tam[rb];
    d->num_conjuntos--;
    return 1;
}

int dsu_conectados(DSU *d, size_t a, size_t b) {
    return dsu_encontrar(d, a) == dsu_encontrar(d, b);
}

size_t dsu_num_conjuntos(const DSU *d) {
    return d->num_conjuntos;
}

size_t dsu_tamanho_conjunto(DSU *d, size_t x) {
    return d->tam[dsu_encontrar(d, x)];
}
