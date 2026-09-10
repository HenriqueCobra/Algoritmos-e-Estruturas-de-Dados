# Árvore B*

Variação da árvore B que mantém os nós (fora a raiz) **pelo menos 2/3 cheios**,
em vez de 1/2. Com nós mais cheios, a árvore tem menos nós e menos níveis — para
as mesmas 5000 chaves deste teste, a árvore B fica com altura 7 e a B* com 5.

O preço é uma manutenção mais elaborada: em vez de dividir/fundir assim que um
nó sai do limite, primeiro tenta-se **redistribuir** com um irmão.

## Inserção

Nó transbordou (`MAXK+1` chaves):

- **irmão com espaço** → redistribui: junta os dois nós mais a chave separadora,
  divide de novo pela metade e devolve a chave do meio ao pai (2 → 2).
- **irmão também cheio** → divisão **2 → 3**: os dois nós cheios mais o
  separador viram três nós, cada um 2/3 cheio, e duas chaves sobem ao pai.

A raiz pode chegar a `2·MINK` chaves antes de se dividir; aí a divisão gera duas
metades que já nascem no mínimo de 2/3.

## Remoção

Nó ficou com `MINK-1` chaves:

- **irmão com sobra** → empresta (2 → 2).
- **irmãos no limite** → fusão **3 → 2**: três nós pouco cheios mais dois
  separadores viram dois nós cheios, e uma chave desce do pai.
- **raiz com só dois filhos** → funde num único nó, que vira a nova raiz.

## Parâmetros e custos

`MAXK = 6`, `MINK = 4` (≈ 2/3). Ajustáveis no `.c`, respeitando `MINK ≤ ⌊2·MAXK/3⌋`.

| Operação                | Custo    |
|-------------------------|----------|
| buscar / inserir / remover | O(log n) |

## Implementação

Todo o rebalanceamento — redistribuir 2→2, dividir 2→3, fundir 3→2 e colapsar
2→1 — passa por uma única rotina `juntar_dividir(x, i0, m_in, parts)`, que
concatena `m_in` filhos consecutivos e os reescreve como `parts` nós.
`bstar_valida` confere os limites de chaves por nó, a ordem e o nível uniforme
das folhas; os testes a chamam após cada operação (400 mil, no teste
aleatório).

## Uso

```c
ArvoreBStar *t = bstar_criar();
bstar_inserir(t, 42);
bstar_contem(t, 42);   // 1
bstar_remover(t, 42);
bstar_destruir(t);
```
