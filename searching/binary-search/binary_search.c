#include "binary_search.h"

long busca_binaria(const int *v, size_t n, int alvo) {
    size_t lo = 0, hi = n;        /* intervalo semiaberto [lo, hi) */
    while (lo < hi) {
        size_t meio = lo + (hi - lo) / 2;
        if (v[meio] == alvo)
            return (long)meio;
        if (v[meio] < alvo)
            lo = meio + 1;
        else
            hi = meio;
    }
    return -1;
}

size_t limite_inferior(const int *v, size_t n, int alvo) {
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t meio = lo + (hi - lo) / 2;
        if (v[meio] < alvo)
            lo = meio + 1;       /* v[meio] cedo demais: descarta até meio */
        else
            hi = meio;           /* v[meio] >= alvo: candidato, olha à esquerda */
    }
    return lo;
}

size_t limite_superior(const int *v, size_t n, int alvo) {
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t meio = lo + (hi - lo) / 2;
        if (v[meio] <= alvo)
            lo = meio + 1;
        else
            hi = meio;
    }
    return lo;
}
