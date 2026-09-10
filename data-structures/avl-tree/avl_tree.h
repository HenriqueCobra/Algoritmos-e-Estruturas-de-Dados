#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stddef.h>

/* Árvore AVL: árvore binária de busca que se rebalanceia a cada inserção e
   remoção para que, em todo nó, as alturas das duas subárvores diferam em no
   máximo 1. Isso mantém a altura em O(log n) e, portanto, todas as buscas,
   inserções e remoções em O(log n) no pior caso. */
typedef struct avl AVL;

AVL *avl_criar(void);
void avl_destruir(AVL *t);

size_t avl_tamanho(const AVL *t);
int    avl_altura(const AVL *t);   /* arestas; -1 se vazia */

/* 0 se inseriu, 1 se a chave já existia, -1 em falha de alocação. */
int avl_inserir(AVL *t, int chave);
/* 0 em sucesso, -1 se a chave não existe. */
int avl_remover(AVL *t, int chave);
int avl_contem(const AVL *t, int chave);

void avl_em_ordem(const AVL *t, void (*visita)(int chave, void *ctx), void *ctx);

#endif
