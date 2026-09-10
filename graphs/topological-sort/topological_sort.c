#include "topological_sort.h"

#include <stdlib.h>

typedef struct aresta {
    int destino;
    struct aresta *prox;
} Aresta;

struct grafo {
    int n;
    Aresta **adj;
};

Grafo *grafo_criar(int n) {
    if (n <= 0)
        return NULL;
    Grafo *g = malloc(sizeof *g);
    if (g == NULL)
        return NULL;
    g->adj = calloc((size_t)n, sizeof *g->adj);
    if (g->adj == NULL) {
        free(g);
        return NULL;
    }
    g->n = n;
    return g;
}

void grafo_destruir(Grafo *g) {
    if (g == NULL)
        return;
    for (int u = 0; u < g->n; u++) {
        Aresta *a = g->adj[u];
        while (a != NULL) {
            Aresta *s = a->prox;
            free(a);
            a = s;
        }
    }
    free(g->adj);
    free(g);
}

int grafo_num_vertices(const Grafo *g) {
    return g->n;
}

int grafo_adicionar_aresta(Grafo *g, int u, int v) {
    if (u < 0 || v < 0 || u >= g->n || v >= g->n || u == v)
        return -1;
    Aresta **p = &g->adj[u];
    while (*p != NULL && (*p)->destino < v)
        p = &(*p)->prox;
    if (*p != NULL && (*p)->destino == v)
        return 1;
    Aresta *nova = malloc(sizeof *nova);
    if (nova == NULL)
        return -1;
    nova->destino = v;
    nova->prox = *p;
    *p = nova;
    return 0;
}

int ordenacao_topologica(const Grafo *g, int *ordem) {
    int n = g->n;
    int *grau_entrada = calloc((size_t)n, sizeof *grau_entrada);
    int *fila = malloc((size_t)n * sizeof *fila);
    if (grau_entrada == NULL || fila == NULL) {
        free(grau_entrada);
        free(fila);
        return -1;
    }

    for (int u = 0; u < n; u++)
        for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox)
            grau_entrada[a->destino]++;

    int ini = 0, fim = 0;
    for (int u = 0; u < n; u++)
        if (grau_entrada[u] == 0)
            fila[fim++] = u;

    int postos = 0;
    while (ini < fim) {
        int u = fila[ini++];
        ordem[postos++] = u;
        for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox)
            if (--grau_entrada[a->destino] == 0)
                fila[fim++] = a->destino;
    }

    free(grau_entrada);
    free(fila);
    return postos == n ? 0 : -1;   /* sobrou vértice => ciclo */
}

/* DFS que empilha cada vértice ao finalizar; a ordem topológica é o reverso.
   Devolve 0 ok, -1 se achar um ciclo. */
static int visita(const Grafo *g, int u, int *cor, int *ordem, int *pos) {
    cor[u] = 1;
    for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
        if (cor[a->destino] == 1)
            return -1;
        if (cor[a->destino] == 0 && visita(g, a->destino, cor, ordem, pos) != 0)
            return -1;
    }
    cor[u] = 2;
    ordem[--(*pos)] = u;
    return 0;
}

int ordenacao_topologica_dfs(const Grafo *g, int *ordem) {
    int n = g->n;
    int *cor = calloc((size_t)n, sizeof *cor);
    if (cor == NULL)
        return -1;

    int pos = n;
    int ok = 0;
    for (int u = 0; u < n && ok == 0; u++)
        if (cor[u] == 0)
            ok = visita(g, u, cor, ordem, &pos);

    free(cor);
    return ok;
}
