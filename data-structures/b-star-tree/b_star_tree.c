#include "b_star_tree.h"

#include <limits.h>
#include <stdlib.h>

/* Fora a raiz, cada nó tem de MINK a MAXK chaves (MINK ~ 2/3 de MAXK).
   A raiz tem de 1 a 2*MINK chaves (ou 0, se for uma folha e a árvore vazia). */
#define MAXK 6
#define MINK 4
#define RAIZ_MAXK (2 * MINK)

/* Espaço de sobra para o transbordo momentâneo antes do rebalanceamento. */
#define CAP_CHAVES (RAIZ_MAXK + 4)
#define CAP_FILHOS (CAP_CHAVES + 1)

typedef struct no {
    int n;
    int folha;
    int chaves[CAP_CHAVES];
    struct no *filhos[CAP_FILHOS];
} No;

struct b_star_tree {
    No *raiz;
    size_t tamanho;
};

static No *no_criar(int folha) {
    No *x = calloc(1, sizeof *x);
    if (x != NULL)
        x->folha = folha;
    return x;
}

ArvoreBStar *bstar_criar(void) {
    ArvoreBStar *t = malloc(sizeof *t);
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

void bstar_destruir(ArvoreBStar *t) {
    if (t == NULL)
        return;
    destruir_no(t->raiz);
    free(t);
}

size_t bstar_tamanho(const ArvoreBStar *t) {
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

int bstar_contem(const ArvoreBStar *t, int chave) {
    return contem_no(t->raiz, chave);
}

/* ---- núcleo do rebalanceamento ----

   Junta `m_in` filhos consecutivos de `x` a partir de `i0` (e as m_in-1 chaves
   separadoras entre eles) e reescreve o trecho como `parts` nós, promovendo
   parts-1 novas chaves separadoras. Cobre todos os casos:
     redistribuir 2->2, dividir 2->3, fundir 3->2 e colapsar 2->1. */
static int juntar_dividir(No *x, int i0, int m_in, int parts) {
    int chaves[3 * MAXK + 8];
    No *filhos[3 * MAXK + 12];
    int nk = 0, nf = 0;
    int folha = x->filhos[i0]->folha;

    for (int m = 0; m < m_in; m++) {
        No *nd = x->filhos[i0 + m];
        for (int j = 0; j < nd->n; j++)
            chaves[nk++] = nd->chaves[j];
        if (!folha)
            for (int j = 0; j <= nd->n; j++)
                filhos[nf++] = nd->filhos[j];
        if (m < m_in - 1)
            chaves[nk++] = x->chaves[i0 + m];
    }

    int n_promove = parts - 1;
    int em_nos = nk - n_promove;
    int base = em_nos / parts;
    int resto = em_nos % parts;

    No *nos[4];
    for (int p = 0; p < parts; p++) {
        if (p < m_in) {
            nos[p] = x->filhos[i0 + p];
        } else {
            nos[p] = no_criar(folha);
            if (nos[p] == NULL) {
                for (int q = m_in; q < p; q++)
                    free(nos[q]);
                return -1;
            }
        }
    }
    for (int p = parts; p < m_in; p++)
        free(x->filhos[i0 + p]);

    int ki = 0, ci = 0, si = 0;
    int seps[3];
    for (int p = 0; p < parts; p++) {
        int cnt = base + (p < resto ? 1 : 0);
        No *nd = nos[p];
        nd->folha = folha;
        nd->n = cnt;
        for (int j = 0; j < cnt; j++)
            nd->chaves[j] = chaves[ki++];
        if (!folha)
            for (int j = 0; j <= cnt; j++)
                nd->filhos[j] = filhos[ci++];
        if (p < parts - 1)
            seps[si++] = chaves[ki++];
    }

    int dk = (parts - 1) - (m_in - 1);   /* variação no nº de chaves de x */
    if (dk > 0) {
        for (int j = x->n - 1; j >= i0 + (m_in - 1); j--)
            x->chaves[j + dk] = x->chaves[j];
        for (int j = x->n; j >= i0 + m_in; j--)
            x->filhos[j + dk] = x->filhos[j];
    } else if (dk < 0) {
        for (int j = i0 + (m_in - 1); j < x->n; j++)
            x->chaves[j + dk] = x->chaves[j];
        for (int j = i0 + m_in; j <= x->n; j++)
            x->filhos[j + dk] = x->filhos[j];
    }
    x->n += dk;

    for (int j = 0; j < parts - 1; j++)
        x->chaves[i0 + j] = seps[j];
    for (int j = 0; j < parts; j++)
        x->filhos[i0 + j] = nos[j];

    return 0;
}

/* ---- inserção ---- */

/* filhos[i] de x transbordou (MAXK+1 chaves). Redistribui com um irmão ou,
   se o irmão também está cheio, faz a divisão 2->3. */
static int resolver_transbordo(No *x, int i) {
    int tem_dir = i < x->n;
    int tem_esq = i > 0;

    int i0, irmao_n;
    if (tem_dir) {
        i0 = i;
        irmao_n = x->filhos[i + 1]->n;
    } else {
        i0 = i - 1;
        irmao_n = x->filhos[i - 1]->n;
    }
    (void)tem_esq;

    int parts = irmao_n == MAXK ? 3 : 2;
    return juntar_dividir(x, i0, 2, parts);
}

static int inserir_rec(No *x, int chave, int eh_raiz) {
    int i = 0;
    while (i < x->n && chave > x->chaves[i])
        i++;
    if (i < x->n && chave == x->chaves[i])
        return 1;

    if (x->folha) {
        for (int j = x->n; j > i; j--)
            x->chaves[j] = x->chaves[j - 1];
        x->chaves[i] = chave;
        x->n++;
        return 0;
    }

    int r = inserir_rec(x->filhos[i], chave, 0);
    if (r != 0)
        return r;

    if (x->filhos[i]->n > MAXK)
        if (resolver_transbordo(x, i) != 0)
            return -1;
    (void)eh_raiz;
    return 0;
}

static int dividir_raiz(ArvoreBStar *t) {
    No *r = t->raiz;
    No *nova = no_criar(0);
    No *dir = no_criar(r->folha);
    if (nova == NULL || dir == NULL) {
        free(nova);
        free(dir);
        return -1;
    }

    int meio = r->n / 2;
    dir->n = r->n - meio - 1;
    for (int j = 0; j < dir->n; j++)
        dir->chaves[j] = r->chaves[meio + 1 + j];
    if (!r->folha)
        for (int j = 0; j <= dir->n; j++)
            dir->filhos[j] = r->filhos[meio + 1 + j];

    nova->folha = 0;
    nova->n = 1;
    nova->chaves[0] = r->chaves[meio];
    nova->filhos[0] = r;
    nova->filhos[1] = dir;
    r->n = meio;

    t->raiz = nova;
    return 0;
}

int bstar_inserir(ArvoreBStar *t, int chave) {
    int r = inserir_rec(t->raiz, chave, 1);
    if (r == -1)
        return -1;

    if (t->raiz->n > RAIZ_MAXK) {
        if (dividir_raiz(t) != 0)
            return -1;
    }
    if (r == 0)
        t->tamanho++;
    return r;
}

/* ---- remoção ---- */

static int min_chaves_no(const No *x, int eh_raiz) {
    if (eh_raiz)
        return x->folha ? 0 : 1;
    return MINK;
}

/* filhos[i] de x ficou com MINK-1 chaves. Empresta de um irmão ou funde. */
static int resolver_falta(No *x, int i) {
    No *esq = i > 0 ? x->filhos[i - 1] : NULL;
    No *dir = i < x->n ? x->filhos[i + 1] : NULL;

    if (esq != NULL && esq->n > MINK)
        return juntar_dividir(x, i - 1, 2, 2);
    if (dir != NULL && dir->n > MINK)
        return juntar_dividir(x, i, 2, 2);

    if (esq == NULL && dir == NULL)
        return 0;                       /* raiz com um único filho: nada a fazer */

    if (x->n == 1) {
        /* x tem só 2 filhos (necessariamente é a raiz): funde num só */
        return juntar_dividir(x, 0, 2, 1);
    }

    int i0;
    if (esq != NULL && dir != NULL)
        i0 = i - 1;
    else if (esq == NULL)
        i0 = i;
    else
        i0 = i - 2;

    int total = x->filhos[i0]->n + x->filhos[i0 + 1]->n + x->filhos[i0 + 2]->n;
    int parts = total <= 2 * MAXK - 1 ? 2 : 3;
    return juntar_dividir(x, i0, 3, parts);
}

static int remover_rec(No *x, int chave, int eh_raiz) {
    int i = 0;
    while (i < x->n && chave > x->chaves[i])
        i++;

    if (i < x->n && chave == x->chaves[i]) {
        if (x->folha) {
            for (int j = i; j < x->n - 1; j++)
                x->chaves[j] = x->chaves[j + 1];
            x->n--;
            return 0;
        }
        /* nó interno: troca pela maior chave da subárvore à esquerda */
        No *p = x->filhos[i];
        while (!p->folha)
            p = p->filhos[p->n];
        int pred = p->chaves[p->n - 1];
        x->chaves[i] = pred;
        int r = remover_rec(x->filhos[i], pred, 0);
        if (x->filhos[i]->n < MINK)
            if (resolver_falta(x, i) != 0)
                return -1;
        return r;
    }

    if (x->folha)
        return -1;

    int r = remover_rec(x->filhos[i], chave, 0);
    if (r != 0)
        return r;

    if (x->filhos[i]->n < MINK)
        if (resolver_falta(x, i) != 0)
            return -1;
    (void)eh_raiz;
    return 0;
}

int bstar_remover(ArvoreBStar *t, int chave) {
    int r = remover_rec(t->raiz, chave, 1);
    if (r != 0)
        return -1;

    if (!t->raiz->folha && t->raiz->n == 0) {
        No *antiga = t->raiz;
        t->raiz = antiga->filhos[0];
        free(antiga);
    }
    t->tamanho--;
    return 0;
}

/* ---- utilidades ---- */

int bstar_altura(const ArvoreBStar *t) {
    if (t->tamanho == 0)
        return 0;
    int h = 1;
    for (const No *x = t->raiz; !x->folha; x = x->filhos[0])
        h++;
    return h;
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

void bstar_em_ordem(const ArvoreBStar *t, void (*visita)(int, void *), void *ctx) {
    em_ordem_no(t->raiz, visita, ctx);
}

static int checar(const No *x, int eh_raiz, long lo, long hi, int nivel,
                  int *nivel_folha) {
    int max_ok = eh_raiz ? RAIZ_MAXK : MAXK;
    if (x->n > max_ok || x->n < min_chaves_no(x, eh_raiz))
        return 0;

    for (int i = 0; i < x->n; i++) {
        if (i > 0 && x->chaves[i] <= x->chaves[i - 1])
            return 0;
        if (x->chaves[i] < lo || x->chaves[i] >= hi)
            return 0;
    }

    if (x->folha) {
        if (*nivel_folha == -1)
            *nivel_folha = nivel;
        return *nivel_folha == nivel;
    }

    for (int i = 0; i <= x->n; i++) {
        long sub_lo = i == 0 ? lo : x->chaves[i - 1];
        long sub_hi = i == x->n ? hi : x->chaves[i];
        if (!checar(x->filhos[i], 0, sub_lo, sub_hi, nivel + 1, nivel_folha))
            return 0;
    }
    return 1;
}

int bstar_valida(const ArvoreBStar *t) {
    if (t->raiz->folha)
        return t->raiz->n >= 0 && t->raiz->n <= RAIZ_MAXK;
    int nivel_folha = -1;
    return checar(t->raiz, 1, INT_MIN, INT_MAX, 0, &nivel_folha);
}
