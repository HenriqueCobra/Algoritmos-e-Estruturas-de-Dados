#ifndef COUNTING_SORT_H
#define COUNTING_SORT_H

#include <stddef.h>

/* Ordena v[0..n) em ordem crescente contando ocorrências.
   O(n + k), onde k = max - min + 1 é a amplitude dos valores. Só vale a pena
   quando k é da ordem de n. Aloca O(k); se falhar, o vetor fica intacto. */
void counting_sort(int *v, size_t n);

#endif
