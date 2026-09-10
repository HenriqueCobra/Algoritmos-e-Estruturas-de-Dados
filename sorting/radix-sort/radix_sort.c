#include "radix_sort.h"

#include <stdint.h>
#include <stdlib.h>

#define BITS_POR_PASSO 8
#define BASE          (1u << BITS_POR_PASSO)   /* 256 */
#define MASCARA       (BASE - 1u)

_Static_assert(sizeof(int) == 4, "esta implementacao supoe int de 32 bits");

void radix_sort(int *v, size_t n) {
    if (n < 2)
        return;

    uint32_t *origem = malloc(n * sizeof *origem);
    uint32_t *destino = malloc(n * sizeof *destino);
    if (origem == NULL || destino == NULL) {
        free(origem);
        free(destino);
        return;
    }

    /* Inverter o bit de sinal transforma o int com sinal numa chave sem sinal
       que preserva a ordem: INT_MIN vira 0, -1 vira 0x7FFFFFFF, 0 vira
       0x80000000, INT_MAX vira 0xFFFFFFFF. */
    for (size_t i = 0; i < n; i++)
        origem[i] = (uint32_t)v[i] ^ 0x80000000u;

    for (int shift = 0; shift < 32; shift += BITS_POR_PASSO) {
        size_t cont[BASE] = {0};

        for (size_t i = 0; i < n; i++)
            cont[(origem[i] >> shift) & MASCARA]++;

        /* Soma de prefixos: cont[d] passa a ser o índice inicial do dígito d. */
        size_t soma = 0;
        for (unsigned d = 0; d < BASE; d++) {
            size_t atual = cont[d];
            cont[d] = soma;
            soma += atual;
        }

        /* Distribui de forma estável na ordem de entrada. */
        for (size_t i = 0; i < n; i++) {
            unsigned d = (origem[i] >> shift) & MASCARA;
            destino[cont[d]++] = origem[i];
        }

        uint32_t *tmp = origem;
        origem = destino;
        destino = tmp;
    }

    for (size_t i = 0; i < n; i++)
        v[i] = (int)(origem[i] ^ 0x80000000u);

    free(origem);
    free(destino);
}
