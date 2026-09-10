#include <assert.h>
#include <stdio.h>

#include "binary_heap.h"

int main(void) {
    /* heapify de um vetor pronto */
    int entrada[] = {9, 4, 7, 1, 8, 3, 6, 2, 5, 0};
    size_t n = sizeof entrada / sizeof entrada[0];

    Heap *h = heap_criar_de(entrada, n);
    assert(h != NULL);
    assert(heap_tamanho(h) == n);

    int menor;
    heap_min(h, &menor);
    assert(menor == 0);

    /* insere mais alguns depois de construído */
    heap_inserir(h, -3);
    heap_inserir(h, 10);

    printf("ordem de saida:");
    int anterior = -1000;
    int x;
    while (heap_remover_min(h, &x) == 0) {
        printf(" %d", x);
        assert(x >= anterior);      /* sai sempre em ordem crescente */
        anterior = x;
    }
    putchar('\n');

    assert(heap_vazio(h));
    assert(heap_remover_min(h, &x) == -1);

    heap_destruir(h);
    return 0;
}
