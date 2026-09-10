#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

/* Tabela hash (dicionário) de chave string para valor inteiro.
   Colisões são resolvidas por encadeamento separado: cada balde é uma lista
   ligada de pares. A tabela cresce e re-espalha quando o fator de carga passa
   de ~0,75, mantendo as operações O(1) em média. */
typedef struct tabela_hash TabelaHash;

TabelaHash *tabela_criar(void);
void        tabela_destruir(TabelaHash *t);

/* Número de pares armazenados. */
size_t tabela_tamanho(const TabelaHash *t);

/* Insere o par ou, se a chave já existe, atualiza o valor.
   A chave é copiada internamente. 0 em sucesso, -1 em falha de alocação. */
int tabela_por(TabelaHash *t, const char *chave, int valor);

/* Se a chave existe, grava o valor em `saida` (quando não NULL) e retorna 0.
   Caso contrário retorna -1. */
int tabela_obter(const TabelaHash *t, const char *chave, int *saida);

/* Remove a chave. 0 em sucesso, -1 se a chave não existe. */
int tabela_remover(TabelaHash *t, const char *chave);

/* 1 se a chave está na tabela, 0 caso contrário. */
int tabela_contem(const TabelaHash *t, const char *chave);

#endif
