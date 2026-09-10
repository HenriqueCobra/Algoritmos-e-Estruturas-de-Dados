#ifndef TRIE_H
#define TRIE_H

#include <stddef.h>

/* Trie (árvore de prefixos) para palavras formadas por letras minúsculas de
   'a' a 'z'. Cada nó tem até 26 filhos; o caminho da raiz até um nó soletra um
   prefixo, e alguns nós são marcados como "fim de palavra".

   Buscar, inserir ou remover uma palavra de comprimento m custa O(m),
   independentemente de quantas palavras a trie contém. */
typedef struct trie Trie;

Trie *trie_criar(void);
void  trie_destruir(Trie *t);

size_t trie_tamanho(const Trie *t);   /* número de palavras */

/* Insere `palavra`. 0 se inseriu, 1 se já existia, -1 em falha de alocação ou
   se a palavra tem algum caractere fora de 'a'..'z' (ou é vazia). */
int trie_inserir(Trie *t, const char *palavra);

/* 1 se a palavra exata está na trie. */
int trie_contem(const Trie *t, const char *palavra);
/* 1 se alguma palavra da trie começa com `prefixo` (prefixo vazio => 1 se
   houver qualquer palavra). */
int trie_tem_prefixo(const Trie *t, const char *prefixo);

/* Remove `palavra`. 0 em sucesso, -1 se não existe. Libera os nós que deixaram
   de ser necessários. */
int trie_remover(Trie *t, const char *palavra);

#endif
