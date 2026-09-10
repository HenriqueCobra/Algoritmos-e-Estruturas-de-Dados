#ifndef B_STAR_TREE_H
#define B_STAR_TREE_H

#include <stddef.h>

/* Árvore B* de chaves inteiras distintas.
   É uma árvore B em que os nós (fora a raiz) são mantidos pelo menos 2/3 cheios,
   em vez de 1/2. O ganho é uma árvore mais compacta (menos nós, menos níveis);
   o custo é uma manutenção mais elaborada:

     - na inserção, quando um nó transborda, primeiro tenta-se **redistribuir**
       chaves com um irmão; só quando o nó e o irmão estão os dois cheios é que
       ocorre uma divisão **2-para-3** (dois nós cheios viram três nós 2/3
       cheios);
     - na remoção, de forma simétrica, três nós pouco cheios podem ser fundidos
       em dois (**3-para-2**).

   Busca, inserção e remoção continuam O(log n). */
typedef struct b_star_tree ArvoreBStar;

ArvoreBStar *bstar_criar(void);
void         bstar_destruir(ArvoreBStar *t);

size_t bstar_tamanho(const ArvoreBStar *t);
int    bstar_altura(const ArvoreBStar *t);

/* 0 se inseriu, 1 se já existia, -1 em falha de alocação. */
int bstar_inserir(ArvoreBStar *t, int chave);
/* 0 em sucesso, -1 se a chave não existe. */
int bstar_remover(ArvoreBStar *t, int chave);
int bstar_contem(const ArvoreBStar *t, int chave);

void bstar_em_ordem(const ArvoreBStar *t, void (*visita)(int chave, void *ctx), void *ctx);

int bstar_valida(const ArvoreBStar *t);

#endif
