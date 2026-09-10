#include <assert.h>
#include <stdio.h>

#include "heap_sort.h"

static void mostrar(const int *v, size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%d ", v[i]);
    putchar('\n');
}

int main(void) {
    /* ordem decrescente: pior caso para muitos algoritmos, indiferente aqui */
    int v[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    size_t n = sizeof v / sizeof v[0];

    printf("entrada: ");
    mostrar(v, n);

    heap_sort(v, n);

    printf("saida:   ");
    mostrar(v, n);

    for (size_t i = 1; i < n; i++)
        assert(v[i - 1] <= v[i]);

    return 0;
}
