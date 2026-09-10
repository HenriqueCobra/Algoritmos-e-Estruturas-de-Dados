#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stddef.h>

/* Árvore rubro-negra: BST balanceada em que cada nó é vermelho ou preto e
   quatro regras garantem que o caminho mais longo da raiz a uma folha tem no
   máximo o dobro do mais curto. Resultado: altura O(log n) e busca, inserção e
   remoção O(log n) no pior caso, com menos rotações que a AVL.

   É a estrutura usada em std::map (C++), TreeMap (Java) e no escalonador do
   kernel Linux. */
typedef struct rb ArvoreRB;

ArvoreRB *rb_criar(void);
void      rb_destruir(ArvoreRB *t);

size_t rb_tamanho(const ArvoreRB *t);
int    rb_altura_preta(const ArvoreRB *t);   /* nº de nós pretos num caminho raiz->folha */

/* 0 se inseriu, 1 se a chave já existia, -1 em falha de alocação. */
int rb_inserir(ArvoreRB *t, int chave);
/* 0 em sucesso, -1 se a chave não existe. */
int rb_remover(ArvoreRB *t, int chave);
int rb_contem(const ArvoreRB *t, int chave);

void rb_em_ordem(const ArvoreRB *t, void (*visita)(int chave, void *ctx), void *ctx);

/* Verifica todas as invariantes rubro-negras (raiz preta, sem vermelho-vermelho,
   mesma altura preta em todo caminho, ordem de BST). Retorna 1 se a árvore está
   consistente. Útil em testes. */
int rb_valida(const ArvoreRB *t);

#endif
