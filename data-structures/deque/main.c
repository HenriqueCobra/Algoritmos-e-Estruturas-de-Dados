#include <assert.h>
#include <stdio.h>

#include "deque.h"

int main(void) {
    Deque *d = deque_criar();
    assert(d != NULL && deque_vazio(d));

    /* monta 3 2 1 _ 4 5 6 alternando as pontas e passando da capacidade inicial */
    deque_inserir_fim(d, 4);
    deque_inserir_inicio(d, 3);
    deque_inserir_inicio(d, 2);
    deque_inserir_inicio(d, 1);
    deque_inserir_fim(d, 5);
    deque_inserir_fim(d, 6);
    for (int i = 7; i <= 20; i++)
        assert(deque_inserir_fim(d, i) == 0);
    assert(deque_tamanho(d) == 20);

    int f, t;
    deque_frente(d, &f);
    deque_tras(d, &t);
    assert(f == 1 && t == 20);

    int x;
    deque_remover_inicio(d, &x); assert(x == 1);
    deque_remover_inicio(d, &x); assert(x == 2);
    deque_remover_fim(d, &x);    assert(x == 20);
    deque_remover_fim(d, &x);    assert(x == 19);

    printf("da frente pro fim:");
    while (deque_remover_inicio(d, &x) == 0)
        printf(" %d", x);          /* 3 4 5 6 7 8 ... 18 */
    putchar('\n');

    assert(deque_vazio(d));
    assert(deque_remover_fim(d, &x) == -1);

    deque_destruir(d);
    return 0;
}
