#include "insertion_sort.h"

void insertion_sort(int *v, size_t n) {
    for (size_t i = 1; i < n; i++) {
        int chave = v[i];

        /* Desloca para a direita todo mundo em v[0..i) que é maior que a
           chave, abrindo o buraco onde ela entra. */
        size_t j = i;
        while (j > 0 && v[j - 1] > chave) {
            v[j] = v[j - 1];
            j--;
        }
        v[j] = chave;
    }
}
