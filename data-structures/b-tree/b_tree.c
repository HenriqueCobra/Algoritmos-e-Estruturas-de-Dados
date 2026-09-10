#include "b_tree.h"

#include <limits.h>
#include <stdlib.h>

/* Grau mínimo. Cada nó (menos a raiz) tem de T-1 a 2T-1 chaves. */
#define T 3
#define MAX_CHAVES (2 * T - 1)
#define MAX_FILHOS (2 * T)

typedef struct no {
    int n;                         /* número de chaves em uso */
    int folha;
    int chaves[MAX_CHAVES];
    struct no *filhos[MAX_FILHOS];
} No;

struct b_tree {
    No *raiz;
    size_t tamanho;
};

static No *no_criar(int folha) {
    No *x = calloc(1, sizeof *x);
    if (x != NULL)
        x->folha = folha;
    return x;
}

ArvoreB *btree_criar(void) {
    ArvoreB *t = malloc(sizeof *t);
    if (t == NULL)
        return NULL;
    t->raiz = no_criar(1);
    if (t->raiz == NULL) {
        free(t);
        return NULL;
    }
    t->tamanho = 0;
    return t;
}

static void destruir_no(No *x) {
    if (x == NULL)
        return;
    if (!x->folha)
        for (int i = 0; i <= x->n; i++)
            destruir_no(x->filhos[i]);
    free(x);
}

void btree_destruir(ArvoreB *t) {
    if (t == NULL)
        return;
    destruir_no(t->raiz);
    free(t);
}

size_t btree_tamanho(const ArvoreB *t) {
    return t->tamanho;
}

/* ---- busca ---- */

static int contem_no(const No *x, int chave) {
    int i = 0;
    while (i < x->n && chave > x->chaves[i])
        i++;
    if (i < x->n && chave == x->chaves[i])
        return 1;
    return x->folha ? 0 : contem_no(x->filhos[i], chave);
}

int btree_contem(const ArvoreB *t, int chave) {
    return contem_no(t->raiz, chave);
}

/* ---- inserção ---- */

/* Divide o filho cheio y = pai->filhos[i]: a chave do meio sobe para o pai e a
   metade de cima vira um novo nó z. */
static int dividir_filho(No *pai, int i) {
    No *y = pai->filhos[i];
    No *z = no_criar(y->folha);
    if (z == NULL)
        return -1;

    z->n = T - 1;
    for (int j = 0; j < T - 1; j++)
        z->chaves[j] = y->chaves[j + T];
    if (!y->folha)
        for (int j = 0; j < T; j++)
            z->filhos[j] = y->filhos[j + T];
    y->n = T - 1;

    for (int j = pai->n; j > i; j--)
        pai->filhos[j + 1] = pai->filhos[j];
    pai->filhos[i + 1] = z;
    for (int j = pai->n - 1; j >= i; j--)
        pai->chaves[j + 1] = pai->chaves[j];
    pai->chaves[i] = y->chaves[T - 1];
    pai->n++;
    return 0;
}

/* Insere em x, que não está cheio. Devolve 0 se inseriu, 1 se já existia,
   -1 em falha de alocação. */
static int inserir_nao_cheio(No *x, int chave) {
    int i = x->n - 1;
    while (i >= 0 && chave < x->chaves[i])
        i--;
    if (i >= 0 && chave == x->chaves[i])
        return 1;
    /* a chave, se existir/entrar, fica logo após o índice i */

    if (x->folha) {
        for (int j = x->n - 1; j > i; j--)
            x->chaves[j + 1] = x->chaves[j];
        x->chaves[i + 1] = chave;
        x->n++;
        return 0;
    }

    i++;

    if (x->filhos[i]->n == MAX_CHAVES) {
        if (dividir_filho(x, i) != 0)
            return -1;
        if (chave == x->chaves[i])
            return 1;
        if (chave > x->chaves[i])
            i++;
    }
    return inserir_nao_cheio(x->filhos[i], chave);
}

int btree_inserir(ArvoreB *t, int chave) {
    No *r = t->raiz;

    if (r->n == MAX_CHAVES) {
        No *nova = no_criar(0);
        if (nova == NULL)
            return -1;
        nova->filhos[0] = r;
        t->raiz = nova;
        if (dividir_filho(nova, 0) != 0) {
            /* desfaz: volta a raiz antiga */
            t->raiz = r;
            free(nova);
            return -1;
        }
        r = nova;
    }

    int resultado = inserir_nao_cheio(r, chave);
    if (resultado == 0)
        t->tamanho++;
    return resultado;
}

/* ---- remoção (CLRS) ---- */

static int maximo_no(No *x) {
    while (!x->folha)
        x = x->filhos[x->n];
    return x->chaves[x->n - 1];
}

static int minimo_no(No *x) {
    while (!x->folha)
        x = x->filhos[0];
    return x->chaves[0];
}

/* Funde filhos[i], a chave chaves[i] e filhos[i+1] num só nó (filhos[i]). */
static void fundir(No *x, int i) {
    No *esq = x->filhos[i];
    No *dir = x->filhos[i + 1];

    esq->chaves[T - 1] = x->chaves[i];
    for (int j = 0; j < dir->n; j++)
        esq->chaves[T + j] = dir->chaves[j];
    if (!esq->folha)
        for (int j = 0; j <= dir->n; j++)
            esq->filhos[T + j] = dir->filhos[j];
    esq->n += dir->n + 1;

    for (int j = i + 1; j < x->n; j++)
        x->chaves[j - 1] = x->chaves[j];
    for (int j = i + 2; j <= x->n; j++)
        x->filhos[j - 1] = x->filhos[j];
    x->n--;

    free(dir);
}

