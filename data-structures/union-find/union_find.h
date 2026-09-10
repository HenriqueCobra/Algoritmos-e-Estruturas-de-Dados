#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <stddef.h>

/* Union-Find / DSU (disjoint set union): mantém uma partição de {0, 1, ..., n-1}
   em conjuntos disjuntos e responde rápido a "a e b estão no mesmo conjunto?"
   e "una os conjuntos de a e b". É a estrutura por trás do Kruskal e de
   detecção de ciclos em grafos.

   Com compressão de caminho + união por tamanho, cada operação custa
   O(alfa(n)) amortizado — na prática, constante. */
typedef struct dsu DSU;

/* Cria a estrutura com `n` elementos, cada um no seu próprio conjunto. */
DSU *dsu_criar(size_t n);
void dsu_destruir(DSU *d);

/* Representante (raiz) do conjunto de `x`. Elementos com o mesmo representante
   estão no mesmo conjunto. */
size_t dsu_encontrar(DSU *d, size_t x);

/* Une os conjuntos de `a` e `b`. Retorna 1 se eram conjuntos diferentes (houve
   união), 0 se já estavam juntos. */
int dsu_unir(DSU *d, size_t a, size_t b);

int dsu_conectados(DSU *d, size_t a, size_t b);

/* Quantidade de conjuntos disjuntos no momento. */
size_t dsu_num_conjuntos(const DSU *d);
/* Tamanho do conjunto que contém `x`. */
size_t dsu_tamanho_conjunto(DSU *d, size_t x);

#endif
