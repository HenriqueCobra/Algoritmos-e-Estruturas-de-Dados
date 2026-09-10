#ifndef MST_H
#define MST_H

/* Árvore geradora mínima (MST) de um grafo não dirigido e ponderado: um
   subconjunto de arestas que conecta todos os vértices com o menor peso total
   possível (e, sendo árvore, sem ciclos). Se o grafo é desconexo, o resultado é
   uma FLORESTA geradora mínima (uma MST por componente).

   Dois algoritmos gulosos clássicos:
     - Prim:    cresce uma árvore a partir de um vértice, sempre pegando a aresta
                mais leve que sai dela. O(E log V) com fila de prioridade.
     - Kruskal: ordena todas as arestas e adiciona cada uma que não forme ciclo,
                testado com Union-Find. O(E log E). */
typedef struct grafo Grafo;

Grafo *grafo_criar(int n);          /* não dirigido */
void   grafo_destruir(Grafo *g);
int    grafo_num_vertices(const Grafo *g);
/* 0 ok, 1 duplicada, -1 inválida. */
int    grafo_adicionar_aresta(Grafo *g, int u, int v, int peso);

/* Preenchem mst_u[] e mst_v[] (tamanho >= n-1) com as arestas escolhidas,
   *num_arestas com a quantidade, e *peso_total com a soma dos pesos.
   Retornam:
     0  o grafo é conexo (achou n-1 arestas);
     1  o grafo é desconexo (resultado é a floresta geradora mínima);
    -1  falha de alocação. */
int mst_prim(const Grafo *g, int *mst_u, int *mst_v, int *num_arestas, long *peso_total);
int mst_kruskal(const Grafo *g, int *mst_u, int *mst_v, int *num_arestas, long *peso_total);

#endif
