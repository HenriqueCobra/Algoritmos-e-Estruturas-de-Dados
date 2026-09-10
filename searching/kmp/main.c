#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "kmp.h"

static void imprime_pos(size_t pos, void *ctx) {
    (void)ctx;
    printf(" %zu", pos);
}

int main(void) {
    /* tabela de falha de "ababaca" -> 0 0 1 2 3 0 1 (exemplo do CLRS) */
    const char *p = "ababaca";
    size_t tab[7];
    kmp_tabela(p, 7, tab);
    size_t esperado[] = {0, 0, 1, 2, 3, 0, 1};
    printf("tabela de \"%s\":", p);
    for (int i = 0; i < 7; i++) {
        printf(" %zu", tab[i]);
        assert(tab[i] == esperado[i]);
    }
    putchar('\n');

    const char *texto = "abababzabababa";
    assert(kmp_buscar(texto, "ababa") == 0);
    assert(kmp_buscar(texto, "z") == 6);
    assert(kmp_buscar(texto, "xyz") == -1);
    assert(kmp_buscar(texto, "") == 0);

    /* ocorrências sobrepostas de "aba": posições 0, 2, 7, 9, 11 */
    printf("\"aba\" em \"%s\":", texto);
    size_t c = kmp_buscar_todas(texto, "aba", imprime_pos, NULL);
    putchar('\n');
    assert(c == 5);

    /* "aaaa": "aa" ocorre em 0,1,2 (sobrepostas) */
    assert(kmp_buscar_todas("aaaa", "aa", NULL, NULL) == 3);

    /* pior caso da busca ingênua, trivial para o KMP */
    char grande[1001];
    memset(grande, 'a', 1000);
    grande[1000] = '\0';
    char pad[51];
    memset(pad, 'a', 49);
    pad[49] = 'b';
    pad[50] = '\0';
    assert(kmp_buscar(grande, pad) == -1);   /* nunca casa, mas em O(n) */

    printf("KMP ok\n");
    return 0;
}
