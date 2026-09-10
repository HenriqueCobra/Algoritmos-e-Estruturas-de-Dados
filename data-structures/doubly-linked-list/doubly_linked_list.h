#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stddef.h>

/* Lista duplamente encadeada de inteiros: cada nó aponta para o anterior e para
   o próximo. Inserir e remover em qualquer das pontas é O(1). */
typedef struct lista_dupla ListaDupla;

ListaDupla *lista_dupla_criar(void);
void        lista_dupla_destruir(ListaDupla *l);

size_t lista_dupla_tamanho(const ListaDupla *l);
int    lista_dupla_vazia(const ListaDupla *l);

/* Retornam 0 em sucesso, -1 em falha de alocação. */
int lista_dupla_inserir_inicio(ListaDupla *l, int valor);
int lista_dupla_inserir_fim(ListaDupla *l, int valor);
/* Insere antes de `pos` (0..tamanho). -1 se `pos` fora do intervalo ou alocação falhar. */
int lista_dupla_inserir_em(ListaDupla *l, size_t pos, int valor);

/* Removem de uma ponta e copiam o valor para `saida` (se não NULL).
   0 em sucesso, -1 se a lista estiver vazia. */
int lista_dupla_remover_inicio(ListaDupla *l, int *saida);
int lista_dupla_remover_fim(ListaDupla *l, int *saida);
/* Remove a primeira ocorrência de `valor`. 0 em sucesso, -1 se não existe. */
int lista_dupla_remover_valor(ListaDupla *l, int valor);

int lista_dupla_obter(const ListaDupla *l, size_t pos, int *saida);
int lista_dupla_buscar(const ListaDupla *l, int valor, size_t *pos);

/* Copia os valores para `destino`: em ordem, ou de trás para frente. */
void lista_dupla_para_vetor(const ListaDupla *l, int *destino);
void lista_dupla_para_vetor_reverso(const ListaDupla *l, int *destino);

#endif
