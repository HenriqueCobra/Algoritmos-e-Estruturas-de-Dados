#include <assert.h>
#include <stdio.h>

#include "trie.h"

int main(void) {
    Trie *t = trie_criar();
    assert(t != NULL);

    const char *palavras[] = {"casa", "casal", "casar", "caso", "carro", "sol"};
    size_t n = sizeof palavras / sizeof palavras[0];
    for (size_t i = 0; i < n; i++)
        assert(trie_inserir(t, palavras[i]) == 0);

    assert(trie_inserir(t, "casa") == 1);      /* já existe */
    assert(trie_inserir(t, "Casa") == -1);     /* fora de a..z */
    assert(trie_tamanho(t) == n);

    assert(trie_contem(t, "casa"));
    assert(!trie_contem(t, "cas"));            /* é prefixo, não palavra */
    assert(trie_tem_prefixo(t, "cas"));
    assert(trie_tem_prefixo(t, "carr"));
    assert(!trie_tem_prefixo(t, "xyz"));

    /* remover "casa" não pode afetar "casal"/"casar"/"caso" */
    assert(trie_remover(t, "casa") == 0);
    assert(!trie_contem(t, "casa"));
    assert(trie_contem(t, "casal") && trie_contem(t, "caso"));
    assert(trie_tem_prefixo(t, "casa"));       /* ainda há "casal", "casar" */

    assert(trie_remover(t, "casa") == -1);
    assert(trie_remover(t, "carro") == 0);
    assert(!trie_tem_prefixo(t, "carr"));      /* nós liberados */

    assert(trie_tamanho(t) == 4);
    printf("trie ok: %zu palavras\n", trie_tamanho(t));

    trie_destruir(t);
    return 0;
}
