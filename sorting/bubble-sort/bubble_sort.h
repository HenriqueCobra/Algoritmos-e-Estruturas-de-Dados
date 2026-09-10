#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include <stddef.h>

/* Ordena v[0..n) em ordem crescente.
   O(n^2) no caso médio e pior, O(n) quando o vetor já está ordenado.
   Estável e in-place. */
void bubble_sort(int *v, size_t n);

#endif
