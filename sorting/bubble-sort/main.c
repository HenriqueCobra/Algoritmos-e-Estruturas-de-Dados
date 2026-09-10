#include <assert.h>
#include <stdio.h>

#include "bubble_sort.h"

static void mostrar(const int *v, size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%d ", v[i]);
    putchar('\n');
}

int main(void) {
    int v[] = {5, 2, 9, 1, 5, 6, 0, -3, 8, 2};
    size_t n = sizeof v / sizeof v[0];

    printf("entrada: ");
    mostrar(v, n);

    bubble_sort(v, n);

    printf("saida:   ");
    mostrar(v, n);

    for (size_t i = 1; i < n; i++)
        assert(v[i - 1] <= v[i]);

    return 0;
}
