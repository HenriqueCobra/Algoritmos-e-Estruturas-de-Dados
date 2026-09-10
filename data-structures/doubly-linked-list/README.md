# Lista duplamente encadeada

Cada nó guarda ponteiros para o **anterior** e o **próximo**. Isso permite
caminhar nos dois sentidos e, principalmente, remover um nó em O(1) quando já se
tem o ponteiro para ele — sem precisar procurar o antecessor, como na lista
simples.

```
NULL <- [1] <-> [2] <-> [3] -> NULL
         ^               ^
       inicio           fim
```

## Operações

| Operação                     | Custo    |
|------------------------------|----------|
| inserir/remover nas pontas   | O(1)     |
| inserir/remover num nó dado  | O(1)     |
| acessar posição i            | O(min(i, n−i)) |
| buscar por valor             | O(n)     |

O acesso por índice caminha a partir da ponta mais próxima (`no_em`), então
chegar ao meio custa ~n/2 em vez de n.

## Detalhes de implementação

- Toda remoção passa por `desligar`, que religa os vizinhos e cuida dos casos de
  ponta (nó no início, no fim, ou nó único) atualizando `inicio`/`fim`.
- Tipo opaco: `struct no` e `struct lista_dupla` vivem no `.c`.
- Sem nó sentinela — as pontas são `NULL`. Um sentinela deixaria `desligar` sem
  os `if` de borda, ao custo de um nó extra e de código menos óbvio para quem
  está estudando.

## Uso

```c
ListaDupla *l = lista_dupla_criar();
lista_dupla_inserir_fim(l, 1);
lista_dupla_inserir_fim(l, 2);

int x;
lista_dupla_remover_fim(l, &x);   // x == 2

lista_dupla_destruir(l);
```
