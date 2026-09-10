#include <assert.h>
#include <stdio.h>

#include "mst.h"

int main(void) {
    /* CLRS 23.1: 9 vértices (a..i = 0..8), MST de peso 37 */
    Grafo *g = grafo_criar(9);
    struct { int u, v, w; } E[] = {
        {0,1,4},{0,7,8},{1,2,8},{1,7,11},{2,3,7},{2,8,2},{2,5,4},
        {3,4,9},{3,5,14},{4,5,10},{5,6,2},{6,7,1},{6,8,6},{7,8,7},
    };
    for (size_t i = 0; i < sizeof E / sizeof E[0]; i++)
        assert(grafo_adicionar_aresta(g, E[i].u, E[i].v, E[i].w) == 0);

    int u[8], v[8], num;
    long peso;

    assert(mst_prim(g, u, v, &num, &peso) == 0);
    assert(num == 8 && peso == 37);
    printf("Prim:    peso %ld, arestas", peso);
    for (int i = 0; i < num; i++)
        printf(" %d-%d", u[i], v[i]);
    putchar('\n');

    assert(mst_kruskal(g, u, v, &num, &peso) == 0);
    assert(num == 8 && peso == 37);
    printf("Kruskal: peso %ld, arestas", peso);
    for (int i = 0; i < num; i++)
        printf(" %d-%d", u[i], v[i]);
    putchar('\n');

    grafo_destruir(g);

    /* grafo desconexo: floresta geradora mínima */
    Grafo *f = grafo_criar(5);
    grafo_adicionar_aresta(f, 0, 1, 3);
    grafo_adicionar_aresta(f, 1, 2, 1);
    grafo_adicionar_aresta(f, 3, 4, 5);      /* {3,4} separado de {0,1,2} */
    int fu[4], fv[4], fn;
    long fp;
    assert(mst_prim(f, fu, fv, &fn, &fp) == 1);
    assert(fn == 3 && fp == 9);
    assert(mst_kruskal(f, fu, fv, &fn, &fp) == 1);
    assert(fn == 3 && fp == 9);
    grafo_destruir(f);

    printf("MST ok\n");
    return 0;
}