/* Garante que filhos[i] tenha pelo menos T chaves, tomando emprestado de um
   irmão ou fundindo. Devolve o índice do filho onde continuar a descida. */
static int garantir_filho(No *x, int i) {
    No *filho = x->filhos[i];
    if (filho->n >= T)
        return i;

    No *esq = i > 0 ? x->filhos[i - 1] : NULL;
    No *dir = i < x->n ? x->filhos[i + 1] : NULL;

    if (esq != NULL && esq->n >= T) {
        /* rotação da direita: desce chaves[i-1], sobe a última do irmão esq */
        for (int j = filho->n; j > 0; j--)
            filho->chaves[j] = filho->chaves[j - 1];
        if (!filho->folha)
            for (int j = filho->n + 1; j > 0; j--)
                filho->filhos[j] = filho->filhos[j - 1];
        filho->chaves[0] = x->chaves[i - 1];
        if (!filho->folha)
            filho->filhos[0] = esq->filhos[esq->n];
        x->chaves[i - 1] = esq->chaves[esq->n - 1];
        filho->n++;
        esq->n--;
        return i;
    }

    if (dir != NULL && dir->n >= T) {
        /* rotação da esquerda: desce chaves[i], sobe a primeira do irmão dir */
        filho->chaves[filho->n] = x->chaves[i];
        if (!filho->folha)
            filho->filhos[filho->n + 1] = dir->filhos[0];
        filho->n++;
        x->chaves[i] = dir->chaves[0];
        for (int j = 0; j < dir->n - 1; j++)
            dir->chaves[j] = dir->chaves[j + 1];
        if (!dir->folha)
            for (int j = 0; j < dir->n; j++)
                dir->filhos[j] = dir->filhos[j + 1];
        dir->n--;
        return i;
    }

    /* nenhum irmão pode emprestar: funde */
    if (dir != NULL) {
        fundir(x, i);
        return i;
    }
    fundir(x, i - 1);
    return i - 1;
}

static void remover_de(No *x, int chave) {
    int i = 0;
    while (i < x->n && chave > x->chaves[i])
        i++;

    if (i < x->n && chave == x->chaves[i]) {
        if (x->folha) {
            for (int j = i; j < x->n - 1; j++)
                x->chaves[j] = x->chaves[j + 1];
            x->n--;
            return;
        }
        /* nó interno */
        if (x->filhos[i]->n >= T) {
            int pred = maximo_no(x->filhos[i]);
            x->chaves[i] = pred;
            remover_de(x->filhos[i], pred);
        } else if (x->filhos[i + 1]->n >= T) {
            int suc = minimo_no(x->filhos[i + 1]);
            x->chaves[i] = suc;
            remover_de(x->filhos[i + 1], suc);
        } else {
            fundir(x, i);
            remover_de(x->filhos[i], chave);
        }
        return;
    }

    if (x->folha)
        return;                        /* não está na árvore */

    int idx = garantir_filho(x, i);
    remover_de(x->filhos[idx], chave);
}

int btree_remover(ArvoreB *t, int chave) {
    if (!btree_contem(t, chave))
        return -1;

    remover_de(t->raiz, chave);
    t->tamanho--;

    if (t->raiz->n == 0 && !t->raiz->folha) {
        No *antiga = t->raiz;
        t->raiz = antiga->filhos[0];
        free(antiga);
    }
    return 0;
}

/* ---- utilidades ---- */

static int altura_no(const No *x) {
    int h = 1;
    while (!x->folha) {
        x = x->filhos[0];
        h++;
    }
    return h;
}

int btree_altura(const ArvoreB *t) {
    return t->tamanho == 0 ? 0 : altura_no(t->raiz);
}

static void em_ordem_no(const No *x, void (*visita)(int, void *), void *ctx) {
    for (int i = 0; i < x->n; i++) {
        if (!x->folha)
            em_ordem_no(x->filhos[i], visita, ctx);
        visita(x->chaves[i], ctx);
    }
    if (!x->folha)
        em_ordem_no(x->filhos[x->n], visita, ctx);
}

void btree_em_ordem(const ArvoreB *t, void (*visita)(int, void *), void *ctx) {
    em_ordem_no(t->raiz, visita, ctx);
}

static int checar(const No *x, int eh_raiz, int menor, int maior, int *profundidade,
                  int nivel) {
    if (x->n < 1 && !eh_raiz)
        return 0;
    if (x->n > MAX_CHAVES)
        return 0;
    if (!eh_raiz && x->n < T - 1)
        return 0;

    for (int i = 0; i < x->n; i++) {
        if (x->chaves[i] <= menor || x->chaves[i] >= maior)
            return 0;
        if (i > 0 && x->chaves[i] <= x->chaves[i - 1])
            return 0;
    }

    if (x->folha) {
        if (*profundidade == -1)
            *profundidade = nivel;
        return *profundidade == nivel;
    }

    for (int i = 0; i <= x->n; i++) {
        int lo = i == 0 ? menor : x->chaves[i - 1];
        int hi = i == x->n ? maior : x->chaves[i];
        if (!checar(x->filhos[i], 0, lo, hi, profundidade, nivel + 1))
            return 0;
    }
    return 1;
}

int btree_valida(const ArvoreB *t) {
    if (t->tamanho == 0)
        return t->raiz->folha && t->raiz->n == 0;
    int profundidade = -1;
    return checar(t->raiz, 1, INT_MIN, INT_MAX, &profundidade, 0);
}
