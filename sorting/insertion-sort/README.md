# Insertion sort

Constrói a parte ordenada do vetor um elemento por vez: pega o próximo valor e o
"insere" na posição certa entre os que já estão ordenados, deslocando os maiores
para a direita. É como a maioria das pessoas ordena um punhado de cartas na mão.

```
[ 3 | 1 4 2 ]   ordenado = {3}
[ 1 3 | 4 2 ]   insere 1
[ 1 3 4 | 2 ]   insere 4 (já está no lugar)
[ 1 2 3 4 ]     insere 2
```

Para um vetor já (ou quase) ordenado, cada inserção faz poucas comparações e o
custo cai para O(n). Por isso é usado como caso base em quicksort/mergesort
quando o subvetor fica pequeno.

## Complexidade

| Caso  | Tempo |
|-------|-------|
| Melhor| O(n)  |
| Médio | O(n²) |
| Pior  | O(n²) |

Espaço O(1), estável.

## Uso

```c
int v[] = {3, 1, 4, 2};
insertion_sort(v, 4);   // {1, 2, 3, 4}
```
