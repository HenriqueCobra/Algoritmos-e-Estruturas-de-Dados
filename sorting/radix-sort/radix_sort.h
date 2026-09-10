#ifndef RADIX_SORT_H
#define RADIX_SORT_H

#include <stddef.h>

/* Ordena v[0..n) em ordem crescente (LSD radix sort, base 256).
   Supõe int de 32 bits: 4 passadas estáveis de counting sort, uma por byte.
   O(n) no tempo (4 passadas + montagem das chaves), O(n) de memória extra.
   Se a alocação falhar, o vetor fica intacto. */
void radix_sort(int *v, size_t n);

#endif
