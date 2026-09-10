# Árvore rubro-negra

BST balanceada por **cor**. Cada nó é vermelho ou preto e o balanceamento vem de
quatro regras:

1. A raiz é preta.
2. Toda folha (o nó sentinela `nil`) é preta.
3. Um nó vermelho não pode ter filho vermelho.
4. Todo caminho de um nó até as folhas descendentes tem o mesmo número de nós
   pretos (a "altura preta").

As regras 3 e 4 juntas garantem que o caminho mais longo da raiz a uma folha tem
no máximo **o dobro** do mais curto — logo a altura é O(log n).

## Inserção

Insere-se como numa BST, pintando o novo nó de **vermelho** (para não violar a
regra 4). Se o pai também for vermelho, `inserir_fixup` conserta subindo pela
árvore, em três casos por lado:

- **tio vermelho** → recolore pai, tio e avô e sobe;
- **tio preto, nó "para dentro"** → rotação para virar o caso seguinte;
- **tio preto, nó "para fora"** → recolore e uma rotação no avô, e acabou.

## Remoção

Remove-se como numa BST (substituindo por sucessor quando há dois filhos). Se o
nó que saiu era **preto**, alguma altura preta diminuiu: `remover_fixup` empurra
esse "preto a mais" para cima com recolorações e até três rotações, até absorvê-lo
num nó vermelho ou na raiz.

## Custos

| Operação                    | Custo    | Rotações (pior caso) |
|-----------------------------|----------|----------------------|
| buscar                      | O(log n) | 0                    |
| inserir                     | O(log n) | ≤ 2                  |
| remover                     | O(log n) | ≤ 3                  |

Comparada à AVL, é menos equilibrada (buscas ligeiramente mais lentas) mas faz
menos rotação — por isso é a preferida de bibliotecas padrão (`std::map`,
`TreeMap`).

## Implementação

- Segue o *Introduction to Algorithms* (CLRS): nó sentinela `nil` único no lugar
  de todos os ponteiros nulos, o que elimina testes de `NULL` nas rotações e nos
  fixups, e ponteiro `pai` em cada nó.
- `rb_valida` confere as quatro regras + a ordem de BST; os testes a chamam após
  cada operação.

## Uso

```c
ArvoreRB *t = rb_criar();
rb_inserir(t, 10);
rb_inserir(t, 20);
rb_inserir(t, 30);      // rebalanceia sozinha

rb_contem(t, 20);       // 1
rb_remover(t, 10);

rb_destruir(t);
```
