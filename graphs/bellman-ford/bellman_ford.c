#include "bellman_ford.h"

#include <stdlib.h>

typedef struct {
    int u, v, peso;
} ArestaDir;

struct grafo {
    int n;
    ArestaDir *arestas;
    int m;
    int cap;
};

Grafo *grafo_criar(int n) {
    if (n <= 0)
        return NULL;
    Grafo *g = malloc(sizeof *g);
    if (g == NULL)
        return NULL;
    g->cap = 8;
    g->arestas = malloc((size_t)g->cap * sizeof *g->arestas);
    if (g->arestas == NULL) {
        free(g);
        return NULL;
    }
    g->n = n;
    g->m = 0;
    return g;
}

void grafo_destruir(Grafo *g) {
    if (g == NULL)
        return;
    free(g->arestas);
    free(g);
}

int grafo_num_vertices(const Grafo *g) {
    return g->n;
}

int grafo_adicionar_aresta(Grafo *g, int u, int v, int peso) {
    if (u < 0 || v < 0 || u >= g->n || v >= g->n)
        return -1;
    if (g->m == g->cap) {
        int nc = g->cap * 2;
        ArestaDir *maior = realloc(g->arestas, (size_t)nc * sizeof *maior);
        if (maior == NULL)
            return -1;
        g->arestas = maior;
        g->cap = nc;
    }
    g->arestas[g->m].u = u;
    g->arestas[g->m].v = v;
    g->arestas[g->m].peso = peso;
    g->m++;
    return 0;
}

int bellman_ford(const Grafo *g, int origem, long *dist, int *pai) {
    int n = g->n;
    for (int i = 0; i < n; i++) {
        dist[i] = BF_INFINITO;
        if (pai != NULL)
            pai[i] = -1;
    }
    if (origem < 0 || origem >= n)
        return 0;
    dist[origem] = 0;

    /* V-1 rodadas de relaxamento de todas as arestas */
    for (int rodada = 0; rodada < n - 1; rodada++) {
        int mudou = 0;
        for (int e = 0; e < g->m; e++) {
            int u = g->arestas[e].u, v = g->arestas[e].v;
            if (dist[u] == BF_INFINITO)
                continue;
            long nd = dist[u] + g->arestas[e].peso;
            if (nd < dist[v]) {
                dist[v] = nd;
                if (pai != NULL)
                    pai[v] = u;
                mudou = 1;
            }
        }
        if (!mudou)
            break;               /* estabilizou antes: não há ciclo negativo */
    }

    /* rodada extra: se ainda melhora, há ciclo negativo alcançável */
    for (int e = 0; e < g->m; e++) {
        int u = g->arestas[e].u, v = g->arestas[e].v;
        if (dist[u] != BF_INFINITO && dist[u] + g->arestas[e].peso < dist[v])
            return 1;
    }
    return 0;
}
