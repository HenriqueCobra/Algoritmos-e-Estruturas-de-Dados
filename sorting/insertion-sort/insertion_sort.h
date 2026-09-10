#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include <stddef.h>

/* Ordena v[0..n) em ordem crescente.
   O(n^2) no caso médio e pior, O(n) quando o vetor já está quase ordenado.
   Estável e in-place. */
void insertion_sort(int *v, size_t n);

#endif
