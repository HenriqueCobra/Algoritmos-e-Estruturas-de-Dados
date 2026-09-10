# BFS e DFS

As duas travessias de grafo. Ambas visitam cada vértice e cada aresta uma vez:
**O(V + E)**. A diferença é a ordem.

## BFS — busca em largura

Usa uma **fila**. Visita a origem, depois todos os vizinhos dela, depois os
vizinhos dos vizinhos... em camadas. Como cada camada está a uma aresta de
distância da anterior, a BFS dá o **caminho mínimo em número de arestas** num
grafo não ponderado.

```
origem 0:   camada 0 = {0}
            camada 1 = {1, 3}
            camada 2 = {2, 4}
```

`bfs` preenche `dist[]` e `pai[]`; `bfs_caminho` segue os `pai` de trás para
frente para reconstruir a rota.

## DFS — busca em profundidade

Usa uma **pilha** (aqui, a pilha de recursão). Vai fundo por um caminho até não
poder mais, então volta e tenta outro. Registrando o **tempo de descoberta** e o
de **finalização** de cada vértice, saem de graça:

- classificação de arestas (árvore, retorno, avanço, cruzamento);
- ordenação topológica (ordem decrescente de finalização);
- componentes fortemente conexos.

## O que este módulo entrega

| Função                 | O quê                                             |
|------------------------|---------------------------------------------------|
| `bfs`                  | distâncias e árvore de caminhos mínimos           |
| `bfs_caminho`          | reconstrói a rota origem→destino                  |
| `dfs`                  | floresta DFS com tempos de descoberta/finalização |
| `componentes_conexos`  | rotula os componentes (grafo não dirigido)        |
| `tem_ciclo`            | detecta ciclo (dirigido: aresta de retorno;       |
|                        | não dirigido: vizinho visitado que não é o pai)   |

## Uso

```c
Grafo *g = grafo_criar(6, 0);
grafo_adicionar_aresta(g, 0, 1);
grafo_adicionar_aresta(g, 1, 2);

int dist[6], pai[6];
bfs(g, 0, dist, pai);      // dist[2] == 2

grafo_destruir(g);
```

As buscas por componente e a BFS são iterativas; a DFS e a detecção de ciclo são
recursivas (profundidade até O(V)).
