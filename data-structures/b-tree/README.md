# Árvore B

Generaliza a árvore de busca balanceada para nós **largos**: cada nó guarda um
bloco de chaves ordenadas e tem um filho entre cada par de chaves (e nas pontas).
Foi criada para índices em disco — se cada nó é um bloco lido de uma vez, uma
árvore de milhões de chaves tem só 3 ou 4 níveis, ou seja 3–4 leituras.

```
grau mínimo t = 3  →  cada nó tem de 2 a 5 chaves e de 3 a 6 filhos

                 [ 17 ]
               /        \
        [ 5  11 ]       [ 23  29  37 ]
       /   |    \        /   |    |   \
     ...  ...  ...     ...  ... ...  ...
```

## Invariantes

- Todo nó, menos a raiz, tem entre `t-1` e `2t-1` chaves.
- Um nó com `k` chaves tem exatamente `k+1` filhos (se não for folha).
- **Todas as folhas estão na mesma profundidade** — é isso que mantém a árvore
  balanceada, sem cores nem fatores de balanceamento.

## Inserção (split proativo)

Descendo em direção à folha, todo nó **cheio** (`2t-1` chaves) que aparece no
caminho é dividido antes de continuar: a chave do meio sobe para o pai e o nó
vira dois. Assim, quando se chega à folha, sempre há espaço, e a divisão nunca
precisa propagar para cima. A árvore só cresce em altura quando a **raiz** é
dividida.

## Remoção

Antes de descer para um filho, garante-se que ele tenha ao menos `t` chaves:

- **irmão com sobra** → rotação: uma chave do pai desce, uma do irmão sobe;
- **nenhum irmão com sobra** → funde o filho, uma chave do pai e um irmão num nó
  só (`2t-1` chaves).

Remover uma chave de um nó interno a troca pelo antecessor ou sucessor (que está
numa folha), reduzindo ao caso da folha.

## Custos

| Operação                | Comparações       | Nós visitados |
|-------------------------|-------------------|---------------|
| buscar / inserir / remover | O(t · log_t n) | O(log_t n)    |

## Implementação

- Grau mínimo `T = 3` (ajustável no `.c`). Nós são arrays de tamanho fixo
  `2T-1` / `2T`.
- `btree_valida` confere os limites de chaves, a ordem e o nível uniforme das
  folhas; os testes a chamam após cada operação.

## Uso

```c
ArvoreB *t = btree_criar();
btree_inserir(t, 42);
btree_contem(t, 42);    // 1
btree_remover(t, 42);
btree_destruir(t);
```
