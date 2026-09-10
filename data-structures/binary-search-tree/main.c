#include <assert.h>
#include <stdio.h>

#include "binary_search_tree.h"

struct coletor {
    int *v;
    size_t n;
};

static void coletar(int chave, void *ctx) {
    struct coletor *c = ctx;
    c->v[c->n++] = chave;
}

int main(void) {
    BST *t = bst_criar();
    assert(t != NULL);

    int chaves[] = {50, 30, 70, 20, 40, 60, 80, 35, 45};
    size_t n = sizeof chaves / sizeof chaves[0];
    for (size_t i = 0; i < n; i++)
        assert(bst_inserir(t, chaves[i]) == 0);

    assert(bst_inserir(t, 40) == 1);          /* duplicada */
    assert(bst_tamanho(t) == n);

    assert(bst_contem(t, 35) && !bst_contem(t, 99));

    int x;
    bst_minimo(t, &x); assert(x == 20);
    bst_maximo(t, &x); assert(x == 80);

    /* remove nós com 0, 1 e 2 filhos */
    assert(bst_remover(t, 20) == 0);          /* folha */
    assert(bst_remover(t, 70) == 0);          /* dois filhos */
    assert(bst_remover(t, 100) == -1);

    int v[16];
    struct coletor c = {v, 0};
    bst_em_ordem(t, coletar, &c);

    printf("em ordem:");
    for (size_t i = 0; i < c.n; i++) {
        printf(" %d", v[i]);
        if (i > 0)
            assert(v[i - 1] < v[i]);          /* sempre crescente */
    }
    printf("\naltura: %d\n", bst_altura(t));

    bst_destruir(t);
    return 0;
}
