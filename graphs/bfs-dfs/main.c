#include <assert.h>
#include <stdio.h>

#include "bfs_dfs.h"

int main(void) {
    /*   0 - 1 - 2        componente {0,1,2,3,4}
          \  |  /
           \ | /
             3 - 4        e um vértice isolado: 5, 6 - 7 separados
    */
    Grafo *g = grafo_criar(8, 0);
    int e[][2] = {{0,1},{1,2},{0,3},{1,3},{2,3},{3,4},{6,7}};
    for (size_t i = 0; i < sizeof e / sizeof e[0]; i++)
        assert(grafo_adicionar_aresta(g, e[i][0], e[i][1]) == 0);

    int dist[8], pai[8];
    bfs(g, 0, dist, pai);
    assert(dist[0] == 0 && dist[1] == 1 && dist[3] == 1 && dist[2] == 2 && dist[4] == 2);
    assert(dist[5] == -1 && dist[7] == -1);
    printf("BFS de 0: ");
    for (int i = 0; i < 8; i++)
        printf("%d ", dist[i]);
    putchar('\n');

    int caminho[8];
    int t = bfs_caminho(g, 0, 4, caminho);
    assert(t == 3);                       /* 0 -> 3 -> 4 */
    printf("caminho 0->4:");
    for (int i = 0; i < t; i++)
        printf(" %d", caminho[i]);
    putchar('\n');
    assert(caminho[0] == 0 && caminho[t - 1] == 4);
    assert(bfs_caminho(g, 0, 5, caminho) == -1);

    int desc[8], fim[8];
    int arvores = dfs(g, desc, fim, NULL);
    assert(arvores == 3);                 /* {0..4}, {5}, {6,7} */
    for (int i = 0; i < 8; i++)
        assert(desc[i] < fim[i]);

    int comp[8];
    int k = componentes_conexos(g, comp);
    assert(k == 3);
    assert(comp[0] == comp[4] && comp[6] == comp[7] && comp[5] != comp[0]);

    assert(tem_ciclo(g));                 /* 0-1-3-0 */
    grafo_destruir(g);

    Grafo *arvore = grafo_criar(4, 0);
    grafo_adicionar_aresta(arvore, 0, 1);
    grafo_adicionar_aresta(arvore, 1, 2);
    grafo_adicionar_aresta(arvore, 1, 3);
    assert(!tem_ciclo(arvore));
    grafo_destruir(arvore);

    Grafo *dag = grafo_criar(3, 1);
    grafo_adicionar_aresta(dag, 0, 1);
    grafo_adicionar_aresta(dag, 1, 2);
    assert(!tem_ciclo(dag));
    grafo_adicionar_aresta(dag, 2, 0);
    assert(tem_ciclo(dag));
    grafo_destruir(dag);

    printf("BFS/DFS ok\n");
    return 0;
}
