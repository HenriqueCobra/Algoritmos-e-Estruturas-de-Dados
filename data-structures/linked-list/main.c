#include <assert.h>
#include <stdio.h>

#include "linked_list.h"

static void mostrar(const Lista *l) {
    size_t n = lista_tamanho(l);
    printf("[%zu]", n);
    for (size_t i = 0; i < n; i++) {
        int x;
        lista_obter(l, i, &x);
        printf(" %d", x);
    }
    putchar('\n');
}

int main(void) {
    Lista *l = lista_criar();
    assert(l != NULL);
    assert(lista_vazia(l));

    for (int i = 1; i <= 5; i++)
        lista_inserir_fim(l, i * 10);
    lista_inserir_inicio(l, 5);
    lista_inserir_em(l, 3, 99);        /* 5 10 20 [99] 30 40 50 */
    printf("apos insercoes: ");
    mostrar(l);
    assert(lista_tamanho(l) == 7);

    size_t pos;
    assert(lista_buscar(l, 99, &pos) == 0 && pos == 3);
    assert(lista_buscar(l, 12345, NULL) == -1);

    int removido;
    lista_remover_inicio(l, &removido);
    assert(removido == 5);
    lista_remover_fim(l, &removido);
    assert(removido == 50);
    lista_remover_valor(l, 99);
    printf("apos remocoes:  ");
    mostrar(l);                        /* 10 20 30 40 */

    lista_inverter(l);
    printf("invertida:      ");
    mostrar(l);                        /* 40 30 20 10 */

    int esperado[] = {40, 30, 20, 10};
    int v[4];
    lista_para_vetor(l, v);
    for (int i = 0; i < 4; i++)
        assert(v[i] == esperado[i]);

    lista_destruir(l);
    return 0;
}
