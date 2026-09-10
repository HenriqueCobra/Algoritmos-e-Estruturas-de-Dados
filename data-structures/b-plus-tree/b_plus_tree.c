#include "b_plus_tree.h"

#include <limits.h>
#include <stdlib.h>

/* Ordem: no máximo ORDEM-1 chaves por nó e ORDEM filhos por nó interno. */
#define ORDEM 5
#define MIN_FOLHA (ORDEM / 2)                 /* 2 */
#define MIN_INTERNO (((ORDEM + 1) / 2) - 1)   /* 2 chaves (3 filhos) */

typedef struct no {
    int folha;
    int n;                         /* nº de chaves */
    int chaves[ORDEM];             /* uma posição extra para o overflow antes do split */
    struct no *filhos[ORDEM + 1];  /* só nós internos */
    struct no *prox;               /* só folhas: próxima folha, ou NULL */
} No;

struct b_plus_tree {
    No *raiz;
    size_t tamanho;
};

static No *no_criar(int folha) {
    No *x = calloc(1, sizeof *x);
    if (x != NULL)
        x->folha = folha;
    return x;
}

ArvoreBMais *bplus_criar(void) {
    ArvoreBMais *t = malloc(sizeof *t);
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

void bplus_destruir(ArvoreBMais *t) {
    if (t == NULL)
        return;
    destruir_no(t->raiz);
    free(t);
}

size_t bplus_tamanho(const ArvoreBMais *t) {
    return t->tamanho;
}

/* ---- busca ---- */

static const No *folha_de(const No *x, int chave) {
    while (!x->folha) {
        int i = 0;
        while (i < x->n && chave >= x->chaves[i])
            i++;
        x = x->filhos[i];
    }
    return x;
}

int bplus_contem(const ArvoreBMais *t, int chave) {
    const No *f = folha_de(t->raiz, chave);
    for (int i = 0; i < f->n; i++)
        if (f->chaves[i] == chave)
            return 1;
    return 0;
}

/* ---- inserção ---- */

/* 0 ok, 1 já existe, -1 alloc. Se o nó se dividiu, *promovida e *irmao são
   preenchidos com a chave separadora e o novo nó à direita. */
static int inserir_rec(No *x, int chave, int *promovida, No **irmao) {
    *irmao = NULL;

    if (x->folha) {
        int i = 0;
        while (i < x->n && chave > x->chaves[i])
            i++;
        if (i < x->n && chave == x->chaves[i])
            return 1;

        for (int j = x->n; j > i; j--)
            x->chaves[j] = x->chaves[j - 1];
        x->chaves[i] = chave;
        x->n++;

        if (x->n < ORDEM)
            return 0;

        No *z = no_criar(1);
        if (z == NULL) {
            for (int j = i; j < x->n - 1; j++)   /* desfaz a inserção */
                x->chaves[j] = x->chaves[j + 1];
            x->n--;
            return -1;
        }
        int meio = ORDEM / 2;
        z->n = x->n - meio;
        for (int j = 0; j < z->n; j++)
            z->chaves[j] = x->chaves[meio + j];
        x->n = meio;
        z->prox = x->prox;
        x->prox = z;
        *promovida = z->chaves[0];               /* a chave é copiada para cima */
        *irmao = z;
        return 0;
    }

    int i = 0;
    while (i < x->n && chave >= x->chaves[i])
        i++;

    int prom;
    No *novo;
    int r = inserir_rec(x->filhos[i], chave, &prom, &novo);
    if (r != 0 || novo == NULL)
        return r;

    for (int j = x->n; j > i; j--)
        x->chaves[j] = x->chaves[j - 1];
    for (int j = x->n + 1; j > i + 1; j--)
        x->filhos[j] = x->filhos[j - 1];
    x->chaves[i] = prom;
    x->filhos[i + 1] = novo;
    x->n++;

    if (x->n < ORDEM)
        return 0;

    No *z = no_criar(0);
    if (z == NULL)
        return -1;
    int meio = x->n / 2;
    *promovida = x->chaves[meio];                 /* sobe e sai deste nó */
    z->n = x->n - meio - 1;
    for (int j = 0; j < z->n; j++)
        z->chaves[j] = x->chaves[meio + 1 + j];
    for (int j = 0; j <= z->n; j++)
        z->filhos[j] = x->filhos[meio + 1 + j];
    x->n = meio;
    *irmao = z;
    return 0;
}

int bplus_inserir(ArvoreBMais *t, int chave) {
    int prom;
    No *novo;
    int r = inserir_rec(t->raiz, chave, &prom, &novo);

    if (r == 0 && novo != NULL) {
        No *nova_raiz = no_criar(0);
        if (nova_raiz == NULL)
            return -1;
        nova_raiz->n = 1;
        nova_raiz->chaves[0] = prom;
        nova_raiz->filhos[0] = t->raiz;
        nova_raiz->filhos[1] = novo;
        t->raiz = nova_raiz;
    }
    if (r == 0)
        t->tamanho++;
    return r;
}

/* ---- remoção ---- */

static void corrigir_underflow(No *x, int i) {
    No *c = x->filhos[i];
    No *esq = i > 0 ? x->filhos[i - 1] : NULL;
    No *dir = i < x->n ? x->filhos[i + 1] : NULL;
    int min = c->folha ? MIN_FOLHA : MIN_INTERNO;

    if (c->folha) {
        if (esq != NULL && esq->n > min) {
            for (int j = c->n; j > 0; j--)
                c->chaves[j] = c->chaves[j - 1];
            c->chaves[0] = esq->chaves[esq->n - 1];
            c->n++;
            esq->n--;
            x->chaves[i - 1] = c->chaves[0];
            return;
        }
        if (dir != NULL && dir->n > min) {
            c->chaves[c->n] = dir->chaves[0];
            c->n++;
            for (int j = 0; j < dir->n - 1; j++)
                dir->chaves[j] = dir->chaves[j + 1];
            dir->n--;
            x->chaves[i] = dir->chaves[0];
            return;
        }
        if (dir != NULL) {
            for (int j = 0; j < dir->n; j++)
                c->chaves[c->n + j] = dir->chaves[j];
            c->n += dir->n;
            c->prox = dir->prox;
            free(dir);
            for (int j = i; j < x->n - 1; j++)
                x->chaves[j] = x->chaves[j + 1];
            for (int j = i + 1; j < x->n; j++)
                x->filhos[j] = x->filhos[j + 1];
            x->n--;
        } else {
            for (int j = 0; j < c->n; j++)
                esq->chaves[esq->n + j] = c->chaves[j];
            esq->n += c->n;
            esq->prox = c->prox;
            free(c);
            for (int j = i - 1; j < x->n - 1; j++)
                x->chaves[j] = x->chaves[j + 1];
            for (int j = i; j < x->n; j++)
                x->filhos[j] = x->filhos[j + 1];
            x->n--;
        }
        return;
    }

    /* nó interno */
    if (esq != NULL && esq->n > min) {
        for (int j = c->n; j > 0; j--)
            c->chaves[j] = c->chaves[j - 1];
        for (int j = c->n + 1; j > 0; j--)
            c->filhos[j] = c->filhos[j - 1];
        c->chaves[0] = x->chaves[i - 1];
        c->filhos[0] = esq->filhos[esq->n];
        c->n++;
        x->chaves[i - 1] = esq->chaves[esq->n - 1];
        esq->n--;
        return;
    }
    if (dir != NULL && dir->n > min) {
        c->chaves[c->n] = x->chaves[i];
        c->filhos[c->n + 1] = dir->filhos[0];
        c->n++;
        x->chaves[i] = dir->chaves[0];
        for (int j = 0; j < dir->n - 1; j++)
            dir->chaves[j] = dir->chaves[j + 1];
        for (int j = 0; j < dir->n; j++)
            dir->filhos[j] = dir->filhos[j + 1];
        dir->n--;
        return;
    }

    if (dir != NULL) {
        c->chaves[c->n] = x->chaves[i];
        c->n++;
        for (int j = 0; j < dir->n; j++)
            c->chaves[c->n + j] = dir->chaves[j];
        for (int j = 0; j <= dir->n; j++)
            c->filhos[c->n + j] = dir->filhos[j];
        c->n += dir->n;
        free(dir);
        for (int j = i; j < x->n - 1; j++)
            x->chaves[j] = x->chaves[j + 1];
        for (int j = i + 1; j < x->n; j++)
            x->filhos[j] = x->filhos[j + 1];
        x->n--;
    } else {
        esq->chaves[esq->n] = x->chaves[i - 1];
        esq->n++;
        for (int j = 0; j < c->n; j++)
            esq->chaves[esq->n + j] = c->chaves[j];
        for (int j = 0; j <= c->n; j++)
            esq->filhos[esq->n + j] = c->filhos[j];
        esq->n += c->n;
        free(c);
        for (int j = i - 1; j < x->n - 1; j++)
            x->chaves[j] = x->chaves[j + 1];
        for (int j = i; j < x->n; j++)
            x->filhos[j] = x->filhos[j + 1];
        x->n--;
    }
}

static int remover_rec(No *x, int chave) {
    if (x->folha) {
        int i = 0;
        while (i < x->n && chave > x->chaves[i])
            i++;
        if (i >= x->n || x->chaves[i] != chave)
            return -1;
        for (int j = i; j < x->n - 1; j++)
            x->chaves[j] = x->chaves[j + 1];
        x->n--;
        return 0;
    }

    int i = 0;
    while (i < x->n && chave >= x->chaves[i])
        i++;

    int r = remover_rec(x->filhos[i], chave);
    if (r != 0)
        return r;

    No *c = x->filhos[i];
    int min = c->folha ? MIN_FOLHA : MIN_INTERNO;
    if (c->n < min)
        corrigir_underflow(x, i);
    return 0;
}

int bplus_remover(ArvoreBMais *t, int chave) {
    int r = remover_rec(t->raiz, chave);
    if (r != 0)
        return -1;

    t->tamanho--;
    if (!t->raiz->folha && t->raiz->n == 0) {
        No *antiga = t->raiz;
        t->raiz = antiga->filhos[0];
        free(antiga);
    }
    return 0;
}

/* ---- percursos ---- */

static const No *primeira_folha(const No *x) {
    while (!x->folha)
        x = x->filhos[0];
    return x;
}

void bplus_em_ordem(const ArvoreBMais *t, void (*visita)(int, void *), void *ctx) {
    for (const No *f = primeira_folha(t->raiz); f != NULL; f = f->prox)
        for (int i = 0; i < f->n; i++)
            visita(f->chaves[i], ctx);
}

void bplus_intervalo(const ArvoreBMais *t, int lo, int hi,
                     void (*visita)(int, void *), void *ctx) {
    const No *f = folha_de(t->raiz, lo);
    while (f != NULL) {
        for (int i = 0; i < f->n; i++) {
            if (f->chaves[i] < lo)
                continue;
            if (f->chaves[i] > hi)
                return;
            visita(f->chaves[i], ctx);
        }
        f = f->prox;
    }
}

int bplus_altura(const ArvoreBMais *t) {
    if (t->tamanho == 0)
        return 0;
    int h = 1;
    for (const No *x = t->raiz; !x->folha; x = x->filhos[0])
        h++;
    return h;
}

/* ---- validação ---- */

static int checar(const No *x, int eh_raiz, long lo, long hi, int nivel,
                  int *nivel_folha, int *menor) {
    if (x->n > ORDEM - 1)
        return 0;
    if (!eh_raiz) {
        int min = x->folha ? MIN_FOLHA : MIN_INTERNO;
        if (x->n < min)
            return 0;
    } else if (!x->folha && x->n < 1) {
        return 0;
    }

    for (int i = 0; i < x->n; i++) {
        if (i > 0 && x->chaves[i] <= x->chaves[i - 1])
            return 0;
        if (x->chaves[i] < lo || x->chaves[i] >= hi)
            return 0;
    }

    if (x->folha) {
        if (*nivel_folha == -1)
            *nivel_folha = nivel;
        if (*nivel_folha != nivel)
            return 0;
        *menor = x->n > 0 ? x->chaves[0] : INT_MAX;
        return 1;
    }

    int menor_esq = 0;
    for (int i = 0; i <= x->n; i++) {
        /* toda chave da subárvore i deve cair em [chaves[i-1], chaves[i]); isso
           já garante que cada separador é um roteador válido, mesmo quando fica
           "fantasma" (a menor chave à direita foi removida depois). */
        long sub_lo = i == 0 ? lo : x->chaves[i - 1];
        long sub_hi = i == x->n ? hi : x->chaves[i];
        int m;
        if (!checar(x->filhos[i], 0, sub_lo, sub_hi, nivel + 1, nivel_folha, &m))
            return 0;
        if (i == 0)
            menor_esq = m;
    }
    *menor = menor_esq;
    return 1;
}

int bplus_valida(const ArvoreBMais *t) {
    if (t->raiz->folha)
        return t->raiz->n >= 0 && t->raiz->n <= ORDEM - 1;
    int nivel_folha = -1;
    int menor;
    return checar(t->raiz, 1, INT_MIN, INT_MAX, 0, &nivel_folha, &menor);
}
