# Merge sort

Divisão e conquista: parte o vetor no meio, ordena cada metade recursivamente e
depois **intercala** as duas metades ordenadas em tempo linear.

```
[38 27 43 3 9 82 10 55]
        /            \
[38 27 43 3]      [9 82 10 55]
   ...               ...
[3 27 38 43]  +  [9 10 55 82]
        \            /
   [3 9 10 27 38 43 55 82]
```

A intercalação percorre as duas metades com dois ponteiros, sempre copiando o
menor dos dois valores da frente. Como em empate escolhemos o elemento da metade
da esquerda, a ordem relativa de valores iguais é mantida — o algoritmo é
estável.

O preço é O(n) de memória extra para o vetor de rascunho. Esta versão aloca um
único buffer do tamanho de `n` e o reaproveita em todas as intercalações.

## Complexidade

| Caso  | Tempo      |
|-------|------------|
| Todos | O(n log n) |

Espaço O(n), estável.

## Uso

```c
int v[] = {5, 2, 4, 1, 3};
merge_sort(v, 5);   // {1, 2, 3, 4, 5}
```
