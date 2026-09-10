# Selection sort

A cada passo, procura o menor elemento do trecho ainda não ordenado e o troca
para a primeira posição desse trecho. Depois de `i` passos, `v[0..i)` já contém
os `i` menores valores, em ordem.

```
[29 10 14 37 13 8 1]   menor = 1   -> troca com a posição 0
[ 1 10 14 37 13 8 29]  menor = 8   -> troca com a posição 1
[ 1  8 14 37 13 10 29] menor = 10  -> ...
```

Faz sempre O(n²) comparações, independentemente da entrada, mas no máximo `n-1`
trocas — útil quando escrever na memória é caro. Não é estável na versão com
troca direta (elementos iguais podem ter a ordem relativa invertida).

## Complexidade

| Caso  | Tempo | Trocas |
|-------|-------|--------|
| Todos | O(n²) | O(n)   |

Espaço O(1).

## Uso

```c
int v[] = {29, 10, 14, 1};
selection_sort(v, 4);   // {1, 10, 14, 29}
```
