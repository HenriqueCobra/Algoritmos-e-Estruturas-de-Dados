#include "quick_sort.h"

static void troca(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/* Ordena v[lo], v[mid], v[hi] entre si e devolve o valor do meio como pivô.
   Além de escolher um pivô razoável, isso garante que v[lo] <= pivô <= v[hi],
   o que serve de sentinela para a partição não sair do intervalo. */
static int mediana3(int *v, size_t lo, size_t hi) {
    size_t mid = lo + (hi - lo) / 2;
    if (v[mid] < v[lo])  troca(&v[mid], &v[lo]);
    if (v[hi]  < v[lo])  troca(&v[hi],  &v[lo]);
    if (v[hi]  < v[mid]) troca(&v[hi],  &v[mid]);
    return v[mid];
}

/* Partição de Hoare sobre o intervalo fechado [lo, hi].
   Ao terminar, v[lo..j] <= pivô <= v[j+1..hi] e lo <= j < hi. */
static size_t particiona(int *v, size_t lo, size_t hi) {
    int p = mediana3(v, lo, hi);
    size_t i = lo, j = hi;
    for (;;) {
        while (v[i] < p) i++;
        while (v[j] > p) j--;
        if (i >= j)
            return j;
        troca(&v[i], &v[j]);
        i++;
        j--;
    }
}

static void ordena(int *v, size_t lo, size_t hi) {
    while (lo < hi) {
        size_t q = particiona(v, lo, hi);

        /* Recorre no lado menor e continua o laço no maior: a profundidade da
           recursão fica em O(log n) mesmo no pior caso de partição. */
        if (q - lo < hi - q) {
            ordena(v, lo, q);
            lo = q + 1;
        } else {
            ordena(v, q + 1, hi);
            hi = q;
        }
    }
}

void quick_sort(int *v, size_t n) {
    if (n > 1)
        ordena(v, 0, n - 1);
}
