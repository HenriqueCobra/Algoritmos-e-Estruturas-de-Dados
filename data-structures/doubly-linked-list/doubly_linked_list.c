#include "doubly_linked_list.h"

#include <stdlib.h>

typedef struct no {
    int valor;
    struct no *ant;
    struct no *prox;
} No;

struct lista_dupla {
    No *inicio;
    No *fim;
    size_t tamanho;
};

ListaDupla *lista_dupla_criar(void) {
    ListaDupla *l = malloc(sizeof *l);
    if (l != NULL) {
        l->inicio = NULL;
        l->fim = NULL;
        l->tamanho = 0;
    }
    return l;
}

void lista_dupla_destruir(ListaDupla *l) {
    if (l == NULL)
        return;

    No *atual = l->inicio;
    while (atual != NULL) {
        No *seguinte = atual->prox;
        free(atual);
        atual = seguinte;
    }
    free(l);
}

size_t lista_dupla_tamanho(const ListaDupla *l) {
    return l->tamanho;
}

int lista_dupla_vazia(const ListaDupla *l) {
    return l->tamanho == 0;
}

static No *no_criar(int valor) {
    No *n = malloc(sizeof *n);
    if (n != NULL) {
        n->valor = valor;
        n->ant = NULL;
        n->prox = NULL;
    }
    return n;
}

int lista_dupla_inserir_inicio(ListaDupla *l, int valor) {
    No *n = no_criar(valor);
    if (n == NULL)
        return -1;

    n->prox = l->inicio;
    if (l->inicio != NULL)
        l->inicio->ant = n;
    else
        l->fim = n;
    l->inicio = n;
    l->tamanho++;
    return 0;
}

int lista_dupla_inserir_fim(ListaDupla *l, int valor) {
    No *n = no_criar(valor);
    if (n == NULL)
        return -1;

    n->ant = l->fim;
    if (l->fim != NULL)
        l->fim->prox = n;
    else
        l->inicio = n;
    l->fim = n;
    l->tamanho++;
    return 0;
}

/* Devolve o nó na posição `pos` (0-based), procurando pela ponta mais perto. */
static No *no_em(const ListaDupla *l, size_t pos) {
    if (pos < l->tamanho / 2) {
        No *atual = l->inicio;
        for (size_t i = 0; i < pos; i++)
            atual = atual->prox;
        return atual;
    }
    No *atual = l->fim;
    for (size_t i = l->tamanho - 1; i > pos; i--)
        atual = atual->ant;
    return atual;
}

int lista_dupla_inserir_em(ListaDupla *l, size_t pos, int valor) {
    if (pos > l->tamanho)
        return -1;
    if (pos == 0)
        return lista_dupla_inserir_inicio(l, valor);
    if (pos == l->tamanho)
        return lista_dupla_inserir_fim(l, valor);

    No *seguinte = no_em(l, pos);
    No *anterior = seguinte->ant;

    No *n = no_criar(valor);
    if (n == NULL)
        return -1;

    n->ant = anterior;
    n->prox = seguinte;
    anterior->prox = n;
    seguinte->ant = n;
    l->tamanho++;
    return 0;
}

/* Desliga `n` da lista, libera o nó e decrementa o tamanho. */
static void desligar(ListaDupla *l, No *n) {
    if (n->ant != NULL)
        n->ant->prox = n->prox;
    else
        l->inicio = n->prox;

    if (n->prox != NULL)
        n->prox->ant = n->ant;
    else
        l->fim = n->ant;

    free(n);
    l->tamanho--;
}

int lista_dupla_remover_inicio(ListaDupla *l, int *saida) {
    if (l->inicio == NULL)
        return -1;
    if (saida != NULL)
        *saida = l->inicio->valor;
    desligar(l, l->inicio);
    return 0;
}

int lista_dupla_remover_fim(ListaDupla *l, int *saida) {
    if (l->fim == NULL)
        return -1;
    if (saida != NULL)
        *saida = l->fim->valor;
    desligar(l, l->fim);
    return 0;
}

int lista_dupla_remover_valor(ListaDupla *l, int valor) {
    for (No *atual = l->inicio; atual != NULL; atual = atual->prox) {
        if (atual->valor == valor) {
            desligar(l, atual);
            return 0;
        }
    }
    return -1;
}

int lista_dupla_obter(const ListaDupla *l, size_t pos, int *saida) {
    if (pos >= l->tamanho)
        return -1;
    if (saida != NULL)
        *saida = no_em(l, pos)->valor;
    return 0;
}

int lista_dupla_buscar(const ListaDupla *l, int valor, size_t *pos) {
    size_t i = 0;
    for (const No *atual = l->inicio; atual != NULL; atual = atual->prox, i++) {
        if (atual->valor == valor) {
            if (pos != NULL)
                *pos = i;
            return 0;
        }
    }
    return -1;
}

void lista_dupla_para_vetor(const ListaDupla *l, int *destino) {
    size_t i = 0;
    for (const No *atual = l->inicio; atual != NULL; atual = atual->prox)
        destino[i++] = atual->valor;
}

void lista_dupla_para_vetor_reverso(const ListaDupla *l, int *destino) {
    size_t i = 0;
    for (const No *atual = l->fim; atual != NULL; atual = atual->ant)
        destino[i++] = atual->valor;
}
