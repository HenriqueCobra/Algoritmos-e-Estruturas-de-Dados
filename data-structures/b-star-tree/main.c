#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "b_star_tree.h"

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
    ArvoreBStar *t = bstar_criar();
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
        assert(bstar_inserir(t, chaves[i]) == 0);
    assert(bstar_inserir(t, chaves[0]) == 1);
    assert(bstar_tamanho(t) == (size_t)N);
    assert(bstar_valida(t));
    printf("apos %d insercoes: altura = %d (nos 2/3 cheios)\n",
           N, bstar_altura(t));

    for (int i = 0; i < N; i += 2) {
        assert(bstar_remover(t, chaves[i]) == 0);
        if (i % 400 == 0)
            assert(bstar_valida(t));
    }
    assert(bstar_valida(t));
    assert(bstar_tamanho(t) == (size_t)(N / 2));

    contados = 0;
    ordenado = 1;
    bstar_em_ordem(t, checar, NULL);
    assert(ordenado && contados == bstar_tamanho(t));

    for (int i = 0; i < N; i += 2)
        assert(!bstar_contem(t, chaves[i]));
    for (int i = 1; i < N; i += 2)
        assert(bstar_contem(t, chaves[i]));

    printf("arvore B* ok: %zu chaves, altura %d\n",
           bstar_tamanho(t), bstar_altura(t));

    free(chaves);
    bstar_destruir(t);
    return 0;
}
