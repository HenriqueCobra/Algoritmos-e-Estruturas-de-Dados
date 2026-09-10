# KMP — busca de padrão em texto

Acha as ocorrências de um padrão de tamanho `m` num texto de tamanho `n` em
**O(n + m)**, contra O(n·m) da busca ingênua.

## O problema da busca ingênua

Ao comparar o padrão a partir de uma posição e falhar no `k`-ésimo caractere, a
busca ingênua joga tudo fora e recomeça uma posição adiante — reexaminando
caracteres do texto que já tinha visto.

```
texto:   a a a a a b
padrão:  a a a b            falha no índice 3
         . a a a b          recomeça do zero, um passo à frente
```

## A tabela de falha

O KMP percebe que os `k` caracteres que já casaram **são** um pedaço do padrão.
Se um prefixo do padrão também é sufixo desse pedaço, ele já está alinhado com o
texto — não precisa recomparar.

`tabela[i]` = comprimento do maior prefixo próprio de `padrao[0..i]` que também é
sufixo dele.

```
padrão:  a b a b a c a
tabela:  0 0 1 2 3 0 1
```

Construir a tabela é o mesmo algoritmo da busca, do padrão contra ele mesmo:
O(m).

## A busca

Percorre o texto uma vez. Mantém `k` = quantos caracteres do padrão casam no
momento. Em uma falha, em vez de voltar no texto, faz `k = tabela[k-1]` (o texto
nunca retrocede — cada caractere é visitado O(1) vezes amortizado). Ao chegar em
`k == m`, registrou uma ocorrência e faz `k = tabela[k-1]` para continuar,
permitindo ocorrências **sobrepostas**.

## Uso

```c
long i = kmp_buscar("abcabcabd", "abcabd");     // 3

size_t total = kmp_buscar_todas("aaaa", "aa", NULL, NULL);   // 3 (sobrepostas)

size_t tab[6];
kmp_tabela("abcabd", 6, tab);
```
