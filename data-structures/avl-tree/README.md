# Árvore AVL

Uma BST que se mantém **equilibrada**. A invariante, verificada em todo nó, é:

> a altura da subárvore esquerda e a da direita diferem em no máximo 1.

Isso limita a altura a ~1,44·log₂(n), então buscas, inserções e remoções são
O(log n) **no pior caso** — sem o risco da BST comum de degenerar em lista.

## Rotações

Depois de inserir ou remover, sobe-se pelo caminho recalculando alturas. Se um
nó fica com fator de balanceamento +2 ou −2, uma ou duas rotações consertam:

```
  caso esquerda-esquerda            caso esquerda-direita
        z                                 z                    y
       / \        rot. dir. em z         / \     rot.esq(x)    / \
      y   T4      ------------->         x   T4   + rot.dir(z) x   z
     / \                               / \       --------->   /|   |\
    x   T3                            T1  y                   T1 T2 T3 T4
   / \                                   / \
  T1  T2                                T2  T3
```

Os casos direita-direita e direita-esquerda são espelhos desses. Cada rotação é
O(1) e ajusta 3 ponteiros; como no máximo O(log n) nós são revisitados, o
rebalanceamento não muda a complexidade.

## AVL x rubro-negra

Ambas garantem O(log n). A AVL é mais rigidamente equilibrada (buscas um pouco
mais rápidas), mas faz mais rotações em inserção/remoção. A rubro-negra relaxa o
equilíbrio em troca de menos rotações — por isso é a escolha de bibliotecas como
`std::map` e o `TreeMap` do Java.

## Uso

```c
AVL *t = avl_criar();
for (int i = 1; i <= 1000; i++)
    avl_inserir(t, i);          // continua com altura ~10, não 1000

avl_contem(t, 742);             // 1
avl_destruir(t);
```
