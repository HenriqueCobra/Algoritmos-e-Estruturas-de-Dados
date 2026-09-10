#include "mst.h"

#include <stdlib.h>

typedef struct aresta {
    int destino;
    int peso;
    struct aresta *prox;
} Aresta;

struct grafo {
    int n;
    int m;                 /* nº de arestas não dirigidas */
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
    g->m = 0;
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
    if (u < 0 || v < 0 || u >= g->n || v >= g->n || u == v)
        return -1;
    int r = ligar(g, u, v, peso);
    if (r != 0)
        return r;
    if (ligar(g, v, u, peso) == -1)
        return -1;
    g->m++;
    return 0;
}

/* ---- Prim (heap binário de pares (peso, vértice), remoção preguiçosa) ---- */

typedef struct { long chave; int v; int de; } Item;

static void troca_item(Item *a, Item *b) { Item t = *a; *a = *b; *b = t; }

static void sobe(Item *h, int i) {
    while (i > 0 && h[(i - 1) / 2].chave > h[i].chave) {
        troca_item(&h[(i - 1) / 2], &h[i]);
        i = (i - 1) / 2;
    }
}

static void desce(Item *h, int tam, int i) {
    for (;;) {
        int m = i, e = 2 * i + 1, d = 2 * i + 2;
        if (e < tam && h[e].chave < h[m].chave) m = e;
        if (d < tam && h[d].chave < h[m].chave) m = d;
        if (m == i) return;
        troca_item(&h[i], &h[m]);
        i = m;
    }
}

int mst_prim(const Grafo *g, int *mst_u, int *mst_v, int *num_arestas,
             long *peso_total) {
    int n = g->n;
    char *na_arvore = calloc((size_t)n, sizeof *na_arvore);
    Item *heap = malloc((size_t)(2 * g->m + 1) * sizeof *heap);
    if (na_arvore == NULL || heap == NULL) {
        free(na_arvore);
        free(heap);
        return -1;
    }

    int tam = 0;
    int postos = 0;
    long total = 0;

    for (int inicio = 0; inicio < n; inicio++) {
        if (na_arvore[inicio])
            continue;
        /* novo componente */
        na_arvore[inicio] = 1;
        for (const Aresta *a = g->adj[inicio]; a != NULL; a = a->prox) {
            heap[tam] = (Item){a->peso, a->destino, inicio};
            sobe(heap, tam++);
        }

        while (tam > 0) {
            Item it = heap[0];
            heap[0] = heap[--tam];
            desce(heap, tam, 0);
            if (na_arvore[it.v])
                continue;
            na_arvore[it.v] = 1;
            mst_u[postos] = it.de;
            mst_v[postos] = it.v;
            postos++;
            total += it.chave;
            for (const Aresta *a = g->adj[it.v]; a != NULL; a = a->prox) {
                if (!na_arvore[a->destino]) {
                    heap[tam] = (Item){a->peso, a->destino, it.v};
                    sobe(heap, tam++);
                }
            }
        }
    }

    free(na_arvore);
    free(heap);
    *num_arestas = postos;
    *peso_total = total;
    return postos == n - 1 ? 0 : 1;
}

/* ---- Kruskal (ordenação + Union-Find) ---- */

typedef struct { int u, v, peso; } ArestaK;

static int cmp_aresta(const void *a, const void *b) {
    int pa = ((const ArestaK *)a)->peso;
    int pb = ((const ArestaK *)b)->peso;
    return (pa > pb) - (pa < pb);
}

static int dsu_raiz(int *pai, int x) {
    while (pai[x] != x) {
        pai[x] = pai[pai[x]];
        x = pai[x];
    }
    return x;
}

int mst_kruskal(const Grafo *g, int *mst_u, int *mst_v, int *num_arestas,
                long *peso_total) {
    int n = g->n;
    ArestaK *arestas = malloc((size_t)(g->m > 0 ? g->m : 1) * sizeof *arestas);
    int *pai = malloc((size_t)n * sizeof *pai);
    int *rank_ = calloc((size_t)n, sizeof *rank_);
    if (arestas == NULL || pai == NULL || rank_ == NULL) {
        free(arestas);
        free(pai);
        free(rank_);
        return -1;
    }

    int m = 0;
    for (int u = 0; u < n; u++)
        for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox)
            if (u < a->destino)
                arestas[m++] = (ArestaK){u, a->destino, a->peso};

    qsort(arestas, (size_t)m, sizeof *arestas, cmp_aresta);

    for (int i = 0; i < n; i++)
        pai[i] = i;

    int postos = 0;
    long total = 0;
    for (int i = 0; i < m && postos < n - 1; i++) {
        int ru = dsu_raiz(pai, arestas[i].u);
        int rv = dsu_raiz(pai, arestas[i].v);
        if (ru == rv)
            continue;                       /* formaria ciclo */
        if (rank_[ru] < rank_[rv]) { int t = ru; ru = rv; rv = t; }
        pai[rv] = ru;
        if (rank_[ru] == rank_[rv])
            rank_[ru]++;
        mst_u[postos] = arestas[i].u;
        mst_v[postos] = arestas[i].v;
        postos++;
        total += arestas[i].peso;
    }

    free(arestas);
    free(pai);
    free(rank_);
    *num_arestas = postos;
    *peso_total = total;
    return postos == n - 1 ? 0 : 1;
}
