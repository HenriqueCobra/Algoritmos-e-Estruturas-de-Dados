# Quick sort

Divisão e conquista in-place: escolhe um **pivô**, particiona o vetor em
"menores ou iguais" à esquerda e "maiores ou iguais" à direita, e ordena cada
lado recursivamente. Sem passo de combinação — quando as partições estão
ordenadas, o vetor todo está.

## Decisões desta implementação

- **Pivô por mediana de três** (`v[lo]`, `v[meio]`, `v[hi]`): evita o pior caso
  O(n²) em entradas já ordenadas ou invertidas e ainda deixa `v[lo]` e `v[hi]`
  como sentinelas da partição.
- **Partição de Hoare**: dois ponteiros que se aproximam do centro; faz cerca de
  3x menos trocas que a partição de Lomuto e lida bem com muitos elementos
  repetidos.
- **Recursão só no lado menor**, iterando no maior: garante profundidade de
  pilha O(log n) mesmo quando a partição é ruim.

Uma otimização comum não incluída aqui para manter o foco: trocar para
insertion sort quando o subvetor fica pequeno (~10 elementos).

## Complexidade

| Caso   | Tempo      |
|--------|------------|
| Melhor | O(n log n) |
| Médio  | O(n log n) |
| Pior   | O(n²)      |

Pilha O(log n), sem alocação no heap. Não é estável.

## Uso

```c
int v[] = {5, 3, 8, 1, 9, 2};
quick_sort(v, 6);   // {1, 2, 3, 5, 8, 9}
```
