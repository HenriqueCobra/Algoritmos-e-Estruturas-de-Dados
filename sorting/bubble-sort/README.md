# Bubble sort

Compara pares de elementos adjacentes e troca os que estão fora de ordem,
repetindo até não haver mais trocas. A cada passada o maior elemento restante
"borbulha" para o fim.

```
[5 2 9 1]  ->  [2 5 1 9]   (9 vai para o fim)
[2 5 1 9]  ->  [2 1 5 9]   (5 no lugar)
[2 1 5 9]  ->  [1 2 5 9]   (pronto)
```

A implementação guarda o índice da última troca de cada passada: tudo depois
dele já está ordenado, então a próxima varredura para nesse ponto. Se uma
passada não faz nenhuma troca, o vetor está ordenado e a função retorna — é o
que dá o melhor caso O(n).

## Complexidade

| Caso  | Tempo |
|-------|-------|
| Melhor| O(n)  |
| Médio | O(n²) |
| Pior  | O(n²) |

Espaço O(1), estável.

## Uso

```c
int v[] = {5, 2, 9, 1};
bubble_sort(v, 4);   // v == {1, 2, 5, 9}
```

```sh
make    # compila e roda a demonstração
```
