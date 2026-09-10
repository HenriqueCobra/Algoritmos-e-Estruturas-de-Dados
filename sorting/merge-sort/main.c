#include <assert.h>
#include <stdio.h>

#include "merge_sort.h"

static void mostrar(const int *v, size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%d ", v[i]);
    putchar('\n');
}

int main(void) {
    int v[] = {38, 27, 43, 3, 9, 82, 10, 55, 1, 20, 17, 6};
    size_t n = sizeof v / sizeof v[0];

    printf("entrada: ");
    mostrar(v, n);

    merge_sort(v, n);

    printf("saida:   ");
    mostrar(v, n);

    for (size_t i = 1; i < n; i++)
        assert(v[i - 1] <= v[i]);

    return 0;
}
