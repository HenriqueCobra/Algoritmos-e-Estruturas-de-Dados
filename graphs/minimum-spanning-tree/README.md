# Árvore geradora mínima (MST)

Dado um grafo não dirigido e ponderado **conexo**, a MST é o subconjunto de
arestas que liga todos os vértices com o menor peso total — necessariamente uma
árvore (`V-1` arestas, sem ciclo). Se o grafo é desconexo, obtém-se uma
**floresta** geradora mínima. Aplicações: projeto de redes (cabo, água),
clustering, aproximação do caixeiro-viajante.

Os dois algoritmos são **gulosos** e ambos se apoiam na propriedade do corte:
para qualquer partição dos vértices em dois lados, a aresta mais leve que
cruza o corte pertence a alguma MST.

## Prim

Cresce **uma** árvore a partir de um vértice. Mantém numa fila de prioridade as
arestas que saem da árvore para fora; a cada passo tira a mais leve e incorpora o
vértice do outro lado.

```
árvore = {0}
repete V-1 vezes:
    (u, v) = aresta mais leve com u na árvore e v fora
    adiciona v e (u, v)
```

O(E log V) com heap binário. Aqui o heap usa remoção preguiçosa (empilha
entradas repetidas e ignora as de vértices já incluídos).

## Kruskal

Olha as arestas do grafo inteiro, da mais leve para a mais pesada, e adiciona
cada uma que **não feche um ciclo**. O teste de ciclo é um **Union-Find**: a
aresta `(u, v)` fecha ciclo se `u` e `v` já estão no mesmo conjunto.

```
ordena as arestas por peso
para cada (u, v) em ordem:
    se find(u) != find(v):
        adiciona (u, v); union(u, v)
```

O(E log E) pela ordenação (o Union-Find é praticamente O(1) por operação).

## Prim x Kruskal

| | Prim | Kruskal |
|---|---|---|
| estrutura | fila de prioridade | ordenação + Union-Find |
| bom para | grafos densos | grafos esparsos |
| resultado | uma árvore por vez | funde florestas |

Os dois dão MSTs de mesmo peso; as arestas podem diferir se houver pesos
repetidos.

## Uso

```c
Grafo *g = grafo_criar(9);
grafo_adicionar_aresta(g, 0, 1, 4);
/* ... */

int u[8], v[8], num;
long peso;
mst_kruskal(g, u, v, &num, &peso);   // peso == 37 no exemplo do CLRS

grafo_destruir(g);
```
