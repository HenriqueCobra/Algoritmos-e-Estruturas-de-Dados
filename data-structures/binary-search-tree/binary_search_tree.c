#include "binary_search_tree.h"

#include <stdlib.h>

typedef struct no {
    int chave;
    struct no *esq;
    struct no *dir;
} No;

struct bst {
    No *raiz;
    size_t tamanho;
};

BST *bst_criar(void) {
    BST *t = malloc(sizeof *t);
    if (t != NULL) {
        t->raiz = NULL;
        t->tamanho = 0;
    }
    return t;
}

static void destruir_no(No *n) {
    if (n == NULL)
        return;
    destruir_no(n->esq);
    destruir_no(n->dir);
    free(n);
}

void bst_destruir(BST *t) {
    if (t == NULL)
        return;
    destruir_no(t->raiz);
    free(t);
}

size_t bst_tamanho(const BST *t) {
    return t->tamanho;
}

static int altura_no(const No *n) {
    if (n == NULL)
        return -1;
    int he = altura_no(n->esq);
    int hd = altura_no(n->dir);
    return 1 + (he > hd ? he : hd);
}

int bst_altura(const BST *t) {
    return altura_no(t->raiz);
}

/* Insere `chave` na subárvore `n`. `*inseriu` recebe 1 se um nó novo foi criado.
   Devolve a nova raiz da subárvore (ou a mesma, se nada mudou). */
static No *inserir_no(No *n, int chave, int *inseriu) {
    if (n == NULL) {
        No *novo = malloc(sizeof *novo);
        if (novo == NULL) {
            *inseriu = -1;
            return NULL;
        }
        novo->chave = chave;
        novo->esq = NULL;
        novo->dir = NULL;
        *inseriu = 1;
        return novo;
    }

    if (chave < n->chave)
        n->esq = inserir_no(n->esq, chave, inseriu);
    else if (chave > n->chave)
        n->dir = inserir_no(n->dir, chave, inseriu);
    else
        *inseriu = 0; /* já existe */

    return n;
}

int bst_inserir(BST *t, int chave) {
    int inseriu = 0;
    No *nova_raiz = inserir_no(t->raiz, chave, &inseriu);

    if (inseriu == -1)
        return -1;

    t->raiz = nova_raiz;
    if (inseriu == 1)
        t->tamanho++;
    return inseriu == 1 ? 0 : 1;
}

int bst_contem(const BST *t, int chave) {
    const No *n = t->raiz;
    while (n != NULL) {
        if (chave == n->chave)
            return 1;
        n = chave < n->chave ? n->esq : n->dir;
    }
    return 0;
}

static No *min_no(No *n) {
    while (n->esq != NULL)
        n = n->esq;
    return n;
}

/* Remove `chave` da subárvore `n`. `*removeu` recebe 1 se algo foi removido. */
static No *remover_no(No *n, int chave, int *removeu) {
    if (n == NULL) {
        *removeu = 0;
        return NULL;
    }

    if (chave < n->chave) {
        n->esq = remover_no(n->esq, chave, removeu);
        return n;
    }
    if (chave > n->chave) {
        n->dir = remover_no(n->dir, chave, removeu);
        return n;
    }

    /* achou o nó a remover */
    *removeu = 1;

    if (n->esq == NULL) {
        No *filho = n->dir;
        free(n);
        return filho;
    }
    if (n->dir == NULL) {
        No *filho = n->esq;
        free(n);
        return filho;
    }

    /* dois filhos: copia o sucessor em ordem (menor da subárvore direita) e
       remove esse sucessor, que tem no máximo um filho */
    No *suc = min_no(n->dir);
    n->chave = suc->chave;
    int ignorado;
    n->dir = remover_no(n->dir, suc->chave, &ignorado);
    return n;
}

int bst_remover(BST *t, int chave) {
    int removeu = 0;
    t->raiz = remover_no(t->raiz, chave, &removeu);
    if (removeu)
        t->tamanho--;
    return removeu ? 0 : -1;
}

int bst_minimo(const BST *t, int *saida) {
    if (t->raiz == NULL)
        return -1;
    const No *n = t->raiz;
    while (n->esq != NULL)
        n = n->esq;
    if (saida != NULL)
        *saida = n->chave;
    return 0;
}

int bst_maximo(const BST *t, int *saida) {
    if (t->raiz == NULL)
        return -1;
    const No *n = t->raiz;
    while (n->dir != NULL)
        n = n->dir;
    if (saida != NULL)
        *saida = n->chave;
    return 0;
}

static void em_ordem_no(const No *n, void (*visita)(int, void *), void *ctx) {
    if (n == NULL)
        return;
    em_ordem_no(n->esq, visita, ctx);
    visita(n->chave, ctx);
    em_ordem_no(n->dir, visita, ctx);
}

void bst_em_ordem(const BST *t, void (*visita)(int, void *), void *ctx) {
    em_ordem_no(t->raiz, visita, ctx);
}
