#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include <stddef.h>

/* Ordena v[0..n) em ordem crescente.
   O(n^2) em todos os casos, mas faz no máximo n-1 trocas.
   In-place; não é estável. */
void selection_sort(int *v, size_t n);

#endif
