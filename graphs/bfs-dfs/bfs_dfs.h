#ifndef BFS_DFS_H
#define BFS_DFS_H

/* Grafo não ponderado (lista de adjacência) e as duas travessias fundamentais:
   busca em largura (BFS) e busca em profundidade (DFS). Quase todo algoritmo de
   grafo é uma dessas duas com alguma informação extra registrada durante a
   visita. Ambas custam O(V + E). */
typedef struct grafo Grafo;

Grafo *grafo_criar(int n, int dirigido);
void   grafo_destruir(Grafo *g);
int    grafo_num_vertices(const Grafo *g);
/* 0 ok, 1 duplicada, -1 inválida. */
int    grafo_adicionar_aresta(Grafo *g, int u, int v);

/* BFS a partir de `origem`. Visita os vértices em ordem de distância, então
   dist[i] é o menor número de arestas de `origem` a `i` (-1 se inalcançável) —
   o caminho mínimo em grafo não ponderado. pai[i] = antecessor na árvore BFS
   (-1 na origem e nos inalcançáveis). Arrays de tamanho n; pai pode ser NULL. */
void bfs(const Grafo *g, int origem, int *dist, int *pai);

/* Escreve em `caminho` (tamanho >= n) a sequência de vértices de `origem` a
   `destino` no caminho mínimo. Retorna o comprimento em vértices, ou -1 se não
   há caminho. */
int bfs_caminho(const Grafo *g, int origem, int destino, int *caminho);

/* DFS cobrindo todos os vértices (floresta DFS). Preenche, para cada vértice,
   o tempo de descoberta e o de finalização (contador único que anda a cada
   evento) e o pai na floresta. Passe NULL no que não interessar. Retorna o
   número de árvores da floresta. */
int dfs(const Grafo *g, int *descoberta, int *finalizacao, int *pai);

/* Rotula cada vértice com o índice do seu componente conexo (0..k-1) e retorna
   k. Assume grafo não dirigido. */
int componentes_conexos(const Grafo *g, int *componente);

/* 1 se há ciclo, 0 caso contrário. Funciona para grafo dirigido e não dirigido. */
int tem_ciclo(const Grafo *g);

#endif
