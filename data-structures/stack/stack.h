#ifndef STACK_H
#define STACK_H

#include <stddef.h>

/* Pilha (LIFO) de inteiros sobre um vetor dinâmico que dobra de capacidade
   quando enche. O último a entrar é o primeiro a sair. */
typedef struct pilha Pilha;

Pilha *pilha_criar(void);
void   pilha_destruir(Pilha *p);

size_t pilha_tamanho(const Pilha *p);
int    pilha_vazia(const Pilha *p);

/* Empilha `valor`. Retorna 0 em sucesso ou -1 se a realocação falhar. */
int pilha_empilhar(Pilha *p, int valor);
/* Desempilha o topo para `saida` (se não NULL). 0 em sucesso, -1 se vazia. */
int pilha_desempilhar(Pilha *p, int *saida);
/* Lê o topo sem remover. 0 em sucesso, -1 se vazia. */
int pilha_topo(const Pilha *p, int *saida);

#endif
