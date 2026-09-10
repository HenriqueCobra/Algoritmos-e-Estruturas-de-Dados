#include "trie.h"

#include <stdbool.h>
#include <stdlib.h>

#define ALFABETO 26

typedef struct no {
    struct no *filhos[ALFABETO];
    bool fim_de_palavra;
    int n_filhos;
} No;

struct trie {
    No *raiz;
    size_t tamanho;
};

static No *no_criar(void) {
    return calloc(1, sizeof(No));   /* zera filhos, fim_de_palavra e n_filhos */
}

Trie *trie_criar(void) {
    Trie *t = malloc(sizeof *t);
    if (t == NULL)
        return NULL;
    t->raiz = no_criar();
    if (t->raiz == NULL) {
        free(t);
        return NULL;
    }
    t->tamanho = 0;
    return t;
}

static void destruir_no(No *n) {
    if (n == NULL)
        return;
    for (int i = 0; i < ALFABETO; i++)
        destruir_no(n->filhos[i]);
    free(n);
}

void trie_destruir(Trie *t) {
    if (t == NULL)
        return;
    destruir_no(t->raiz);
    free(t);
}

size_t trie_tamanho(const Trie *t) {
    return t->tamanho;
}

/* Converte a palavra em índices 0..25. Retorna 0 se algum caractere é inválido
   ou se a palavra é vazia. */
static int valida(const char *palavra) {
    if (palavra[0] == '\0')
        return 0;
    for (size_t i = 0; palavra[i] != '\0'; i++)
        if (palavra[i] < 'a' || palavra[i] > 'z')
            return 0;
    return 1;
}

int trie_inserir(Trie *t, const char *palavra) {
    if (!valida(palavra))
        return -1;

    No *n = t->raiz;
    for (size_t i = 0; palavra[i] != '\0'; i++) {
        int c = palavra[i] - 'a';
        if (n->filhos[c] == NULL) {
            No *novo = no_criar();
            if (novo == NULL)
                return -1;
            n->filhos[c] = novo;
            n->n_filhos++;
        }
        n = n->filhos[c];
    }

    if (n->fim_de_palavra)
        return 1;
    n->fim_de_palavra = true;
    t->tamanho++;
    return 0;
}

/* Desce pelo caminho de `s`; devolve o nó final ou NULL se o caminho não existe. */
static const No *descer(const No *n, const char *s) {
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < 'a' || s[i] > 'z')
            return NULL;
        n = n->filhos[s[i] - 'a'];
        if (n == NULL)
            return NULL;
    }
    return n;
}

int trie_contem(const Trie *t, const char *palavra) {
    const No *n = descer(t->raiz, palavra);
    return n != NULL && n->fim_de_palavra;
}

int trie_tem_prefixo(const Trie *t, const char *prefixo) {
    return descer(t->raiz, prefixo) != NULL;
}

/* Remoção recursiva: desmarca o fim da palavra e, ao voltar, libera cada nó
   intermediário que ficou sem filhos e não é fim de outra palavra. O parâmetro
   `profundidade` existe para nunca liberar a raiz (profundidade 0). */
static No *remover_rec(No *n, const char *palavra, size_t profundidade,
                       int *removeu) {
    if (n == NULL) {
        *removeu = 0;
        return NULL;
    }

    if (palavra[profundidade] == '\0') {
        if (!n->fim_de_palavra) {
            *removeu = 0;
            return n;
        }
        *removeu = 1;
        n->fim_de_palavra = false;
    } else {
        int c = palavra[profundidade] - 'a';
        No *filho = remover_rec(n->filhos[c], palavra, profundidade + 1, removeu);
        if (filho == NULL && n->filhos[c] != NULL) {
            n->filhos[c] = NULL;
            n->n_filhos--;
        }
    }

    if (profundidade > 0 && !n->fim_de_palavra && n->n_filhos == 0) {
        free(n);
        return NULL;
    }
    return n;
}

int trie_remover(Trie *t, const char *palavra) {
    if (!valida(palavra))
        return -1;

    int removeu = 0;
    remover_rec(t->raiz, palavra, 0, &removeu);
    if (removeu)
        t->tamanho--;
    return removeu ? 0 : -1;
}
