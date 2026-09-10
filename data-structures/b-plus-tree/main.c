#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "b_plus_tree.h"

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

static void soma(int chave, void *ctx) {
    *(long *)ctx += chave;
}

int main(void) {
    ArvoreBMais *t = bplus_criar();
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
        assert(bplus_inserir(t, chaves[i]) == 0);
    assert(bplus_inserir(t, chaves[0]) == 1);
    assert(bplus_tamanho(t) == (size_t)N);
    assert(bplus_valida(t));
    printf("apos %d insercoes: altura = %d\n", N, bplus_altura(t));

    /* consulta por intervalo: soma de 1000..1099 deve ser 104950 */
    long s = 0;
    bplus_intervalo(t, 1000, 1099, soma, &s);
    assert(s == 104950);

    for (int i = 0; i < N; i += 2) {
        assert(bplus_remover(t, chaves[i]) == 0);
        if (i % 400 == 0)
            assert(bplus_valida(t));
    }
    assert(bplus_valida(t));
    assert(bplus_tamanho(t) == (size_t)(N / 2));

    contados = 0;
    ordenado = 1;
    bplus_em_ordem(t, checar, NULL);
    assert(ordenado && contados == bplus_tamanho(t));

    for (int i = 0; i < N; i += 2)
        assert(!bplus_contem(t, chaves[i]));
    for (int i = 1; i < N; i += 2)
        assert(bplus_contem(t, chaves[i]));

    printf("arvore B+ ok: %zu chaves, altura %d, folhas encadeadas\n",
           bplus_tamanho(t), bplus_altura(t));

    free(chaves);
    bplus_destruir(t);
    return 0;
}
