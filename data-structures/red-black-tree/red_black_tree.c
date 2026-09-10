#include "red_black_tree.h"

#include <limits.h>
#include <stdlib.h>

typedef enum { VERMELHO, PRETO } Cor;

typedef struct no {
    int chave;
    Cor cor;
    struct no *esq;
    struct no *dir;
    struct no *pai;
} No;

struct rb {
    No *raiz;
    No *nil;        /* sentinela: representa toda folha nula; é sempre PRETO */
    size_t tamanho;
};

ArvoreRB *rb_criar(void) {
    ArvoreRB *t = malloc(sizeof *t);
    if (t == NULL)
        return NULL;

    t->nil = malloc(sizeof *t->nil);
    if (t->nil == NULL) {
        free(t);
        return NULL;
    }
    t->nil->cor = PRETO;
    t->nil->esq = t->nil->dir = t->nil->pai = t->nil;
    t->nil->chave = 0;

    t->raiz = t->nil;
    t->tamanho = 0;
    return t;
}

static void destruir_no(ArvoreRB *t, No *n) {
    if (n == t->nil)
        return;
    destruir_no(t, n->esq);
    destruir_no(t, n->dir);
    free(n);
}

void rb_destruir(ArvoreRB *t) {
    if (t == NULL)
        return;
    destruir_no(t, t->raiz);
    free(t->nil);
    free(t);
}

size_t rb_tamanho(const ArvoreRB *t) {
    return t->tamanho;
}

static void rotacionar_esq(ArvoreRB *t, No *x) {
    No *y = x->dir;
    x->dir = y->esq;
    if (y->esq != t->nil)
        y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == t->nil)
        t->raiz = y;
    else if (x == x->pai->esq)
        x->pai->esq = y;
    else
        x->pai->dir = y;
    y->esq = x;
    x->pai = y;
}

static void rotacionar_dir(ArvoreRB *t, No *x) {
    No *y = x->esq;
    x->esq = y->dir;
    if (y->dir != t->nil)
        y->dir->pai = x;
    y->pai = x->pai;
    if (x->pai == t->nil)
        t->raiz = y;
    else if (x == x->pai->dir)
        x->pai->dir = y;
    else
        x->pai->esq = y;
    y->dir = x;
    x->pai = y;
}

