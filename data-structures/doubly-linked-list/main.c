#include <assert.h>
#include <stdio.h>

#include "doubly_linked_list.h"

static void mostrar(const ListaDupla *l) {
    size_t n = lista_dupla_tamanho(l);
    int v[32];
    lista_dupla_para_vetor(l, v);
    printf("[%zu]", n);
    for (size_t i = 0; i < n; i++)
        printf(" %d", v[i]);
    putchar('\n');
}

int main(void) {
    ListaDupla *l = lista_dupla_criar();
    assert(l != NULL);

    lista_dupla_inserir_fim(l, 2);
    lista_dupla_inserir_fim(l, 3);
    lista_dupla_inserir_inicio(l, 1);
    lista_dupla_inserir_em(l, 3, 4);      /* 1 2 3 4 */
    lista_dupla_inserir_em(l, 2, 99);     /* 1 2 99 3 4 */
    printf("lista:    ");
    mostrar(l);
    assert(lista_dupla_tamanho(l) == 5);

    int x;
    assert(lista_dupla_obter(l, 2, &x) == 0 && x == 99);

    size_t pos;
    assert(lista_dupla_buscar(l, 4, &pos) == 0 && pos == 4);

    lista_dupla_remover_valor(l, 99);     /* 1 2 3 4 */
    lista_dupla_remover_inicio(l, &x);
    assert(x == 1);
    lista_dupla_remover_fim(l, &x);
    assert(x == 4);
    printf("apos rem: ");
    mostrar(l);                           /* 2 3 */

    int frente[2], tras[2];
    lista_dupla_para_vetor(l, frente);
    lista_dupla_para_vetor_reverso(l, tras);
    assert(frente[0] == 2 && frente[1] == 3);
    assert(tras[0] == 3 && tras[1] == 2);
    printf("reverso:  %d %d\n", tras[0], tras[1]);

    lista_dupla_destruir(l);
    return 0;
}
