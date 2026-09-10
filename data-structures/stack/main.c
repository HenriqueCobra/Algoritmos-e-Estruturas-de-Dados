#include <assert.h>
#include <stdio.h>

#include "stack.h"

/* Uso clássico de pilha: verificar se os parênteses/colchetes estão balanceados. */
static int balanceado(const char *s) {
    Pilha *p = pilha_criar();
    if (p == NULL)
        return 0;

    int ok = 1;
    for (size_t i = 0; s[i] != '\0' && ok; i++) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            pilha_empilhar(p, c);
        } else if (c == ')' || c == ']' || c == '}') {
            int topo;
            if (pilha_desempilhar(p, &topo) != 0)
                ok = 0;
            else if ((c == ')' && topo != '(') ||
                     (c == ']' && topo != '[') ||
                     (c == '}' && topo != '{'))
                ok = 0;
        }
    }
    ok = ok && pilha_vazia(p);
    pilha_destruir(p);
    return ok;
}

int main(void) {
    Pilha *p = pilha_criar();
    assert(p != NULL && pilha_vazia(p));

    for (int i = 1; i <= 20; i++)          /* força o crescimento do vetor */
        assert(pilha_empilhar(p, i) == 0);
    assert(pilha_tamanho(p) == 20);

    int topo;
    pilha_topo(p, &topo);
    assert(topo == 20);

    int x;
    for (int esperado = 20; esperado >= 1; esperado--) {
        pilha_desempilhar(p, &x);
        assert(x == esperado);
    }
    assert(pilha_desempilhar(p, &x) == -1);
    pilha_destruir(p);

    assert(balanceado("{[()()]}"));
    assert(!balanceado("([)]"));
    assert(!balanceado("((("));
    printf("pilha ok; \"{[()()]}\" balanceado, \"([)]\" nao\n");

    return 0;
}
