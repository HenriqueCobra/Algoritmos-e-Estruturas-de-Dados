# Floyd-Warshall

Caminhos mínimos entre **todos os pares** de vértices, de uma vez. Aceita pesos
negativos (sem ciclos negativos) e detecta se há um ciclo negativo.

## Ideia (programação dinâmica)

Seja `d(i, j, k)` o menor custo de `i` a `j` usando como intermediários apenas os
vértices `0..k`. Então:

```
d(i, j, k) = min( d(i, j, k-1),                    não usa k
                  d(i, k, k-1) + d(k, j, k-1) )    passa por k
```

Começando com `d(i, j, -1)` = peso da aresta direta (ou ∞), o algoritmo é só
três laços encaixados sobre `k`, `i`, `j`. A dimensão `k` pode ser eliminada:
atualiza-se a matriz no lugar.

```c
para k, para i, para j:
    se dist[i][k] + dist[k][j] < dist[i][j]:
        dist[i][j] = dist[i][k] + dist[k][j]
```

## Custo

O(V³) tempo, O(V²) espaço. Para caminhos mínimos de **todos** os pares num grafo
denso, é mais simples e muitas vezes mais rápido na prática que rodar Dijkstra V
vezes. Para grafos esparsos, V execuções de Dijkstra (O(V·E·log V)) ganham.

## Reconstrução do caminho

A matriz `prox[i][j]` guarda o **próximo** vértice depois de `i` no caminho até
`j`. Na atualização acima, quando o caminho passa a ir por `k`,
`prox[i][j] = prox[i][k]`. `fw_caminho` segue esses ponteiros.

## Ciclo negativo

Se, ao final, algum `dist[i][i] < 0`, existe um ciclo negativo passando por `i`
(ir de `i` a `i` "de graça" deveria custar 0).

## Uso

```c
Grafo *g = grafo_criar(4);
grafo_adicionar_aresta(g, 0, 1, 3);
/* ... */

long dist[16];
int  prox[16];
if (floyd_warshall(g, dist, prox) == 0) {
    long d02 = dist[0 * 4 + 2];
    int caminho[4];
    int t = fw_caminho(4, prox, 0, 2, caminho);
}
grafo_destruir(g);
```
