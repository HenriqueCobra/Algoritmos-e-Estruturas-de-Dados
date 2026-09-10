# Ordenação topológica

Dado um grafo dirigido acíclico (DAG), produz uma ordem linear dos vértices em
que toda aresta `u -> v` tem `u` antes de `v`. É a ordem para executar tarefas
respeitando dependências: compilar arquivos, resolver células de planilha,
montar um cronograma.

Só existe se o grafo **não tem ciclo** — uma dependência circular não pode ser
linearizada. As duas funções detectam isso e retornam -1.

```
calca -> cinto -> paleto        uma ordem válida:
camisa -> gravata -> paleto     cueca calca camisa meia gravata cinto sapato paleto
```

## Kahn (baseado em grau de entrada)

1. Conte o grau de entrada de cada vértice.
2. Coloque numa fila os de grau 0 (sem dependências).
3. Retire um da fila, coloque na saída, e decremente o grau de entrada dos seus
   vizinhos; os que chegam a 0 entram na fila.
4. Se ao final sobraram vértices, há um ciclo.

O(V + E). É o mais intuitivo e o mais fácil de adaptar (ex.: para achar *a* ordem
lexicograficamente menor, troque a fila por uma fila de prioridade).

## Baseado em DFS

Faça uma DFS. Ao **finalizar** um vértice (todos os descendentes já visitados),
coloque-o na frente da lista. A ordem final é a das finalizações ao contrário.
Um ciclo aparece como uma aresta para um vértice ainda "na pilha" (cinza).

## Uso

```c
Grafo *g = grafo_criar(4);
grafo_adicionar_aresta(g, 0, 1);
grafo_adicionar_aresta(g, 0, 2);
grafo_adicionar_aresta(g, 1, 3);
grafo_adicionar_aresta(g, 2, 3);

int ordem[4];
if (ordenacao_topologica(g, ordem) == 0) { /* usa ordem */ }

grafo_destruir(g);
```
