#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "b_tree.h"

static int anterior;
static int ordenado;
static size_t contados;

static void checar(int chave, void *ctx) {
    (void)ctx;
    if (contados > 0 && chave <= anterior)
        ordenado = 0;
    anterior = chave;
    contados++;
}

int main(void) {
    ArvoreB *t = btree_criar();
    assert(t != NULL);

    const int N = 5000;
    int *chaves = malloc(N * sizeof *chaves);
    for (int i = 0; i < N; i++)
        chaves[i] = i;
    for (int i = N - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = chaves[i]; chaves[i] = chaves[j]; chaves[j] = tmp;
    }

    for (int i = 0; i < N; i++)
        assert(btree_inserir(t, chaves[i]) == 0);
    assert(btree_inserir(t, chaves[0]) == 1);
    assert(btree_tamanho(t) == (size_t)N);
    assert(btree_valida(t));
    printf("apos %d insercoes: altura = %d (nós com 2..5 chaves)\n",
           N, btree_altura(t));

    for (int i = 0; i < N; i += 3) {
        assert(btree_remover(t, chaves[i]) == 0);
        if (i % 300 == 0)
            assert(btree_valida(t));
    }
    assert(btree_valida(t));

    contados = 0;
    ordenado = 1;
    btree_em_ordem(t, checar, NULL);
    assert(ordenado && contados == btree_tamanho(t));

    for (int i = 0; i < N; i += 3)
        assert(!btree_contem(t, chaves[i]));
    for (int i = 1; i < N; i += 3)
        assert(btree_contem(t, chaves[i]));

    printf("arvore B ok: %zu chaves, altura %d\n",
           btree_tamanho(t), btree_altura(t));

    free(chaves);
    btree_destruir(t);
    return 0;
}
