#include <assert.h>
#include <stdio.h>

#include "graph.h"

static void imprime_vizinho(int v, void *ctx) {
    (void)ctx;
    printf(" %d", v);
}

int main(void) {
    /* dígrafo com 6 vértices */
    Grafo *g = grafo_criar(6, 1);
    assert(g != NULL);

    int arestas[][2] = {{0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}, {4, 0}, {5, 3}};
    for (size_t i = 0; i < sizeof arestas / sizeof arestas[0]; i++)
        assert(grafo_adicionar_aresta(g, arestas[i][0], arestas[i][1]) == 0);

    assert(grafo_adicionar_aresta(g, 0, 1) == 1);     /* duplicada */
    assert(grafo_adicionar_aresta(g, 0, 0) == -1);    /* laço */
    assert(grafo_num_arestas(g) == 7);

    assert(grafo_tem_aresta(g, 3, 4) && !grafo_tem_aresta(g, 4, 3));

    for (int u = 0; u < grafo_num_vertices(g); u++) {
        printf("%d: saida=%d entrada=%d ->", u,
               grafo_grau_saida(g, u), grafo_grau_entrada(g, u));
        grafo_para_cada_vizinho(g, u, imprime_vizinho, NULL);
        putchar('\n');
    }

    grafo_remover_aresta(g, 4, 0);
    assert(!grafo_tem_aresta(g, 4, 0));
    assert(grafo_grau_entrada(g, 0) == 0);
    grafo_destruir(g);

    /* mesmo grafo, não dirigido: cada aresta conta dos dois lados */
    Grafo *h = grafo_criar(4, 0);
    grafo_adicionar_aresta(h, 0, 1);
    grafo_adicionar_aresta(h, 1, 2);
    grafo_adicionar_aresta(h, 2, 0);
    assert(grafo_tem_aresta(h, 1, 0) && grafo_tem_aresta(h, 0, 1));
    assert(grafo_grau_saida(h, 0) == 2);
    grafo_destruir(h);

    printf("representacao de grafo ok\n");
    return 0;
}
