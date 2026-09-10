#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include <stddef.h>

/* Ordena v[0..n) em ordem crescente usando um max-heap montado sobre o próprio
   vetor. O(n log n) em todos os casos, O(1) de memória extra. Não é estável. */
void heap_sort(int *v, size_t n);

#endif
