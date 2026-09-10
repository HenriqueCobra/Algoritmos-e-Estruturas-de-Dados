#include <assert.h>
#include <stdio.h>

#include "union_find.h"

int main(void) {
    /* 10 elementos, todos separados */
    DSU *d = dsu_criar(10);
    assert(d != NULL);
    assert(dsu_num_conjuntos(d) == 10);

    /* forma dois grupos: {0,1,2,3} e {5,6,7} */
    assert(dsu_unir(d, 0, 1) == 1);
    assert(dsu_unir(d, 2, 3) == 1);
    assert(dsu_unir(d, 1, 3) == 1);
    assert(dsu_unir(d, 0, 2) == 0);        /* já conectados */

    dsu_unir(d, 5, 6);
    dsu_unir(d, 6, 7);

    assert(dsu_conectados(d, 0, 3));
    assert(!dsu_conectados(d, 3, 5));
    assert(dsu_tamanho_conjunto(d, 0) == 4);
    assert(dsu_tamanho_conjunto(d, 7) == 3);
    assert(dsu_num_conjuntos(d) == 5);     /* {0..3} {5..7} {4} {8} {9} */

    /* junta os dois grupos grandes */
    dsu_unir(d, 3, 5);
    assert(dsu_conectados(d, 1, 7));
    assert(dsu_tamanho_conjunto(d, 6) == 7);
    assert(dsu_num_conjuntos(d) == 4);

    printf("union-find ok: %zu conjuntos\n", dsu_num_conjuntos(d));

    dsu_destruir(d);
    return 0;
}
