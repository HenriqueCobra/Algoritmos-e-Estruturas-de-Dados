#include "dijkstra.h"

#include <stdlib.h>

typedef struct aresta {
    int destino;
    int peso;
    struct aresta *prox;
} Aresta;

struct grafo {
    int n;
    int dirigido;
    Aresta **adj;
};

Grafo *grafo_criar(int n, int dirigido) {
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
    g->dirigido = dirigido != 0;
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

static int ligar(Grafo *g, int u, int v, int peso) {
    Aresta **p = &g->adj[u];
    while (*p != NULL && (*p)->destino < v)
        p = &(*p)->prox;
    if (*p != NULL && (*p)->destino == v)
        return 1;
    Aresta *nova = malloc(sizeof *nova);
    if (nova == NULL)
        return -1;
    nova->destino = v;
    nova->peso = peso;
    nova->prox = *p;
    *p = nova;
    return 0;
}

int grafo_adicionar_aresta(Grafo *g, int u, int v, int peso) {
    if (u < 0 || v < 0 || u >= g->n || v >= g->n || u == v || peso < 0)
        return -1;
    int r = ligar(g, u, v, peso);
    if (r != 0)
        return r;
    if (!g->dirigido && ligar(g, v, u, peso) == -1)
        return -1;
    return 0;
}

/* ---- heap binário de pares (distância, vértice) ---- */

typedef struct {
    long d;
    int v;
} Item;

typedef struct {
    Item *dados;
    int tam;
} Heap;

static void heap_sobe(Heap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[pai].d <= h->dados[i].d)
            break;
        Item t = h->dados[pai];
        h->dados[pai] = h->dados[i];
        h->dados[i] = t;
        i = pai;
    }
}

static void heap_desce(Heap *h, int i) {
    for (;;) {
        int menor = i, e = 2 * i + 1, d = 2 * i + 2;
        if (e < h->tam && h->dados[e].d < h->dados[menor].d)
            menor = e;
        if (d < h->tam && h->dados[d].d < h->dados[menor].d)
            menor = d;
        if (menor == i)
            return;
        Item t = h->dados[menor];
        h->dados[menor] = h->dados[i];
        h->dados[i] = t;
        i = menor;
    }
}

static void heap_insere(Heap *h, long d, int v) {
    h->dados[h->tam].d = d;
    h->dados[h->tam].v = v;
    h->tam++;
    heap_sobe(h, h->tam - 1);
}

static Item heap_remove_min(Heap *h) {
    Item topo = h->dados[0];
    h->dados[0] = h->dados[--h->tam];
    heap_desce(h, 0);
    return topo;
}

int dijkstra(const Grafo *g, int origem, long *dist, int *pai) {
    int n = g->n;
    if (n <= 0)
        return 0;
    for (int i = 0; i < n; i++) {
        dist[i] = -1;
        if (pai != NULL)
            pai[i] = -1;
    }
    if (origem < 0 || origem >= n)
        return 0;

    char *fechado = calloc((size_t)n, sizeof *fechado);
    /* o heap pode ter uma entrada por relaxamento: no máximo E + 1 */
    int cap = 1;
    for (int u = 0; u < n; u++)
        for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox)
            cap++;
    Item *buf = malloc((size_t)cap * sizeof *buf);
    if (fechado == NULL || buf == NULL) {
        free(fechado);
        free(buf);
        return -1;
    }
    Heap h = {buf, 0};

    dist[origem] = 0;
    heap_insere(&h, 0, origem);

    while (h.tam > 0) {
        Item it = heap_remove_min(&h);
        int u = it.v;
        if (fechado[u])
            continue;              /* entrada obsoleta (lazy deletion) */
        fechado[u] = 1;

        for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
            long nd = dist[u] + a->peso;
            if (dist[a->destino] == -1 || nd < dist[a->destino]) {
                dist[a->destino] = nd;
                if (pai != NULL)
                    pai[a->destino] = u;
                heap_insere(&h, nd, a->destino);
            }
        }
    }

    free(fechado);
    free(buf);
    return 0;
}

int dijkstra_caminho(const Grafo *g, int origem, int destino,
                     int *caminho, long *custo) {
    int n = g->n;
    if (origem < 0 || destino < 0 || origem >= n || destino >= n)
        return -1;

    long *dist = malloc((size_t)n * sizeof *dist);
    int *pai = malloc((size_t)n * sizeof *pai);
    if (dist == NULL || pai == NULL || dijkstra(g, origem, dist, pai) != 0) {
        free(dist);
        free(pai);
        return -1;
    }

    int tam = -1;
    if (dist[destino] != -1) {
        if (custo != NULL)
            *custo = dist[destino];
        tam = 0;
        for (int v = destino; v != -1; v = pai[v])
            tam++;
        for (int v = destino, i = tam - 1; v != -1; v = pai[v], i--)
            caminho[i] = v;
    }

    free(dist);
    free(pai);
    return tam;
}
