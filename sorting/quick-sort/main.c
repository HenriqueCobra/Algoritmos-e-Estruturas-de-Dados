#include <assert.h>
#include <stdio.h>

#include "quick_sort.h"

static void mostrar(const int *v, size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%d ", v[i]);
    putchar('\n');
}

int main(void) {
    /* muitos valores repetidos: a partição de Hoare lida bem com isso */
    int v[] = {5, 3, 8, 3, 5, 1, 8, 5, 3, 1, 9, 5};
    size_t n = sizeof v / sizeof v[0];

    printf("entrada: ");
    mostrar(v, n);

    quick_sort(v, n);

    printf("saida:   ");
    mostrar(v, n);

    for (size_t i = 1; i < n; i++)
        assert(v[i - 1] <= v[i]);

    return 0;
}
