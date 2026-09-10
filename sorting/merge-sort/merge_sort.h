#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <stddef.h>

/* Ordena v[0..n) em ordem crescente (merge sort top-down).
   O(n log n) em todos os casos. Estável.
   Usa O(n) de memória auxiliar; se a alocação falhar, o vetor fica intacto. */
void merge_sort(int *v, size_t n);

#endif
