#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include <stddef.h>

/* Árvore B+ de chaves inteiras distintas.
   É uma árvore B em que:
     - os nós internos guardam apenas chaves de roteamento (separadores), não
       dados;
     - todas as chaves de verdade ficam nas folhas;
     - as folhas são ligadas numa lista encadeada, o que torna a varredura em
       ordem e as consultas por intervalo O(k) depois de achar o início, sem
       subir e descer na árvore.
   É o formato de índice mais comum em bancos de dados relacionais. */
typedef struct b_plus_tree ArvoreBMais;

ArvoreBMais *bplus_criar(void);
void         bplus_destruir(ArvoreBMais *t);

size_t bplus_tamanho(const ArvoreBMais *t);
int    bplus_altura(const ArvoreBMais *t);

/* 0 se inseriu, 1 se já existia, -1 em falha de alocação. */
int bplus_inserir(ArvoreBMais *t, int chave);
/* 0 em sucesso, -1 se a chave não existe. */
int bplus_remover(ArvoreBMais *t, int chave);
int bplus_contem(const ArvoreBMais *t, int chave);

/* Percorre todas as chaves em ordem crescente, caminhando pela lista de folhas. */
void bplus_em_ordem(const ArvoreBMais *t, void (*visita)(int chave, void *ctx), void *ctx);
/* Percorre as chaves no intervalo fechado [lo, hi], em ordem. */
void bplus_intervalo(const ArvoreBMais *t, int lo, int hi,
                     void (*visita)(int chave, void *ctx), void *ctx);

int bplus_valida(const ArvoreBMais *t);

#endif
