#include <assert.h>
#include <stdio.h>

#include "counting_sort.h"

static void mostrar(const int *v, size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%d ", v[i]);
    putchar('\n');
}

int main(void) {
    /* amplitude pequena, incluindo negativos */
    int v[] = {3, -1, 0, 3, 2, -1, 1, 2, 0, 3, -2};
    size_t n = sizeof v / sizeof v[0];

    printf("entrada: ");
    mostrar(v, n);

    counting_sort(v, n);

    printf("saida:   ");
    mostrar(v, n);

    for (size_t i = 1; i < n; i++)
        assert(v[i - 1] <= v[i]);

    return 0;
}
