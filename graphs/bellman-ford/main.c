#include <assert.h>
#include <stdio.h>

#include "bellman_ford.h"

int main(void) {
    /* CLRS 24.4: s=0 t=1 x=2 y=3 z=4, com arestas negativas
       s->t:6  s->y:7  t->x:5  t->y:8  t->z:-4  x->t:-2  y->x:-3  y->z:9  z->s:2  z->x:7 */
    Grafo *g = grafo_criar(5);
    struct { int u, v, w; } E[] = {
        {0,1,6},{0,3,7},{1,2,5},{1,3,8},{1,4,-4},{2,1,-2},
        {3,2,-3},{3,4,9},{4,0,2},{4,2,7},
    };
    for (size_t i = 0; i < sizeof E / sizeof E[0]; i++)
        assert(grafo_adicionar_aresta(g, E[i].u, E[i].v, E[i].w) == 0);

    long dist[5];
    int pai[5];
    assert(bellman_ford(g, 0, dist, pai) == 0);

    long esperado[] = {0, 2, 4, 7, -2};
    printf("distancias de s:");
    for (int i = 0; i < 5; i++) {
        printf(" %ld", dist[i]);
        assert(dist[i] == esperado[i]);
    }
    putchar('\n');
    grafo_destruir(g);

    /* ciclo negativo alcançável: 0->1 (1), 1->2 (-1), 2->1 (-1) */
    Grafo *c = grafo_criar(3);
    grafo_adicionar_aresta(c, 0, 1, 1);
    grafo_adicionar_aresta(c, 1, 2, -1);
    grafo_adicionar_aresta(c, 2, 1, -1);
    long d[3];
    assert(bellman_ford(c, 0, d, NULL) == 1);
    grafo_destruir(c);

    /* ciclo negativo que NÃO é alcançável da origem: não deve ser reportado */
    Grafo *k = grafo_criar(4);
    grafo_adicionar_aresta(k, 0, 1, 5);
    grafo_adicionar_aresta(k, 2, 3, -1);
    grafo_adicionar_aresta(k, 3, 2, -1);
    long d2[4];
    assert(bellman_ford(k, 0, d2, NULL) == 0);
    assert(d2[1] == 5 && d2[2] == BF_INFINITO);
    grafo_destruir(k);

    printf("bellman-ford ok\n");
    return 0;
}
