#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"

int main(void) {
    TabelaHash *t = tabela_criar();
    assert(t != NULL);

    tabela_por(t, "um", 1);
    tabela_por(t, "dois", 2);
    tabela_por(t, "tres", 3);

    int v;
    assert(tabela_obter(t, "dois", &v) == 0 && v == 2);
    assert(tabela_obter(t, "quatro", &v) == -1);

    tabela_por(t, "dois", 22);              /* atualiza, não duplica */
    assert(tabela_obter(t, "dois", &v) == 0 && v == 22);
    assert(tabela_tamanho(t) == 3);

    /* força vários reespalhamentos */
    char chave[32];
    for (int i = 0; i < 1000; i++) {
        snprintf(chave, sizeof chave, "chave-%d", i);
        assert(tabela_por(t, chave, i) == 0);
    }
    assert(tabela_tamanho(t) == 1003);

    for (int i = 0; i < 1000; i++) {
        snprintf(chave, sizeof chave, "chave-%d", i);
        assert(tabela_obter(t, chave, &v) == 0 && v == i);
    }

    assert(tabela_remover(t, "chave-500") == 0);
    assert(tabela_remover(t, "chave-500") == -1);
    assert(!tabela_contem(t, "chave-500"));
    assert(tabela_tamanho(t) == 1002);

    printf("tabela ok: %zu pares apos as operacoes\n", tabela_tamanho(t));

    tabela_destruir(t);
    return 0;
}
