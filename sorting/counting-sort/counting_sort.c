#include "counting_sort.h"

#include <stdlib.h>

void counting_sort(int *v, size_t n) {
    if (n < 2)
        return;

    int min = v[0], max = v[0];
    for (size_t i = 1; i < n; i++) {
        if (v[i] < min) min = v[i];
        if (v[i] > max) max = v[i];
    }

    size_t k = (size_t)(max - min) + 1;
    size_t *cont = calloc(k, sizeof *cont);
    if (cont == NULL)
        return;

    /* Conta quantas vezes cada valor aparece (deslocado por min). */
    for (size_t i = 0; i < n; i++)
        cont[v[i] - min]++;

    /* Reescreve o vetor em ordem, cada valor repetido conforme a contagem. */
    size_t pos = 0;
    for (size_t c = 0; c < k; c++) {
        while (cont[c] > 0) {
            v[pos++] = min + (int)c;
            cont[c]--;
        }
    }

    free(cont);
}
