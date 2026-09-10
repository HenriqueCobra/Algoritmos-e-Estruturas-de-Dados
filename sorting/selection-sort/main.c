#include <assert.h>
#include <stdio.h>

#include "selection_sort.h"

static void mostrar(const int *v, size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%d ", v[i]);
    putchar('\n');
}

int main(void) {
    int v[] = {29, 10, 14, 37, 13, 8, 1};
    size_t n = sizeof v / sizeof v[0];

    printf("entrada (%zu elementos): ", n);
    mostrar(v, n);

    selection_sort(v, n);

    printf("saida:                  ");
    mostrar(v, n);

    for (size_t i = 1; i < n; i++)
        assert(v[i - 1] <= v[i]);

    return 0;
}
