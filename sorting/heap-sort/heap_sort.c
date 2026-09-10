#include "heap_sort.h"

static void troca(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/* Restaura a propriedade de max-heap em v[0..n) supondo que só v[i] pode estar
   fora de lugar: desce esse elemento trocando-o sempre com o maior filho. */
static void peneira(int *v, size_t n, size_t i) {
    for (;;) {
        size_t maior = i;
        size_t esq = 2 * i + 1;
        size_t dir = 2 * i + 2;

        if (esq < n && v[esq] > v[maior])
            maior = esq;
        if (dir < n && v[dir] > v[maior])
            maior = dir;

        if (maior == i)
            return;

        troca(&v[i], &v[maior]);
        i = maior;
    }
}

void heap_sort(int *v, size_t n) {
    if (n < 2)
        return;

    /* Constrói o heap de baixo para cima: os últimos n/2 nós são folhas. */
    for (size_t i = n / 2; i-- > 0; )
        peneira(v, n, i);

    /* Move o maior (raiz) para o fim e reduz o heap, repetindo. */
    for (size_t fim = n; fim-- > 1; ) {
        troca(&v[0], &v[fim]);
        peneira(v, fim, 0);
    }
}
