#include "bfs_dfs.h"

#include <stdlib.h>

typedef struct aresta {
    int destino;
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

static int ligar(Grafo *g, int u, int v) {
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

int grafo_adicionar_aresta(Grafo *g, int u, int v) {
    if (u < 0 || v < 0 || u >= g->n || v >= g->n || u == v)
        return -1;
    int r = ligar(g, u, v);
    if (r != 0)
        return r;
    if (!g->dirigido && ligar(g, v, u) == -1) {
        /* desfaz */
        Aresta **p = &g->adj[u];
        while (*p != NULL && (*p)->destino != v)
            p = &(*p)->prox;
        if (*p != NULL) {
            Aresta *lixo = *p;
            *p = (*p)->prox;
            free(lixo);
        }
        return -1;
    }
    return 0;
}

void bfs(const Grafo *g, int origem, int *dist, int *pai) {
    for (int i = 0; i < g->n; i++) {
        dist[i] = -1;
        if (pai != NULL)
            pai[i] = -1;
    }
    if (origem < 0 || origem >= g->n)
        return;

    int *fila = malloc((size_t)g->n * sizeof *fila);
    if (fila == NULL)
        return;
    int ini = 0, fim = 0;

    dist[origem] = 0;
    fila[fim++] = origem;

    while (ini < fim) {
        int u = fila[ini++];
        for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
            if (dist[a->destino] == -1) {
                dist[a->destino] = dist[u] + 1;
                if (pai != NULL)
                    pai[a->destino] = u;
                fila[fim++] = a->destino;
            }
        }
    }
    free(fila);
}

int bfs_caminho(const Grafo *g, int origem, int destino, int *caminho) {
    if (origem < 0 || destino < 0 || origem >= g->n || destino >= g->n)
        return -1;

    int *dist = malloc((size_t)g->n * sizeof *dist);
    int *pai = malloc((size_t)g->n * sizeof *pai);
    if (dist == NULL || pai == NULL) {
        free(dist);
        free(pai);
        return -1;
    }

    bfs(g, origem, dist, pai);

    int tam = -1;
    if (dist[destino] != -1) {
        tam = dist[destino] + 1;
        for (int v = destino, i = tam - 1; v != -1; v = pai[v], i--)
            caminho[i] = v;
    }

    free(dist);
    free(pai);
    return tam;
}

static void dfs_visita(const Grafo *g, int u, int *cor, int *relogio,
                       int *descoberta, int *finalizacao, int *pai) {
    cor[u] = 1;
    if (descoberta != NULL)
        descoberta[u] = (*relogio)++;
    else
        (*relogio)++;

    for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
        if (cor[a->destino] == 0) {
            if (pai != NULL)
                pai[a->destino] = u;
            dfs_visita(g, a->destino, cor, relogio, descoberta, finalizacao, pai);
        }
    }

    cor[u] = 2;
    if (finalizacao != NULL)
        finalizacao[u] = (*relogio)++;
    else
        (*relogio)++;
}

int dfs(const Grafo *g, int *descoberta, int *finalizacao, int *pai) {
    int *cor = calloc((size_t)g->n, sizeof *cor);
    if (cor == NULL)
        return -1;
    if (pai != NULL)
        for (int i = 0; i < g->n; i++)
            pai[i] = -1;

    int relogio = 0;
    int arvores = 0;
    for (int u = 0; u < g->n; u++) {
        if (cor[u] == 0) {
            arvores++;
            dfs_visita(g, u, cor, &relogio, descoberta, finalizacao, pai);
        }
    }
    free(cor);
    return arvores;
}

int componentes_conexos(const Grafo *g, int *componente) {
    for (int i = 0; i < g->n; i++)
        componente[i] = -1;

    int *pilha = malloc((size_t)g->n * sizeof *pilha);
    if (pilha == NULL)
        return -1;

    int k = 0;
    for (int s = 0; s < g->n; s++) {
        if (componente[s] != -1)
            continue;
        int topo = 0;
        pilha[topo++] = s;
        componente[s] = k;
        while (topo > 0) {
            int u = pilha[--topo];
            for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
                if (componente[a->destino] == -1) {
                    componente[a->destino] = k;
                    pilha[topo++] = a->destino;
                }
            }
        }
        k++;
    }
    free(pilha);
    return k;
}

/* --- detecção de ciclo --- */

static int ciclo_dirigido(const Grafo *g, int u, int *cor) {
    cor[u] = 1;                       /* na pilha de recursão */
    for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
        if (cor[a->destino] == 1)
            return 1;                 /* aresta de retorno */
        if (cor[a->destino] == 0 && ciclo_dirigido(g, a->destino, cor))
            return 1;
    }
    cor[u] = 2;
    return 0;
}

static int ciclo_nao_dirigido(const Grafo *g, int u, int origem, int *visitado) {
    visitado[u] = 1;
    for (const Aresta *a = g->adj[u]; a != NULL; a = a->prox) {
        if (!visitado[a->destino]) {
            if (ciclo_nao_dirigido(g, a->destino, u, visitado))
                return 1;
        } else if (a->destino != origem) {
            return 1;                 /* vizinho já visitado que não é o pai */
        }
    }
    return 0;
}

int tem_ciclo(const Grafo *g) {
    int *marca = calloc((size_t)g->n, sizeof *marca);
    if (marca == NULL)
        return 0;

    int achou = 0;
    for (int u = 0; u < g->n && !achou; u++) {
        if (marca[u] == 0) {
            if (g->dirigido)
                achou = ciclo_dirigido(g, u, marca);
            else
                achou = ciclo_nao_dirigido(g, u, -1, marca);
        }
    }
    free(marca);
    return achou;
}
