#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "binary_search.h"

int main(void) {
    int v[] = {1, 3, 3, 3, 5, 8, 13, 21, 34};
    size_t n = sizeof v / sizeof v[0];

    assert(busca_binaria(v, n, 13) == 6);
    assert(busca_binaria(v, n, 4) == -1);
    assert(busca_binaria(v, n, 1) == 0);
    assert(busca_binaria(v, n, 34) == (long)n - 1);

    /* três cópias do 3, nos índices 1..3 */
    assert(limite_inferior(v, n, 3) == 1);
    assert(limite_superior(v, n, 3) == 4);
    assert(limite_superior(v, n, 3) - limite_inferior(v, n, 3) == 3);

    /* valor ausente: as duas fronteiras coincidem no ponto de inserção */
    assert(limite_inferior(v, n, 4) == 4 && limite_superior(v, n, 4) == 4);
    assert(limite_inferior(v, n, 0) == 0);
    assert(limite_inferior(v, n, 100) == n);

    printf("busca por 3: [%zu, %zu) -> %zu ocorrencia(s)\n",
           limite_inferior(v, n, 3), limite_superior(v, n, 3),
           limite_superior(v, n, 3) - limite_inferior(v, n, 3));

    /* teste aleatório contra busca linear */
    int *a = malloc(2000 * sizeof *a);
    for (int i = 0; i < 2000; i++)
        a[i] = (rand() % 500) * 2;            /* ordenado? não — precisa ordenar */
    for (int i = 1; i < 2000; i++)            /* insertion sort rápido pro teste */
        for (int j = i; j > 0 && a[j - 1] > a[j]; j--) {
            int t = a[j]; a[j] = a[j - 1]; a[j - 1] = t;
        }
    for (int q = 0; q < 5000; q++) {
        int alvo = rand() % 1200;
        long bb = busca_binaria(a, 2000, alvo);
        int linear = -1;
        for (int i = 0; i < 2000; i++)
            if (a[i] == alvo) { linear = i; break; }
        assert((bb == -1) == (linear == -1));
        if (bb != -1)
            assert(a[bb] == alvo);
    }
    free(a);

    printf("busca binaria ok\n");
    return 0;
}
