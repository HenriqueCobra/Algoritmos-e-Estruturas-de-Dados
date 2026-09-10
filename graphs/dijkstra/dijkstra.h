#ifndef DIJKSTRA_H
#define DIJKSTRA_H

/* Caminhos mínimos de uma origem para todos os vértices, num grafo com pesos
   NÃO NEGATIVOS. Usa uma fila de prioridade (heap binário): a cada passo, fecha
   o vértice não visitado mais próximo da origem e relaxa as suas arestas.
   O((V + E) log V). */
typedef struct grafo Grafo;

Grafo *grafo_criar(int n, int dirigido);
void   grafo_destruir(Grafo *g);
int    grafo_num_vertices(const Grafo *g);
/* Adiciona a aresta com peso >= 0. 0 ok, 1 duplicada, -1 inválida. */
int    grafo_adicionar_aresta(Grafo *g, int u, int v, int peso);

/* dist[i] = menor custo de `origem` a `i`, ou -1 se inalcançável.
   pai[i] = antecessor no caminho mínimo (-1 na origem e nos inalcançáveis);
   pode ser NULL. Arrays de tamanho n. Retorna 0, ou -1 em falha de alocação. */
int dijkstra(const Grafo *g, int origem, long *dist, int *pai);

/* Escreve em `caminho` (tamanho >= n) a rota de menor custo de `origem` a
   `destino` e, se `custo` != NULL, o custo total. Retorna o nº de vértices do
   caminho, ou -1 se `destino` é inalcançável. */
int dijkstra_caminho(const Grafo *g, int origem, int destino,
                     int *caminho, long *custo);

#endif
