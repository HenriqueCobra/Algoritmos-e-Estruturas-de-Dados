#include <assert.h>
#include <stdio.h>

#include "topological_sort.h"

int main(void) {
    /* dependências de "vestir-se" (exemplo clássico) */
    enum { MEIA, SAPATO, CUECA, CALCA, CINTO, CAMISA, GRAVATA, PALETO, N };
    int arestas[][2] = {
        {MEIA, SAPATO}, {CUECA, CALCA}, {CALCA, SAPATO}, {CALCA, CINTO},
        {CAMISA, CINTO}, {CAMISA, GRAVATA}, {CINTO, PALETO}, {GRAVATA, PALETO},
    };
    size_t m = sizeof arestas / sizeof arestas[0];

    Grafo *g = grafo_criar(N);
    for (size_t i = 0; i < m; i++)
        assert(grafo_adicionar_aresta(g, arestas[i][0], arestas[i][1]) == 0);

    int ordem[N], pos[N];
    assert(ordenacao_topologica(g, ordem) == 0);
    for (int i = 0; i < N; i++)
        pos[ordem[i]] = i;
    for (size_t i = 0; i < m; i++)
        assert(pos[arestas[i][0]] < pos[arestas[i][1]]);

    printf("Kahn: ");
    const char *nome[] = {"meia","sapato","cueca","calca","cinto","camisa","gravata","paleto"};
    for (int i = 0; i < N; i++)
        printf("%s ", nome[ordem[i]]);
    putchar('\n');

    int ordem2[N];
    assert(ordenacao_topologica_dfs(g, ordem2) == 0);
    for (int i = 0; i < N; i++)
        pos[ordem2[i]] = i;
    for (size_t i = 0; i < m; i++)
        assert(pos[arestas[i][0]] < pos[arestas[i][1]]);

    grafo_destruir(g);

    /* com ciclo: as duas versões devem recusar */
    Grafo *c = grafo_criar(3);
    grafo_adicionar_aresta(c, 0, 1);
    grafo_adicionar_aresta(c, 1, 2);
    grafo_adicionar_aresta(c, 2, 0);
    int lixo[3];
    assert(ordenacao_topologica(c, lixo) == -1);
    assert(ordenacao_topologica_dfs(c, lixo) == -1);
    grafo_destruir(c);

    printf("ordenacao topologica ok\n");
    return 0;
}
