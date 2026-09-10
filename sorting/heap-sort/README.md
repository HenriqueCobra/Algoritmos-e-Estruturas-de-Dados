# Heap sort

Usa um **heap binário** representado implicitamente no vetor: o filho esquerdo de
`i` está em `2i+1`, o direito em `2i+2`. Em um max-heap, todo pai é maior ou
igual aos filhos, então o maior elemento fica sempre na raiz (`v[0]`).

O algoritmo tem duas fases:

1. **Construção** — aplica `peneira` (sift-down) dos nós internos para cima. Como
   metade dos nós são folhas e a maioria fica perto da base, isso custa O(n), e
   não O(n log n).
2. **Ordenação** — troca a raiz (o máximo) com o último elemento do heap, encolhe
   o heap em 1 e peneira a nova raiz. Repetindo, o vetor vai sendo preenchido do
   fim para o começo, em ordem crescente.

## Complexidade

| Caso  | Tempo      |
|-------|------------|
| Todos | O(n log n) |

Espaço O(1) (in-place, sem recursão). Não é estável.

Ao contrário do quicksort, não tem pior caso quadrático; ao contrário do merge
sort, não usa memória extra. Costuma perder em velocidade prática para os dois
por acessar a memória de forma menos previsível para a cache.

## Uso

```c
int v[] = {9, 4, 7, 1, 3};
heap_sort(v, 5);   // {1, 3, 4, 7, 9}
```
