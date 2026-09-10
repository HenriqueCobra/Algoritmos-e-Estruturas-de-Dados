#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

/* Lista simplesmente encadeada de inteiros.
   Guarda ponteiros para o primeiro e o último nó, então inserir no início ou
   no fim é O(1); acesso por posição é O(n). */
typedef struct lista Lista;

Lista *lista_criar(void);
void   lista_destruir(Lista *l);

size_t lista_tamanho(const Lista *l);
int    lista_vazia(const Lista *l);

void lista_inserir_inicio(Lista *l, int valor);
void lista_inserir_fim(Lista *l, int valor);
/* Insere antes da posição `pos` (0 = início, tamanho = fim).
   Retorna 0 em sucesso ou -1 se `pos` estiver fora do intervalo. */
int  lista_inserir_em(Lista *l, size_t pos, int valor);

/* Removem um elemento e, se `saida` não for NULL, copiam o valor removido.
   Retornam 0 em sucesso ou -1 se não havia o que remover. */
int lista_remover_inicio(Lista *l, int *saida);
int lista_remover_fim(Lista *l, int *saida);
/* Remove a primeira ocorrência de `valor`. 0 em sucesso, -1 se não existe. */
int lista_remover_valor(Lista *l, int valor);

/* Procura a primeira ocorrência de `valor`. Se achar, grava o índice em `pos`
   (quando não for NULL) e retorna 0; senão retorna -1. */
int lista_buscar(const Lista *l, int valor, size_t *pos);
/* Lê o valor na posição `pos`. 0 em sucesso, -1 se fora do intervalo. */
int lista_obter(const Lista *l, size_t pos, int *saida);

void lista_inverter(Lista *l);
/* Copia os valores, em ordem, para `destino` (que deve caber lista_tamanho). */
void lista_para_vetor(const Lista *l, int *destino);

#endif
