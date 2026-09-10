#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "red_black_tree.h"

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
    ArvoreRB *t = rb_criar();
    assert(t != NULL);

    /* insere 0..4999 embaralhado */
    const int N = 5000;
    int *chaves = malloc(N * sizeof *chaves);
    for (int i = 0; i < N; i++)
        chaves[i] = i;
    for (int i = N - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = chaves[i]; chaves[i] = chaves[j]; chaves[j] = tmp;
    }

    for (int i = 0; i < N; i++)
        assert(rb_inserir(t, chaves[i]) == 0);
    assert(rb_tamanho(t) == (size_t)N);
    assert(rb_valida(t));
    printf("apos %d insercoes: altura preta = %d, valida = %d\n",
           N, rb_altura_preta(t), rb_valida(t));

    /* remove ~metade, revalidando de vez em quando */
    for (int i = 0; i < N; i += 2) {
        assert(rb_remover(t, chaves[i]) == 0);
        if (i % 500 == 0)
            assert(rb_valida(t));
    }
    assert(rb_valida(t));
    assert(rb_tamanho(t) == (size_t)(N / 2));

    contados = 0;
    ordenado = 1;
    rb_em_ordem(t, checar, NULL);
    assert(ordenado && contados == (size_t)(N / 2));

    assert(rb_remover(t, chaves[0]) == -1);   /* já removido */

    free(chaves);
    rb_destruir(t);
    printf("rubro-negra ok\n");
    return 0;
}
