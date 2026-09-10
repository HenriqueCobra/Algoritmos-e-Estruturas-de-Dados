#include <assert.h>
#include <stdio.h>

#include "dijkstra.h"

int main(void) {
    /* grafo do exemplo clássico (CLRS), dirigido
         s->t:10  s->y:5  t->y:2  t->x:1  y->t:3  y->x:9  y->z:2
         x->z:4   z->x:6  z->s:7
       vértices: s=0 t=1 x=2 y=3 z=4 */
    Grafo *g = grafo_criar(5, 1);
    struct { int u, v, w; } E[] = {
        {0,1,10},{0,3,5},{1,3,2},{1,2,1},{3,1,3},{3,2,9},{3,4,2},
        {2,4,4},{4,2,6},{4,0,7},
    };
    for (size_t i = 0; i < sizeof E / sizeof E[0]; i++)
        assert(grafo_adicionar_aresta(g, E[i].u, E[i].v, E[i].w) == 0);

    long dist[5];
    int pai[5];
    assert(dijkstra(g, 0, dist, pai) == 0);

    long esperado[] = {0, 8, 9, 5, 7};
    printf("distancias de s:");
    for (int i = 0; i < 5; i++) {
        printf(" %ld", dist[i]);
        assert(dist[i] == esperado[i]);
    }
    putchar('\n');

    int caminho[5];
    long custo;
    int t = dijkstra_caminho(g, 0, 2, caminho, &custo);
    assert(t == 4 && custo == 9);        /* s -> y -> t -> x */
    printf("caminho s->x (custo %ld):", custo);
    for (int i = 0; i < t; i++)
        printf(" %d", caminho[i]);
    putchar('\n');

    grafo_destruir(g);

    /* inalcançável */
    Grafo *h = grafo_criar(3, 1);
    grafo_adicionar_aresta(h, 0, 1, 4);
    long d2[3];
    dijkstra(h, 0, d2, NULL);
    assert(d2[2] == -1);
    assert(dijkstra_caminho(h, 0, 2, caminho, NULL) == -1);
    grafo_destruir(h);

    /* peso negativo é recusado na construção */
    Grafo *k = grafo_criar(2, 1);
    assert(grafo_adicionar_aresta(k, 0, 1, -1) == -1);
    grafo_destruir(k);

    printf("dijkstra ok\n");
    return 0;
}
