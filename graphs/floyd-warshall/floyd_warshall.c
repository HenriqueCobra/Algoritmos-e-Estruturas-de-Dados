#include "floyd_warshall.h"

#include <stdlib.h>

struct grafo {
    int n;
    long *w;      /* w[i*n + j] = peso da aresta i->j, ou FW_INFINITO */
};

Grafo *grafo_criar(int n) {
    if (n <= 0)
        return NULL;
    Grafo *g = malloc(sizeof *g);
    if (g == NULL)
        return NULL;
    g->w = malloc((size_t)n * (size_t)n * sizeof *g->w);
    if (g->w == NULL) {
        free(g);
        return NULL;
    }
    g->n = n;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            g->w[i * n + j] = (i == j) ? 0 : FW_INFINITO;
    return g;
}

void grafo_destruir(Grafo *g) {
    if (g == NULL)
        return;
    free(g->w);
    free(g);
}

int grafo_num_vertices(const Grafo *g) {
    return g->n;
}

int grafo_adicionar_aresta(Grafo *g, int u, int v, int peso) {
    if (u < 0 || v < 0 || u >= g->n || v >= g->n)
        return -1;
    long *slot = &g->w[u * g->n + v];
    if (*slot == FW_INFINITO || peso < *slot)
        *slot = peso;
    return 0;
}

int floyd_warshall(const Grafo *g, long *dist, int *prox) {
    int n = g->n;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist[i * n + j] = g->w[i * n + j];
            if (prox != NULL)
                prox[i * n + j] =
                    (g->w[i * n + j] != FW_INFINITO && i != j) ? j : -1;
        }
        if (prox != NULL)
            prox[i * n + i] = i;
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (dist[i * n + k] == FW_INFINITO)
                continue;
            for (int j = 0; j < n; j++) {
                if (dist[k * n + j] == FW_INFINITO)
                    continue;
                long via_k = dist[i * n + k] + dist[k * n + j];
                if (via_k < dist[i * n + j]) {
                    dist[i * n + j] = via_k;
                    if (prox != NULL)
                        prox[i * n + j] = prox[i * n + k];
                }
            }
        }
    }

    for (int i = 0; i < n; i++)
        if (dist[i * n + i] < 0)
            return 1;            /* ciclo negativo passando por i */
    return 0;
}

int fw_caminho(int n, const int *prox, int i, int j, int *caminho) {
    if (i < 0 || j < 0 || i >= n || j >= n || prox[i * n + j] == -1)
        return -1;

    int tam = 0;
    int atual = i;
    caminho[tam++] = atual;
    while (atual != j) {
        atual = prox[atual * n + j];
        if (atual == -1 || tam >= n)
            return -1;
        caminho[tam++] = atual;
    }
    return tam;
}
