#include "merge_sort.h"

#include <stdlib.h>

/* Intercala v[lo..mid) e v[mid..hi), ambos já ordenados, usando aux como
   rascunho. A comparação `<` (e não `<=`) preserva a estabilidade. */
static void intercala(int *v, int *aux, size_t lo, size_t mid, size_t hi) {
    for (size_t k = lo; k < hi; k++)
        aux[k] = v[k];

    size_t i = lo, j = mid;
    for (size_t k = lo; k < hi; k++) {
        if (i >= mid)
            v[k] = aux[j++];
        else if (j >= hi)
            v[k] = aux[i++];
        else if (aux[j] < aux[i])
            v[k] = aux[j++];
        else
            v[k] = aux[i++];
    }
}

static void ordena(int *v, int *aux, size_t lo, size_t hi) {
    if (hi - lo < 2)
        return;

    size_t mid = lo + (hi - lo) / 2;
    ordena(v, aux, lo, mid);
    ordena(v, aux, mid, hi);
    intercala(v, aux, lo, mid, hi);
}

void merge_sort(int *v, size_t n) {
    if (n < 2)
        return;

    int *aux = malloc(n * sizeof *aux);
    if (aux == NULL)
        return;

    ordena(v, aux, 0, n);
    free(aux);
}
