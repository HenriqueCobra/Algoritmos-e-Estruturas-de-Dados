# Árvore binária de busca (BST)

Cada nó tem uma chave, um filho esquerdo e um direito. A **invariante** é: para
todo nó, todas as chaves da subárvore esquerda são menores e todas as da direita
são maiores. Um percurso *em ordem* (esquerda, nó, direita) visita as chaves em
ordem crescente.

```
        50
       /  \
     30    70
    /  \   / \
   20  40 60  80
```

## Operações

| Operação            | Custo       |
|---------------------|-------------|
| inserir / buscar / remover | O(altura) |
| mínimo / máximo     | O(altura)   |
| percurso em ordem   | O(n)        |

`altura` é O(log n) numa árvore equilibrada, mas O(n) no pior caso — inserir
`1, 2, 3, 4, 5` produz uma árvore degenerada em lista. É esse problema que a
**AVL** e a **rubro-negra** resolvem, mantendo a altura em O(log n) por meio de
rotações.

## Remoção

Três casos para o nó removido:

1. **Sem filhos** — some.
2. **Um filho** — é substituído pelo filho.
3. **Dois filhos** — copia-se a chave do **sucessor em ordem** (o menor da
   subárvore direita) para o nó, e então remove-se o sucessor, que cai no caso 1
   ou 2.

## Uso

```c
BST *t = bst_criar();
bst_inserir(t, 50);
bst_inserir(t, 30);
bst_inserir(t, 70);

bst_contem(t, 30);     // 1
bst_remover(t, 50);

bst_destruir(t);
```
