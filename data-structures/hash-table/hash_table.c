#include "hash_table.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BALDES_INICIAIS 16
#define CARGA_MAXIMA    0.75

typedef struct par {
    char *chave;
    int valor;
    struct par *prox;
} Par;

struct tabela_hash {
    Par **baldes;
    size_t n_baldes;
    size_t tamanho;
};

/* FNV-1a: hash simples e de boa dispersão para strings curtas. */
static uint64_t hash_str(const char *s) {
    uint64_t h = 1469598103934665603ULL;
    for (; *s != '\0'; s++) {
        h ^= (unsigned char)*s;
        h *= 1099511628211ULL;
    }
    return h;
}

static char *copia_str(const char *s) {
    size_t n = strlen(s) + 1;
    char *c = malloc(n);
    if (c != NULL)
        memcpy(c, s, n);
    return c;
}

TabelaHash *tabela_criar(void) {
    TabelaHash *t = malloc(sizeof *t);
    if (t == NULL)
        return NULL;

    t->baldes = calloc(BALDES_INICIAIS, sizeof *t->baldes);
    if (t->baldes == NULL) {
        free(t);
        return NULL;
    }
    t->n_baldes = BALDES_INICIAIS;
    t->tamanho = 0;
    return t;
}

void tabela_destruir(TabelaHash *t) {
    if (t == NULL)
        return;

    for (size_t i = 0; i < t->n_baldes; i++) {
        Par *p = t->baldes[i];
        while (p != NULL) {
            Par *seguinte = p->prox;
            free(p->chave);
            free(p);
            p = seguinte;
        }
    }
    free(t->baldes);
    free(t);
}

size_t tabela_tamanho(const TabelaHash *t) {
    return t->tamanho;
}

/* Dobra o número de baldes e redistribui os pares. */
static int reespalhar(TabelaHash *t) {
    size_t novo_n = t->n_baldes * 2;
    Par **novos = calloc(novo_n, sizeof *novos);
    if (novos == NULL)
        return -1;

    for (size_t i = 0; i < t->n_baldes; i++) {
        Par *p = t->baldes[i];
        while (p != NULL) {
            Par *seguinte = p->prox;
            size_t b = (size_t)(hash_str(p->chave) % novo_n);
            p->prox = novos[b];
            novos[b] = p;
            p = seguinte;
        }
    }

    free(t->baldes);
    t->baldes = novos;
    t->n_baldes = novo_n;
    return 0;
}

static Par *procurar(const TabelaHash *t, const char *chave) {
    size_t b = (size_t)(hash_str(chave) % t->n_baldes);
    for (Par *p = t->baldes[b]; p != NULL; p = p->prox) {
        if (strcmp(p->chave, chave) == 0)
            return p;
    }
    return NULL;
}

int tabela_por(TabelaHash *t, const char *chave, int valor) {
    Par *existente = procurar(t, chave);
    if (existente != NULL) {
        existente->valor = valor;
        return 0;
    }

    if ((double)(t->tamanho + 1) > CARGA_MAXIMA * (double)t->n_baldes) {
        if (reespalhar(t) != 0)
            return -1;
    }

    Par *novo = malloc(sizeof *novo);
    if (novo == NULL)
        return -1;
    novo->chave = copia_str(chave);
    if (novo->chave == NULL) {
        free(novo);
        return -1;
    }
    novo->valor = valor;

    size_t b = (size_t)(hash_str(chave) % t->n_baldes);
    novo->prox = t->baldes[b];
    t->baldes[b] = novo;
    t->tamanho++;
    return 0;
}

int tabela_obter(const TabelaHash *t, const char *chave, int *saida) {
    Par *p = procurar(t, chave);
    if (p == NULL)
        return -1;
    if (saida != NULL)
        *saida = p->valor;
    return 0;
}

int tabela_contem(const TabelaHash *t, const char *chave) {
    return procurar(t, chave) != NULL;
}

int tabela_remover(TabelaHash *t, const char *chave) {
    size_t b = (size_t)(hash_str(chave) % t->n_baldes);

    Par *anterior = NULL;
    for (Par *p = t->baldes[b]; p != NULL; anterior = p, p = p->prox) {
        if (strcmp(p->chave, chave) == 0) {
            if (anterior == NULL)
                t->baldes[b] = p->prox;
            else
                anterior->prox = p->prox;
            free(p->chave);
            free(p);
            t->tamanho--;
            return 0;
        }
    }
    return -1;
}
