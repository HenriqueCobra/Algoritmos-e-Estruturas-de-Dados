#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <limits.h>

/* Caminhos mínimos de uma origem, permitindo pesos NEGATIVOS. Também detecta
   ciclos de peso negativo (nos quais "caminho mínimo" deixa de fazer sentido).

   Ideia: relaxar TODAS as arestas repetidamente. Depois de V-1 rodadas, se não
   há ciclo negativo, todas as distâncias estão corretas (um caminho mínimo tem
   no máximo V-1 arestas). Se uma V-ésima rodada ainda melhora algo, existe um
   ciclo negativo alcançável. O(V · E). */
typedef struct grafo Grafo;

Grafo *grafo_criar(int n);          /* sempre dirigido */
void   grafo_destruir(Grafo *g);
int    grafo_num_vertices(const Grafo *g);
/* Peso pode ser negativo. 0 em sucesso, -1 se índice inválido ou alloc. */
int    grafo_adicionar_aresta(Grafo *g, int u, int v, int peso);

#define BF_INFINITO LONG_MAX

/* dist[i] = menor custo de `origem` a `i`, ou BF_INFINITO se inalcançável.
   pai[i] = antecessor no caminho (-1); pode ser NULL.
   Retorna:
     0  sucesso;
     1  existe ciclo negativo alcançável a partir de `origem` (dist não é
        confiável para os vértices afetados);
    -1  falha de alocação. */
int bellman_ford(const Grafo *g, int origem, long *dist, int *pai);

#endif
