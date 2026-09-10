#ifndef KMP_H
#define KMP_H

#include <stddef.h>

/* Busca de padrão em texto pelo algoritmo de Knuth-Morris-Pratt.

   A busca ingênua, ao falhar numa posição, volta o texto atrás e recomeça —
   O(n·m) no pior caso. O KMP pré-computa, para cada prefixo do padrão, o
   tamanho do maior prefixo que também é sufixo dele (a "tabela de falha").
   Com ela, ao falhar depois de casar k caracteres, sabe-se que os próximos
   `tabela[k-1]` já casam, então o texto nunca retrocede: O(n + m). */

/* Preenche `tabela` (tamanho m) com a função de falha de `padrao`.
   tabela[i] = comprimento do maior prefixo próprio de padrao[0..i] que é
   também sufixo de padrao[0..i]. */
void kmp_tabela(const char *padrao, size_t m, size_t *tabela);

/* Índice da primeira ocorrência de `padrao` em `texto`, ou -1.
   Um padrão vazio casa na posição 0. */
long kmp_buscar(const char *texto, const char *padrao);

/* Chama `visita(pos, ctx)` para cada ocorrência (inclusive sobrepostas) e
   retorna quantas foram. `visita` pode ser NULL (só conta). */
size_t kmp_buscar_todas(const char *texto, const char *padrao,
                        void (*visita)(size_t pos, void *ctx), void *ctx);

#endif
