#include <assert.h>
#include <stdio.h>

#include "avl_tree.h"

static int anterior;
static int em_ordem_ok = 1;
static size_t contados;

static void checar(int chave, void *ctx) {
    (void)ctx;
    if (contados > 0 && chave <= anterior)
        em_ordem_ok = 0;
    anterior = chave;
    contados++;
}

int main(void) {
    AVL *t = avl_criar();
    assert(t != NULL);

    /* Inserção em ordem crescente: numa BST comum viraria uma lista de altura
       999. Na AVL a altura fica em torno de log2(1000) ~ 10. */
    const int N = 1000;
    for (int i = 1; i <= N; i++)
        assert(avl_inserir(t, i) == 0);
    assert(avl_inserir(t, 500) == 1);

    assert(avl_tamanho(t) == (size_t)N);
    printf("apos %d insercoes em ordem, altura = %d\n", N, avl_altura(t));
    assert(avl_altura(t) <= 15);

    contados = 0;
    em_ordem_ok = 1;
    avl_em_ordem(t, checar, NULL);
    assert(em_ordem_ok && contados == (size_t)N);

    /* remove metade e confere que segue equilibrada e ordenada */
    for (int i = 1; i <= N; i += 2)
        assert(avl_remover(t, i) == 0);
    assert(avl_remover(t, 1) == -1);
    assert(avl_tamanho(t) == (size_t)(N / 2));
    printf("apos remover os impares, altura = %d\n", avl_altura(t));
    assert(avl_altura(t) <= 15);

    contados = 0;
    em_ordem_ok = 1;
    avl_em_ordem(t, checar, NULL);
    assert(em_ordem_ok);

    assert(avl_contem(t, 2) && !avl_contem(t, 3));

    avl_destruir(t);
    printf("avl ok\n");
    return 0;
}
