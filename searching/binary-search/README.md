# Busca binária

Procura um valor num vetor **ordenado** comparando com o elemento do meio e
descartando metade do vetor a cada passo. O(log n).

```
procurar 13 em [1 3 3 3 5 8 13 21 34]
                         ^meio=5, 8 < 13  -> descarta a esquerda
                              [13 21 34]
                               ^meio, 13  -> achou
```

## As três variantes

Todas usam o mesmo esqueleto sobre o intervalo semiaberto `[lo, hi)`; muda só a
condição.

| Função             | Devolve                                    |
|--------------------|--------------------------------------------|
| `busca_binaria`    | um índice onde está `alvo`, ou -1          |
| `limite_inferior`  | 1º índice com `v[i] >= alvo` (= nº de elementos `< alvo`) |
| `limite_superior`  | 1º índice com `v[i] > alvo`                |

Com as fronteiras:

- **contar cópias** de `x`: `limite_superior(x) - limite_inferior(x)`;
- **ponto de inserção** mantendo a ordem: `limite_inferior(x)`;
- **contar no intervalo** `[a, b]`: `limite_superior(b) - limite_inferior(a)`.

## Cuidados de implementação

- `meio = lo + (hi - lo) / 2`, não `(lo + hi) / 2` — o segundo pode estourar
  quando os índices são grandes.
- Intervalo semiaberto e a invariante clara ("`hi` é sempre um índice já
  descartado ou o fim") evitam o off-by-one clássico e o laço infinito.

## Generalização

A busca binária funciona em qualquer predicado **monótono** `p` (falso, falso,
..., verdadeiro, ..., verdadeiro): ela acha a fronteira. Ordenação é só o caso
`p(i) = (v[i] >= alvo)`. É assim que se faz "busca binária na resposta".

## Uso

```c
int v[] = {1, 3, 5, 8, 13};
long i = busca_binaria(v, 5, 8);        // 3
size_t ins = limite_inferior(v, 5, 6);  // 3  (onde inserir o 6)
```
