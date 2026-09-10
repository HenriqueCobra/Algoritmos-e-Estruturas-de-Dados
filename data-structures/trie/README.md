# Trie (árvore de prefixos)

Guarda um conjunto de palavras de forma que prefixos comuns são compartilhados.
Cada aresta é uma letra; o caminho da raiz até um nó soletra um prefixo. Nós
marcados com `fim_de_palavra` indicam que aquele prefixo também é uma palavra
inteira.

```
        (raiz)
        /     \
       c       s
       |       |
       a       o
      / \      |
     s   r     l*
    /|   |
   a r   r
  /* |*  |*
 l*  o*  o*        (cada * é fim de palavra: casa, casal, casar, caso, carro, sol)
```

Aqui o alfabeto é `a`–`z` (26 filhos por nó). Para texto qualquer, usa-se 256
filhos ou um mapa por nó.

## Custos

| Operação                       | Custo  |
|--------------------------------|--------|
| inserir / buscar / remover palavra de tamanho m | O(m) |
| existe prefixo de tamanho m    | O(m)   |

O custo **não depende** de quantas palavras estão guardadas — diferente de uma
BST de strings, onde cada comparação já é O(m) e há O(log n) delas.

## Remoção

Desmarca-se o `fim_de_palavra` do último nó. Na volta da recursão, cada nó que
ficou **sem filhos e sem marca** é liberado — assim remover `"carro"` de um
conjunto que não tem mais nada com prefixo `"car"` recupera a memória do galho.
A raiz nunca é liberada.

## Uso

```c
Trie *t = trie_criar();
trie_inserir(t, "casa");
trie_inserir(t, "casal");

trie_contem(t, "casa");         // 1
trie_tem_prefixo(t, "cas");     // 1

trie_destruir(t);
```
