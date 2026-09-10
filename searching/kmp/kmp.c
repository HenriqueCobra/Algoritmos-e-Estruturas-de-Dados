#include "kmp.h"

#include <stdlib.h>
#include <string.h>

void kmp_tabela(const char *padrao, size_t m, size_t *tabela) {
    if (m == 0)
        return;
    tabela[0] = 0;

    size_t k = 0;                 /* tamanho do prefixo-sufixo atual */
    for (size_t i = 1; i < m; i++) {
        while (k > 0 && padrao[i] != padrao[k])
            k = tabela[k - 1];    /* recua para o próximo candidato */
        if (padrao[i] == padrao[k])
            k++;
        tabela[i] = k;
    }
}

/* Aloca a tabela de falha (buffer local para padrões pequenos, heap para os
   grandes). Devolve NULL só em falha de alocação. */
static size_t *montar_tabela(const char *padrao, size_t m, size_t *buf_local,
                             size_t tam_local, int *no_heap) {
    size_t *tabela = buf_local;
    *no_heap = 0;
    if (m > tam_local) {
        tabela = malloc(m * sizeof *tabela);
        if (tabela == NULL)
            return NULL;
        *no_heap = 1;
    }
    kmp_tabela(padrao, m, tabela);
    return tabela;
}

long kmp_buscar(const char *texto, const char *padrao) {
    size_t n = strlen(texto);
    size_t m = strlen(padrao);
    if (m == 0)
        return 0;

    size_t buf[128];
    int no_heap;
    size_t *tabela = montar_tabela(padrao, m, buf, 128, &no_heap);
    if (tabela == NULL)
        return -1;

    long achou = -1;
    size_t k = 0;
    for (size_t i = 0; i < n && achou == -1; i++) {
        while (k > 0 && texto[i] != padrao[k])
            k = tabela[k - 1];
        if (texto[i] == padrao[k])
            k++;
        if (k == m)
            achou = (long)(i - m + 1);
    }

    if (no_heap)
        free(tabela);
    return achou;
}

size_t kmp_buscar_todas(const char *texto, const char *padrao,
                        void (*visita)(size_t, void *), void *ctx) {
    size_t n = strlen(texto);
    size_t m = strlen(padrao);
    if (m == 0)
        return 0;

    size_t buf[128];
    int no_heap;
    size_t *tabela = montar_tabela(padrao, m, buf, 128, &no_heap);
    if (tabela == NULL)
        return 0;

    size_t ocorrencias = 0;
    size_t k = 0;                 /* caracteres do padrão já casados */
    for (size_t i = 0; i < n; i++) {
        while (k > 0 && texto[i] != padrao[k])
            k = tabela[k - 1];
        if (texto[i] == padrao[k])
            k++;
        if (k == m) {
            if (visita != NULL)
                visita(i - m + 1, ctx);
            ocorrencias++;
            k = tabela[k - 1];   /* segue, permitindo ocorrências sobrepostas */
        }
    }

    if (no_heap)
        free(tabela);
    return ocorrencias;
}
