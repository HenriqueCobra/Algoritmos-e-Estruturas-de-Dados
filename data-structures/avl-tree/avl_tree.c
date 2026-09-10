#include "avl_tree.h"

#include <stdlib.h>

typedef struct no {
    int chave;
    int altura;          /* altura da subárvore em arestas (folha = 0) */
    struct no *esq;
    struct no *dir;
} No;

struct avl {
    No *raiz;
    size_t tamanho;
};

AVL *avl_criar(void) {
    AVL *t = malloc(sizeof *t);
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

void avl_destruir(AVL *t) {
    if (t == NULL)
        return;
    destruir_no(t->raiz);
    free(t);
}

size_t avl_tamanho(const AVL *t) {
    return t->tamanho;
}

static int altura(const No *n) {
    return n == NULL ? -1 : n->altura;
}

static int maior(int a, int b) {
    return a > b ? a : b;
}

static void atualizar_altura(No *n) {
    n->altura = 1 + maior(altura(n->esq), altura(n->dir));
}

/* fator de balanceamento: >0 pende para a esquerda, <0 para a direita */
static int fator(const No *n) {
    return altura(n->esq) - altura(n->dir);
}

static No *rotacionar_dir(No *y) {
    No *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}

static No *rotacionar_esq(No *x) {
    No *y = x->dir;
    x->dir = y->esq;
    y->esq = x;
    atualizar_altura(x);
    atualizar_altura(y);
    return y;
}

/* Recalcula a altura de `n` e aplica a rotação necessária se ele ficou
   desbalanceado. Devolve a nova raiz da subárvore. */
static No *rebalancear(No *n) {
    atualizar_altura(n);
    int f = fator(n);

    if (f > 1) {                     /* pesado à esquerda */
        if (fator(n->esq) < 0)       /* caso esquerda-direita */
            n->esq = rotacionar_esq(n->esq);
        return rotacionar_dir(n);
    }
    if (f < -1) {                    /* pesado à direita */
        if (fator(n->dir) > 0)       /* caso direita-esquerda */
            n->dir = rotacionar_dir(n->dir);
        return rotacionar_esq(n);
    }
    return n;
}

static No *no_criar(int chave) {
    No *n = malloc(sizeof *n);
    if (n != NULL) {
        n->chave = chave;
        n->altura = 0;
        n->esq = NULL;
        n->dir = NULL;
    }
    return n;
}

static No *inserir_no(No *n, int chave, int *resultado) {
    if (n == NULL) {
        No *novo = no_criar(chave);
        *resultado = novo == NULL ? -1 : 0;
        return novo;
    }

    if (chave < n->chave)
        n->esq = inserir_no(n->esq, chave, resultado);
    else if (chave > n->chave)
        n->dir = inserir_no(n->dir, chave, resultado);
    else {
        *resultado = 1;
        return n;
    }

    if (*resultado != 0)             /* já existia ou falhou: não mexe na forma */
        return n;

    return rebalancear(n);
}

int avl_inserir(AVL *t, int chave) {
    int resultado = 0;
    No *nova_raiz = inserir_no(t->raiz, chave, &resultado);

    if (resultado == -1)
        return -1;

    t->raiz = nova_raiz;
    if (resultado == 0)
        t->tamanho++;
    return resultado;               /* 0 inseriu, 1 já existia */
}

static No *min_no(No *n) {
    while (n->esq != NULL)
        n = n->esq;
    return n;
}

static No *remover_no(No *n, int chave, int *removeu) {
    if (n == NULL) {
        *removeu = 0;
        return NULL;
    }

    if (chave < n->chave) {
        n->esq = remover_no(n->esq, chave, removeu);
    } else if (chave > n->chave) {
        n->dir = remover_no(n->dir, chave, removeu);
    } else {
        *removeu = 1;
        if (n->esq == NULL || n->dir == NULL) {
            No *filho = n->esq != NULL ? n->esq : n->dir;
            free(n);
            return filho;           /* pode ser NULL */
        }
        No *suc = min_no(n->dir);
        n->chave = suc->chave;
        int ignorado;
        n->dir = remover_no(n->dir, suc->chave, &ignorado);
    }

    if (*removeu == 0)
        return n;

    return rebalancear(n);
}

int avl_remover(AVL *t, int chave) {
    int removeu = 0;
    t->raiz = remover_no(t->raiz, chave, &removeu);
    if (removeu)
        t->tamanho--;
    return removeu ? 0 : -1;
}

int avl_contem(const AVL *t, int chave) {
    const No *n = t->raiz;
    while (n != NULL) {
        if (chave == n->chave)
            return 1;
        n = chave < n->chave ? n->esq : n->dir;
    }
    return 0;
}

int avl_altura(const AVL *t) {
    return altura(t->raiz);
}

static void em_ordem_no(const No *n, void (*visita)(int, void *), void *ctx) {
    if (n == NULL)
        return;
    em_ordem_no(n->esq, visita, ctx);
    visita(n->chave, ctx);
    em_ordem_no(n->dir, visita, ctx);
}

void avl_em_ordem(const AVL *t, void (*visita)(int, void *), void *ctx) {
    em_ordem_no(t->raiz, visita, ctx);
}
