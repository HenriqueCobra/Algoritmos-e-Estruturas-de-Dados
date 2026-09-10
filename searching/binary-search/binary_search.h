#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <stddef.h>

/* Busca binária: acha um elemento num vetor JÁ ORDENADO (crescente) cortando o
   espaço de busca pela metade a cada passo. O(log n) comparações.

   Além do "achou ou não", as duas variantes de fronteira (as mesmas do
   lower_bound / upper_bound do C++) resolvem "quantos elementos são < x?",
   "onde inserir x mantendo a ordem?", buscas por intervalo, etc. */

/* Índice de alguma ocorrência de `alvo`, ou -1 se não existe.
   Se houver repetições, qualquer um dos índices pode ser devolvido. */
long busca_binaria(const int *v, size_t n, int alvo);

/* Primeiro índice i com v[i] >= alvo (ou n se todos são menores).
   É também o número de elementos estritamente menores que `alvo`. */
size_t limite_inferior(const int *v, size_t n, int alvo);

/* Primeiro índice i com v[i] > alvo (ou n).
   limite_superior - limite_inferior = quantidade de cópias de `alvo`. */
size_t limite_superior(const int *v, size_t n, int alvo);

#endif
