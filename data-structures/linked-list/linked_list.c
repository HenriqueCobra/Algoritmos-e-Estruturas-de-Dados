#include "linked_list.h"

#include <stdlib.h>

typedef struct no {
    int valor;
    struct no *prox;
} No;

struct lista {
    No *inicio;
    No *fim;
    size_t tamanho;
};

Lista *lista_criar(void) {
    Lista *l = malloc(sizeof *l);
    if (l != NULL) {
        l->inicio = NULL;
        l->fim = NULL;
        l->tamanho = 0;
    }
    return l;
}

void lista_destruir(Lista *l) {
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

size_t lista_tamanho(const Lista *l) {
    return l->tamanho;
}

int lista_vazia(const Lista *l) {
    return l->tamanho == 0;
}

static No *no_criar(int valor) {
    No *n = malloc(sizeof *n);
    if (n != NULL) {
        n->valor = valor;
        n->prox = NULL;
    }
    return n;
}

void lista_inserir_inicio(Lista *l, int valor) {
    No *n = no_criar(valor);
    if (n == NULL)
        return;

    n->prox = l->inicio;
    l->inicio = n;
    if (l->fim == NULL)
        l->fim = n;
    l->tamanho++;
}

void lista_inserir_fim(Lista *l, int valor) {
    No *n = no_criar(valor);
    if (n == NULL)
        return;

    if (l->fim == NULL)
        l->inicio = n;
    else
        l->fim->prox = n;
    l->fim = n;
    l->tamanho++;
}

int lista_inserir_em(Lista *l, size_t pos, int valor) {
    if (pos > l->tamanho)
        return -1;
    if (pos == 0) {
        lista_inserir_inicio(l, valor);
        return 0;
    }
    if (pos == l->tamanho) {
        lista_inserir_fim(l, valor);
        return 0;
    }

    /* anterior aponta para o nó na posição pos-1 */
    No *anterior = l->inicio;
    for (size_t i = 0; i + 1 < pos; i++)
        anterior = anterior->prox;

    No *n = no_criar(valor);
    if (n == NULL)
        return -1;

    n->prox = anterior->prox;
    anterior->prox = n;
    l->tamanho++;
    return 0;
}

int lista_remover_inicio(Lista *l, int *saida) {
    if (l->inicio == NULL)
        return -1;

    No *removido = l->inicio;
    if (saida != NULL)
        *saida = removido->valor;

    l->inicio = removido->prox;
    if (l->inicio == NULL)
        l->fim = NULL;
    free(removido);
    l->tamanho--;
    return 0;
}

int lista_remover_fim(Lista *l, int *saida) {
    if (l->fim == NULL)
        return -1;

    if (l->inicio == l->fim)
        return lista_remover_inicio(l, saida);

    No *anterior = l->inicio;
    while (anterior->prox != l->fim)
        anterior = anterior->prox;

    if (saida != NULL)
        *saida = l->fim->valor;

    free(l->fim);
    anterior->prox = NULL;
    l->fim = anterior;
    l->tamanho--;
    return 0;
}

int lista_remover_valor(Lista *l, int valor) {
    No *anterior = NULL;
    No *atual = l->inicio;

    while (atual != NULL && atual->valor != valor) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL)
        return -1;

    if (anterior == NULL)
        return lista_remover_inicio(l, NULL);

    anterior->prox = atual->prox;
    if (atual == l->fim)
        l->fim = anterior;
    free(atual);
    l->tamanho--;
    return 0;
}

int lista_buscar(const Lista *l, int valor, size_t *pos) {
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

int lista_obter(const Lista *l, size_t pos, int *saida) {
    if (pos >= l->tamanho)
        return -1;

    const No *atual = l->inicio;
    for (size_t i = 0; i < pos; i++)
        atual = atual->prox;

    if (saida != NULL)
        *saida = atual->valor;
    return 0;
}

void lista_inverter(Lista *l) {
    No *anterior = NULL;
    No *atual = l->inicio;

    l->fim = l->inicio;
    while (atual != NULL) {
        No *seguinte = atual->prox;
        atual->prox = anterior;
        anterior = atual;
        atual = seguinte;
    }
    l->inicio = anterior;
}

void lista_para_vetor(const Lista *l, int *destino) {
    size_t i = 0;
    for (const No *atual = l->inicio; atual != NULL; atual = atual->prox)
        destino[i++] = atual->valor;
}
