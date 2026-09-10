# Heap binário / fila de prioridade

Uma **fila de prioridade** entrega sempre o elemento de maior prioridade (aqui, o
**menor** valor). O heap binário é a implementação usual: uma árvore binária
*quase completa* guardada num vetor, sem ponteiros.

```
índice:   0   1   2   3   4   5
valor:   [1] [2] [3] [5] [4] [9]

           1
         /   \
        2     3
       / \   /
      5   4 9
```

Para o nó `i`: pai em `(i-1)/2`, filhos em `2i+1` e `2i+2`. A **propriedade de
heap** é que todo pai é ≤ os filhos, então a raiz é o mínimo.

## Operações

| Operação        | Como funciona                                   | Custo    |
|-----------------|-------------------------------------------------|----------|
| `heap_min`      | lê `dados[0]`                                    | O(1)     |
| `heap_inserir`  | põe no fim e **sobe** enquanto menor que o pai   | O(log n) |
| `heap_remover_min` | tira a raiz, traz o último pro topo e **desce** | O(log n) |
| `heap_criar_de` | copia o vetor e aplica `descer` de baixo pra cima | O(n)    |

`heap_criar_de` construir em O(n) (e não O(n log n) de n inserções) é o mesmo
truque do heap sort: a maioria dos nós está perto das folhas e desce pouco.

## Uso

```c
Heap *h = heap_criar();
heap_inserir(h, 5);
heap_inserir(h, 1);
heap_inserir(h, 3);

int x;
heap_remover_min(h, &x);   // x == 1

heap_destruir(h);
```

Para uma **max**-heap, basta inverter as comparações em `subir` e `descer`.
