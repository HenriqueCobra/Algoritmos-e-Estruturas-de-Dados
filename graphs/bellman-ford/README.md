# Bellman-Ford

Caminhos mínimos de uma origem, aceitando **pesos negativos**, e detecção de
**ciclos negativos**.

## Ideia

Um caminho mínimo simples tem no máximo `V-1` arestas. Se, a cada rodada,
relaxarmos **todas** as arestas, depois de `V-1` rodadas toda distância correta
já "propagou" pelo seu caminho. Uma rodada a mais que ainda consiga melhorar
alguma distância só é possível se houver um ciclo de peso negativo — nele, dar
mais uma volta sempre diminui o custo.

```
para rodada = 1 .. V-1:
    para cada aresta (u, v, w):
        se dist[u] + w < dist[v]:
            dist[v] = dist[u] + w
rodada extra: se ainda relaxa alguma aresta  ->  ciclo negativo
```

## Custo

O(V · E). Mais lento que Dijkstra (O((V+E) log V)), mas:

- lida com pesos negativos;
- é mais simples (só uma lista de arestas, sem fila de prioridade);
- distribui bem (é a base do protocolo de roteamento *distance-vector*).

A implementação para mais cedo se uma rodada não muda nada.

## Detalhe

Um ciclo negativo que **não é alcançável** a partir da origem não afeta as
distâncias e não é reportado — a verificação só olha arestas `(u, v)` com `u` já
alcançado.

## Uso

```c
Grafo *g = grafo_criar(5);
grafo_adicionar_aresta(g, 0, 1, 6);
grafo_adicionar_aresta(g, 3, 2, -3);
/* ... */

long dist[5];
int pai[5];
int r = bellman_ford(g, 0, dist, pai);
/* r == 0: ok | r == 1: ciclo negativo | r == -1: erro */

grafo_destruir(g);
```
