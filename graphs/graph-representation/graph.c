#include "graph.h"

#include <stdlib.h>

typedef struct aresta {
    int destino;
    struct aresta *prox;
} Aresta;

struct grafo {
    int n;
    int dirigido;
    int num_arestas;
    Aresta **adj;      /* adj[u] = lista de vizinhos de u, ordenada por índice */
    int *grau_entrada;
};

Grafo *grafo_criar(int n, int dirigido) {
    if (n <= 0)
        return NULL;

    Grafo *g = malloc(sizeof *g);
    if (g == NULL)
        return NULL;

    g->adj = calloc((size_t)n, sizeof *g->adj);
    g->grau_entrada = calloc((size_t)n, sizeof *g->grau_entrada);
    if (g->adj == NULL || g->grau_entrada == NULL) {
        free(g->adj);
        free(g->grau_entrada);
        free(g);
        return NULL;
    }
    g->n = n;
    g->dirigido = dirigido != 0;
    g->num_arestas = 0;
    return g;
}

void grafo_destruir(Grafo *g) {
    if (g == NULL)
        return;
    for (int u = 0; u < g->n; u++) {
        Aresta *a = g->adj[u];
        while (a != NULL) {
            Aresta *seguinte = a->prox;
            free(a);
            a = seguinte;
        }
    }
    free(g->adj);
    free(g->grau_entrada);
    free(g);
}

int grafo_num_vertices(const Grafo *g) {
    return g->n;
}

int grafo_num_arestas(const Grafo *g) {
    return g->num_arestas;
}

int grafo_dirigido(const Grafo *g) {
    return g->dirigido;
}

/* Insere `v` na lista ordenada de `u`. 0 se inseriu, 1 se já existia, -1 alloc. */
static int inserir_ordenado(Aresta **lista, int v) {
    Aresta *ant = NULL;
    Aresta *at = *lista;
    while (at != NULL && at->destino < v) {
        ant = at;
        at = at->prox;
    }
    if (at != NULL && at->destino == v)
        return 1;

    Aresta *nova = malloc(sizeof *nova);
    if (nova == NULL)
        return -1;
    nova->destino = v;
    nova->prox = at;
    if (ant == NULL)
        *lista = nova;
    else
        ant->prox = nova;
    return 0;
}

static int remover_da_lista(Aresta **lista, int v) {
    Aresta *ant = NULL;
    Aresta *at = *lista;
    while (at != NULL && at->destino != v) {
        ant = at;
        at = at->prox;
    }
    if (at == NULL)
        return -1;
    if (ant == NULL)
        *lista = at->prox;
    else
        ant->prox = at->prox;
    free(at);
    return 0;
}

int grafo_adicionar_aresta(Grafo *g, int u, int v) {
    if (u < 0 || v < 0 || u >= g->n || v >= g->n || u == v)
        return -1;

    int r = inserir_ordenado(&g->adj[u], v);
    if (r != 0)
        return r;                 /* 1 (duplicada) ou -1 (alloc) */

    if (g->dirigido) {
        g->grau_entrada[v]++;
    } else {
        /* espelha; se falhar, desfaz a primeira inserção */
        int r2 = inserir_ordenado(&g->adj[v], u);
        if (r2 == -1) {
            remover_da_lista(&g->adj[u], v);
            return -1;
        }
        g->grau_entrada[u]++;
        g->grau_entrada[v]++;
    }
    g->num_arestas++;
    return 0;
}

int grafo_remover_aresta(Grafo *g, int u, int v) {
    if (u < 0 || v < 0 || u >= g->n || v >= g->n)
        return -1;

    if (remover_da_lista(&g->adj[u], v) != 0)
        return -1;

    if (g->dirigido) {
        g->grau_entrada[v]--;
    } else {
        remover_da_lista(&g->adj[v], u);
        g->grau_entrada[u]--;
        g->grau_entrada[v]--;
    }
    g->num_arestas--;
    return 0;
}

int grafo_tem_aresta(const Grafo *g, int u, int v) {
    if (u < 0 || v < 0 || u >= g->n || v >= g->n)
        return 0;
    for (const Aresta *a = g->adj[u]; a != NULL && a->destino <= v; a = a->prox)
        if (a->destino == v)
            return 1;
    return 0;
}

int grafo_grau_saida(const Grafo *g, int u) {
    if (u < 0 || u >= g->n)
        return -1;
    int c = 0;
    for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox)
        c++;
    return c;
}

int grafo_grau_entrada(const Grafo *g, int u) {
    if (u < 0 || u >= g->n)
        return -1;
    if (!g->dirigido)
        return grafo_grau_saida(g, u);
    return g->grau_entrada[u];
}

void grafo_para_cada_vizinho(const Grafo *g, int u,
                             void (*visita)(int, void *), void *ctx) {
    if (u < 0 || u >= g->n)
        return;
    for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox)
        visita(a->destino, ctx);
}
