#include <assert.h>
#include <stdio.h>

#include "queue.h"

int main(void) {
    Fila *f = fila_criar();
    assert(f != NULL && fila_vazia(f));

    for (int i = 1; i <= 5; i++)
        assert(fila_enfileirar(f, i) == 0);
    assert(fila_tamanho(f) == 5);

    int frente;
    fila_frente(f, &frente);
    assert(frente == 1);

    /* consome metade e enfileira mais, checando a ordem FIFO */
    int x;
    fila_desenfileirar(f, &x); assert(x == 1);
    fila_desenfileirar(f, &x); assert(x == 2);
    fila_enfileirar(f, 6);
    fila_enfileirar(f, 7);

    printf("saindo da fila:");
    while (fila_desenfileirar(f, &x) == 0)
        printf(" %d", x);          /* 3 4 5 6 7 */
    putchar('\n');

    assert(fila_vazia(f));
    assert(fila_frente(f, &x) == -1);

    fila_destruir(f);
    return 0;
}
