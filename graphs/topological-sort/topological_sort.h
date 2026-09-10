#ifndef TOPOLOGICAL_SORT_H
#define TOPOLOGICAL_SORT_H

/* Ordenação topológica de um grafo dirigido acíclico (DAG): uma ordem linear
   dos vértices em que toda aresta u->v aparece com u antes de v. É a ordem em
   que se pode executar tarefas com dependências, compilar módulos, etc.
   Existe se e somente se o grafo não tem ciclo. */
typedef struct grafo Grafo;

Grafo *grafo_criar(int n);          /* sempre dirigido */
void   grafo_destruir(Grafo *g);
int    grafo_num_vertices(const Grafo *g);
int    grafo_adicionar_aresta(Grafo *g, int u, int v);   /* 0 ok, 1 dup, -1 inválida */

/* Algoritmo de Kahn: repetidamente remove um vértice sem dependências pendentes
   (grau de entrada 0). Escreve a ordem em `ordem` (tamanho >= n).
   Retorna 0 em sucesso, -1 se o grafo tem ciclo. O(V + E). */
int ordenacao_topologica(const Grafo *g, int *ordem);

/* Alternativa via DFS: a ordem é a dos tempos de finalização em ordem
   decrescente. Mesmo contrato. */
int ordenacao_topologica_dfs(const Grafo *g, int *ordem);

#endif
