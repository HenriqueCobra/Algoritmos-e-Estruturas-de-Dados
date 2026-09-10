# Algoritmos e Estruturas de Dados

Implementações em C (padrão C11) de algoritmos e estruturas de dados clássicos.
Cada tópico fica em um diretório próprio, com a implementação, uma demonstração
executável e um README explicando a ideia e a complexidade.

## Estrutura

```
common/            cabeçalhos e regras de Makefile compartilhados
sorting/           algoritmos de ordenação
data-structures/   listas, pilhas, filas, árvores, tabelas hash
graphs/            representação e algoritmos em grafos
searching/         algoritmos de busca
```

Cada módulo segue o mesmo formato:

```
sorting/bubble-sort/
  bubble_sort.h      interface
  bubble_sort.c      implementação
  main.c             demonstração executável
  Makefile
  README.md
```

## Compilando

Precisa de `gcc` (ou `clang`) e `make`.

```sh
make            # compila e roda a demonstração de todos os módulos
make clean
make list

cd sorting/bubble-sort
make            # compila e executa a demonstração
```

A CI compila tudo com `-Wall -Wextra -Wpedantic` a cada push.

## Conteúdo

### Ordenação
- [x] Bubble sort
- [x] Insertion sort
- [x] Selection sort
- [x] Merge sort
- [x] Quick sort
- [x] Heap sort
- [x] Counting sort
- [ ] Radix sort

### Estruturas de dados
- [ ] Lista encadeada
- [ ] Lista duplamente encadeada
- [ ] Pilha
- [ ] Fila
- [ ] Deque
- [ ] Heap binário / fila de prioridade
- [ ] Tabela hash
- [ ] Árvore binária de busca
- [ ] Árvore AVL
- [ ] Árvore rubro-negra
- [ ] Árvore B
- [ ] Árvore B+
- [ ] Árvore B*
- [ ] Trie
- [ ] Union-Find

### Grafos
- [ ] Representação (lista e matriz de adjacência)
- [ ] BFS e DFS
- [ ] Ordenação topológica
- [ ] Dijkstra
- [ ] Bellman-Ford
- [ ] Floyd-Warshall
- [ ] Árvore geradora mínima (Prim e Kruskal)

### Busca
- [ ] Busca binária
- [ ] Busca em texto (KMP)

## Licença

[MIT](LICENSE)
