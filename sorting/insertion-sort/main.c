#include <assert.h>
#include <stdio.h>

#include "insertion_sort.h"

static void mostrar(const int *v, size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%d ", v[i]);
    putchar('\n');
}

int main(void) {
    /* quase ordenado: caso em que o insertion sort brilha */
    int v[] = {1, 2, 4, 3, 5, 7, 6, 8, 10, 9};
    size_t n = sizeof v / sizeof v[0];

    printf("entrada: ");
    mostrar(v, n);

    insertion_sort(v, n);

    printf("saida:   ");
    mostrar(v, n);

    for (size_t i = 1; i < n; i++)
        assert(v[i - 1] <= v[i]);

    return 0;
}
