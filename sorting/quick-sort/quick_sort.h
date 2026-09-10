#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <stddef.h>

/* Ordena v[0..n) em ordem crescente (quicksort in-place).
   Pivô por mediana de três e partição de Hoare; recorre no subvetor menor e
   itera no maior, o que limita a pilha a O(log n).
   O(n log n) em média, O(n^2) no pior caso. Não é estável. */
void quick_sort(int *v, size_t n);

#endif
