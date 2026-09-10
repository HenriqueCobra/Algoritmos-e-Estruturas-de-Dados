# Union-Find (DSU)

Mantém uma coleção de **conjuntos disjuntos** sobre `{0, 1, ..., n-1}` e suporta
duas operações:

- `encontrar(x)` — qual o representante do conjunto de `x`?
- `unir(a, b)` — funde os conjuntos de `a` e `b`.

Com isso, "`a` e `b` estão juntos?" é só `encontrar(a) == encontrar(b)`.

## Representação

Uma **floresta**: cada elemento aponta para um "pai"; a raiz de cada árvore é o
representante do conjunto. `unir` liga a raiz de um na raiz do outro.

```
pai:  [0] [0] [1] [1] [4] ...
       0<-1<-2                   conjunto {0,1,2}, representante 0
              \-3
```

## As duas otimizações

1. **União por tamanho** — sempre pendura a árvore menor sob a maior, para não
   criar árvores altas.
2. **Compressão de caminho** — durante `encontrar`, aproxima os nós visitados da
   raiz (aqui, apontando cada um para o avô).

Juntas, deixam cada operação em **O(α(n)) amortizado**, onde α é a inversa da
função de Ackermann — ≤ 4 para qualquer `n` imaginável. Na prática, constante.

## Onde se usa

- Kruskal: aceitar uma aresta só se ela conecta dois componentes diferentes.
- Detectar ciclo enquanto se adicionam arestas.
- Componentes conexos em processamento offline.

## Uso

```c
DSU *d = dsu_criar(10);
dsu_unir(d, 1, 2);
dsu_unir(d, 2, 3);

dsu_conectados(d, 1, 3);        // 1
dsu_num_conjuntos(d);          // 8

dsu_destruir(d);
```
