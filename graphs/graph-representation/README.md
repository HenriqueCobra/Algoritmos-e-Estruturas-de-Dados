# Representação de grafos

Um grafo tem vértices e arestas. As duas formas usuais de guardá-lo na memória:

## Lista de adjacência (usada aqui)

Para cada vértice, uma lista dos seus vizinhos.

```
0 -> 1 -> 2
1 -> 3
2 -> 3
3 -> 4
4 -> 0
5 -> 3
```

| Operação                  | Custo         |
|---------------------------|---------------|
| espaço                    | O(V + E)      |
| existe aresta u–v?        | O(grau(u))    |
| percorrer vizinhos de u   | O(grau(u))    |
| adicionar aresta          | O(grau(u))\*  |

\* O(1) se a lista não for mantida ordenada e não checar duplicata.

É a escolha padrão: a maioria dos grafos reais é **esparsa** (E ≈ V, não V²), e
quase todo algoritmo (BFS, DFS, Dijkstra, ...) só precisa percorrer vizinhos.

## Matriz de adjacência

Uma matriz V×V onde `M[u][v]` indica se existe a aresta (ou guarda o peso).

```c
int **m = /* V x V, zerada */;
m[u][v] = 1;                 /* e m[v][u] = 1 se não dirigido */
int existe = m[u][v];       /* O(1) */
```

| Operação                  | Custo   |
|---------------------------|---------|
| espaço                    | O(V²)   |
| existe aresta u–v?        | O(1)    |
| percorrer vizinhos de u   | O(V)    |

Vale a pena para grafos **densos** (E ≈ V²), quando o teste O(1) compensa, ou em
algoritmos que já são O(V³) como Floyd-Warshall.

## Dirigido x não dirigido

Numa aresta não dirigida `u–v`, os dois vértices entram na lista um do outro. Num
dígrafo, só `v` entra na lista de `u`; daí a distinção entre **grau de saída** e
**grau de entrada**.

## Interface

Grafo simples (sem laços nem arestas paralelas), vértices `0..n-1`. As listas são
mantidas ordenadas por índice do vizinho, o que torna a iteração determinística.

## Uso

```c
Grafo *g = grafo_criar(5, 0 /* não dirigido */);
grafo_adicionar_aresta(g, 0, 1);
grafo_adicionar_aresta(g, 1, 2);

grafo_tem_aresta(g, 2, 1);        // 1
grafo_grau_saida(g, 1);          // 2

grafo_destruir(g);
```
