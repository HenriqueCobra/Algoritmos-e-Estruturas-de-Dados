#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <limits.h>

/* Caminhos mínimos entre TODOS os pares de vértices, com pesos possivelmente
   negativos (sem ciclos negativos). Programação dinâmica sobre "qual o menor
   caminho de i a j usando apenas os vértices 0..k como intermediários?".
   O(V^3) tempo, O(V^2) espaço. */
typedef struct grafo Grafo;

Grafo *grafo_criar(int n);          /* sempre dirigido */
void   grafo_destruir(Grafo *g);
int    grafo_num_vertices(const Grafo *g);
/* Peso pode ser negativo. Se a aresta já existe, fica com o menor peso.
   0 em sucesso, -1 se índice inválido. */
int    grafo_adicionar_aresta(Grafo *g, int u, int v, int peso);

#define FW_INFINITO LONG_MAX

/* `dist` e `prox` são matrizes n*n (linha-maior), fornecidas pelo chamador.
   dist[i*n + j] = menor custo de i a j (FW_INFINITO se não há caminho).
   prox[i*n + j] = próximo vértice depois de i no caminho até j (-1 se não há
                   caminho); use com fw_caminho. `prox` pode ser NULL.
   Retorna 0; 1 se detectar um ciclo negativo (algum dist[i*n+i] < 0). */
int floyd_warshall(const Grafo *g, long *dist, int *prox);

/* Reconstrói o caminho de i a j em `caminho` (tamanho >= n) a partir de `prox`.
   Retorna o nº de vértices, ou -1 se não há caminho. */
int fw_caminho(int n, const int *prox, int i, int j, int *caminho);

#endif
