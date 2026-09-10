#include "bubble_sort.h"

void bubble_sort(int *v, size_t n) {
    if (n < 2)
        return;

    /* Cada passada empurra o maior elemento restante para o fim. O índice da
       última troca marca a partir de onde o vetor já está ordenado, então a
       próxima passada pode parar ali. Quando não há trocas, terminou. */
    size_t fim = n - 1;
    while (fim > 0) {
        size_t ultima_troca = 0;
        for (size_t i = 0; i < fim; i++) {
            if (v[i] > v[i + 1]) {
                int tmp = v[i];
                v[i] = v[i + 1];
                v[i + 1] = tmp;
                ultima_troca = i;
            }
        }
        fim = ultima_troca;
    }
}
