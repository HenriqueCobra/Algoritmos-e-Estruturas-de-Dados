#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "floyd_warshall.h"

int main(void) {
    /* CLRS 25.1: 5 vértices, com pesos negativos, sem ciclo negativo */
    int n = 5;
    Grafo *g = grafo_criar(n);
    struct { int u, v, w; } E[] = {
        {0,1,3},{0,2,8},{0,4,-4},{1,3,1},{1,4,7},
        {2,1,4},{3,0,2},{3,2,-5},{4,3,6},
    };
    for (size_t i = 0; i < sizeof E / sizeof E[0]; i++)
        assert(grafo_adicionar_aresta(g, E[i].u, E[i].v, E[i].w) == 0);

    long *dist = malloc((size_t)n * n * sizeof *dist);
    int *prox = malloc((size_t)n * n * sizeof *prox);
    assert(floyd_warshall(g, dist, prox) == 0);

    long esperado[5][5] = {
        {  0,  1, -3,  2, -4},
        {  3,  0, -4,  1, -1},
        {  7,  4,  0,  5,  3},
        {  2, -1, -5,  0, -2},
        {  8,  5,  1,  6,  0},
    };
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            assert(dist[i * n + j] == esperado[i][j]);
    printf("matriz de distancias (CLRS 25.1) confere\n");

    int caminho[5];
    int t = fw_caminho(n, prox, 0, 2, caminho);   /* 0 -> 4 -> 3 -> 2, custo -3 */
    printf("caminho 0->2:");
    for (int i = 0; i < t; i++)
        printf(" %d", caminho[i]);
    putchar('\n');
    assert(t == 4 && caminho[0] == 0 && caminho[3] == 2);

    free(dist);
    free(prox);
    grafo_destruir(g);

    /* ciclo negativo */
    Grafo *c = grafo_criar(3);
    grafo_adicionar_aresta(c, 0, 1, 1);
    grafo_adicionar_aresta(c, 1, 2, -3);
    grafo_adicionar_aresta(c, 2, 0, 1);
    long d[9];
    assert(floyd_warshall(c, d, NULL) == 1);
    grafo_destruir(c);

    printf("floyd-warshall ok\n");
    return 0;
}