static void inserir_fixup(ArvoreRB *t, No *z) {
    while (z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esq) {
            No *tio = z->pai->pai->dir;
            if (tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    rotacionar_esq(t, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionar_dir(t, z->pai->pai);
            }
        } else {
            No *tio = z->pai->pai->esq;
            if (tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rotacionar_dir(t, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionar_esq(t, z->pai->pai);
            }
        }
    }
    t->raiz->cor = PRETO;
}

int rb_inserir(ArvoreRB *t, int chave) {
    No *pai = t->nil;
    No *atual = t->raiz;
    while (atual != t->nil) {
        pai = atual;
        if (chave == atual->chave)
            return 1;
        atual = chave < atual->chave ? atual->esq : atual->dir;
    }

    No *z = malloc(sizeof *z);
    if (z == NULL)
        return -1;
    z->chave = chave;
    z->cor = VERMELHO;
    z->esq = z->dir = t->nil;
    z->pai = pai;

    if (pai == t->nil)
        t->raiz = z;
    else if (chave < pai->chave)
        pai->esq = z;
    else
        pai->dir = z;

    inserir_fixup(t, z);
    t->tamanho++;
    return 0;
}

static No *minimo(ArvoreRB *t, No *n) {
    while (n->esq != t->nil)
        n = n->esq;
    return n;
}

static No *buscar(const ArvoreRB *t, int chave) {
    No *n = t->raiz;
    while (n != t->nil && n->chave != chave)
        n = chave < n->chave ? n->esq : n->dir;
    return n;
}

/* Substitui a subárvore enraizada em u pela enraizada em v. */
static void transplantar(ArvoreRB *t, No *u, No *v) {
    if (u->pai == t->nil)
        t->raiz = v;
    else if (u == u->pai->esq)
        u->pai->esq = v;
    else
        u->pai->dir = v;
    v->pai = u->pai;
}

static void remover_fixup(ArvoreRB *t, No *x) {
    while (x != t->raiz && x->cor == PRETO) {
        if (x == x->pai->esq) {
            No *irmao = x->pai->dir;
            if (irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacionar_esq(t, x->pai);
                irmao = x->pai->dir;
            }
            if (irmao->esq->cor == PRETO && irmao->dir->cor == PRETO) {
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                if (irmao->dir->cor == PRETO) {
                    irmao->esq->cor = PRETO;
                    irmao->cor = VERMELHO;
                    rotacionar_dir(t, irmao);
                    irmao = x->pai->dir;
                }
                irmao->cor = x->pai->cor;
                x->pai->cor = PRETO;
                irmao->dir->cor = PRETO;
                rotacionar_esq(t, x->pai);
                x = t->raiz;
            }
        } else {
            No *irmao = x->pai->esq;
            if (irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacionar_dir(t, x->pai);
                irmao = x->pai->esq;
            }
            if (irmao->dir->cor == PRETO && irmao->esq->cor == PRETO) {
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                if (irmao->esq->cor == PRETO) {
                    irmao->dir->cor = PRETO;
                    irmao->cor = VERMELHO;
                    rotacionar_esq(t, irmao);
                    irmao = x->pai->esq;
                }
                irmao->cor = x->pai->cor;
                x->pai->cor = PRETO;
                irmao->esq->cor = PRETO;
                rotacionar_dir(t, x->pai);
                x = t->raiz;
            }
        }
    }
    x->cor = PRETO;
}

int rb_remover(ArvoreRB *t, int chave) {
    No *z = buscar(t, chave);
    if (z == t->nil)
        return -1;

    No *y = z;
    Cor cor_original = y->cor;
    No *x;

    if (z->esq == t->nil) {
        x = z->dir;
        transplantar(t, z, z->dir);
    } else if (z->dir == t->nil) {
        x = z->esq;
        transplantar(t, z, z->esq);
    } else {
        y = minimo(t, z->dir);
        cor_original = y->cor;
        x = y->dir;
        if (y->pai == z) {
            x->pai = y;             /* mesmo quando x é o nil, para o fixup */
        } else {
            transplantar(t, y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }
        transplantar(t, z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }

    free(z);
    t->tamanho--;

    if (cor_original == PRETO)
        remover_fixup(t, x);

    return 0;
}

int rb_contem(const ArvoreRB *t, int chave) {
    return buscar(t, chave) != t->nil;
}

static int altura_preta_no(const ArvoreRB *t, const No *n) {
    if (n == t->nil)
        return 1;
    int abaixo = altura_preta_no(t, n->esq);
    return abaixo + (n->cor == PRETO ? 1 : 0);
}

int rb_altura_preta(const ArvoreRB *t) {
    return altura_preta_no(t, t->raiz);
}

static void em_ordem_no(const ArvoreRB *t, const No *n,
                        void (*visita)(int, void *), void *ctx) {
    if (n == t->nil)
        return;
    em_ordem_no(t, n->esq, visita, ctx);
    visita(n->chave, ctx);
    em_ordem_no(t, n->dir, visita, ctx);
}

void rb_em_ordem(const ArvoreRB *t, void (*visita)(int, void *), void *ctx) {
    em_ordem_no(t, t->raiz, visita, ctx);
}

/* Devolve a altura preta da subárvore se ela é válida, ou -1 se não é. */
static int checar(const ArvoreRB *t, const No *n, int menor, int maior) {
    if (n == t->nil)
        return 1;

    if (n->chave <= menor || n->chave >= maior)
        return -1;                          /* fora da ordem de BST */

    if (n->cor == VERMELHO &&
        (n->esq->cor == VERMELHO || n->dir->cor == VERMELHO))
        return -1;                          /* vermelho com filho vermelho */

    int he = checar(t, n->esq, menor, n->chave);
    int hd = checar(t, n->dir, n->chave, maior);
    if (he == -1 || hd == -1 || he != hd)
        return -1;                          /* alturas pretas diferentes */

    return he + (n->cor == PRETO ? 1 : 0);
}

int rb_valida(const ArvoreRB *t) {
    if (t->raiz->cor != PRETO)
        return 0;
    return checar(t, t->raiz, INT_MIN, INT_MAX) != -1;
}
