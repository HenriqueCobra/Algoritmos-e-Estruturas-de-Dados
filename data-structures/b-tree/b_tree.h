#ifndef B_TREE_H
#define B_TREE_H

#include <stddef.h>

/* Árvore B de chaves inteiras distintas.
   Diferente das árvores binárias, cada nó guarda várias chaves e tem muitos
   filhos, o que reduz a altura e o número de acessos — é a estrutura de índices
   de bancos de dados e sistemas de arquivos, onde cada nó é um bloco de disco.

   Parâmetro: grau mínimo t (definido em b_tree.c). Todo nó, exceto a raiz, tem
   entre t-1 e 2t-1 chaves; a raiz tem entre 1 e 2t-1 (ou 0, se a árvore está
   vazia). Todas as folhas ficam na mesma profundidade.

   Busca, inserção e remoção são O(t · log_t n) comparações, O(log_t n) nós
   visitados. */
typedef struct b_tree ArvoreB;

ArvoreB *btree_criar(void);
void     btree_destruir(ArvoreB *t);

size_t btree_tamanho(const ArvoreB *t);
int    btree_altura(const ArvoreB *t);   /* nº de níveis; 0 se vazia */

/* 0 se inseriu, 1 se a chave já existia, -1 em falha de alocação. */
int btree_inserir(ArvoreB *t, int chave);
/* 0 em sucesso, -1 se a chave não existe. */
int btree_remover(ArvoreB *t, int chave);
int btree_contem(const ArvoreB *t, int chave);

void btree_em_ordem(const ArvoreB *t, void (*visita)(int chave, void *ctx), void *ctx);

/* Confere as invariantes (limites de chaves por nó, ordem, folhas no mesmo
   nível). Retorna 1 se a árvore está consistente. Usado nos testes. */
int btree_valida(const ArvoreB *t);

#endif
