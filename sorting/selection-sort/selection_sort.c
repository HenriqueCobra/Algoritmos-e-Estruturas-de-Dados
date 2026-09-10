#include "selection_sort.h"

void selection_sort(int *v, size_t n) {
    for (size_t i = 0; i + 1 < n; i++) {
        /* Acha o menor elemento de v[i..n) e o coloca em v[i]. */
        size_t menor = i;
        for (size_t j = i + 1; j < n; j++) {
            if (v[j] < v[menor])
                menor = j;
        }

        if (menor != i) {
            int tmp = v[i];
            v[i] = v[menor];
            v[menor] = tmp;
        }
    }
}
