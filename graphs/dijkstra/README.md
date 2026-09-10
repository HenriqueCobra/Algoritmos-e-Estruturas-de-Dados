# Dijkstra

Caminhos mínimos de uma origem para todos os vértices, quando **os pesos são não
negativos**.

## Ideia

Mantém uma distância provisória para cada vértice (∞ no começo, 0 na origem) e um
conjunto de vértices já "fechados". Repete:

1. escolhe o vértice não fechado com a menor distância provisória;
2. fecha esse vértice — a distância dele agora é definitiva;
3. **relaxa** cada aresta que sai dele: se `dist[u] + peso(u,v) < dist[v]`,
   atualiza `dist[v]`.

O passo 1 é o gargalo. Com uma **fila de prioridade** (heap binário) ele custa
O(log V), e o total fica **O((V + E) log V)**.

## Por que exige pesos não negativos

O algoritmo assume que, ao fechar o vértice mais próximo, não existe caminho
melhor por vértices ainda abertos — o que só é verdade se nenhum peso é negativo
(uma aresta negativa mais adiante poderia reduzir a distância de um vértice já
fechado). Para pesos negativos, use **Bellman-Ford**.

## Fila de prioridade com remoção preguiçosa

Em vez de atualizar a chave de um vértice no heap (que exigiria um heap
indexado), a implementação empurra uma **nova** entrada `(dist, v)` a cada
relaxamento e, ao desempilhar, ignora entradas cujo vértice já está fechado. Mais
simples, mesma complexidade assintótica; o heap fica com até O(E) entradas.

## Uso

```c
Grafo *g = grafo_criar(5, 1 /* dirigido */);
grafo_adicionar_aresta(g, 0, 1, 10);
grafo_adicionar_aresta(g, 0, 3, 5);
/* ... */

long dist[5];
int pai[5];
dijkstra(g, 0, dist, pai);          // dist[i] = custo mínimo, ou -1

int caminho[5];
long custo;
int t = dijkstra_caminho(g, 0, 2, caminho, &custo);

grafo_destruir(g);
```
