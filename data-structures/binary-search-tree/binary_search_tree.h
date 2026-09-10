#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stddef.h>

/* Árvore binária de busca (sem balanceamento) de chaves inteiras distintas.
   Para cada nó, tudo à esquerda é menor e tudo à direita é maior. As operações
   são O(altura): O(log n) se a árvore está equilibrada, O(n) no pior caso
   (inserções em ordem crescente viram uma lista). */
typedef struct bst BST;

BST *bst_criar(void);
void bst_destruir(BST *t);

size_t bst_tamanho(const BST *t);
/* Altura em número de arestas; -1 para a árvore vazia. */
int bst_altura(const BST *t);

/* Insere a chave. 0 se inseriu, 1 se já existia, -1 em falha de alocação. */
int bst_inserir(BST *t, int chave);
/* 1 se a chave está na árvore, 0 caso contrário. */
int bst_contem(const BST *t, int chave);
/* Remove a chave. 0 em sucesso, -1 se não existe. */
int bst_remover(BST *t, int chave);

/* Menor / maior chave. 0 em sucesso, -1 se a árvore está vazia. */
int bst_minimo(const BST *t, int *saida);
int bst_maximo(const BST *t, int *saida);

/* Percorre em ordem crescente, chamando `visita(chave, ctx)` em cada nó. */
void bst_em_ordem(const BST *t, void (*visita)(int chave, void *ctx), void *ctx);

#endif
