# Deque

*Double-ended queue* — fila em que as duas pontas aceitam inserção e remoção.
Generaliza pilha (só uma ponta) e fila (insere numa, remove na outra).

## Buffer circular

Os elementos ficam num vetor, mas a "frente" não é fixa no índice 0: guardamos
`inicio` (índice do primeiro elemento) e `tamanho`. O fim lógico está em
`(inicio + tamanho) % capacidade`. Assim, inserir no início é só recuar `inicio`
dando a volta no vetor — sem deslocar nada.

```
capacidade 8, inicio = 6, tamanho = 4

índice:  0   1   2   3   4   5   6   7
        [C] [D]  .   .   .   .  [A] [B]
         \_______ lógico: A B C D _______/
```

Quando `tamanho == capacidade`, alocamos um vetor com o dobro do tamanho e
copiamos os elementos já na ordem lógica, com a frente voltando ao índice 0.

## Custos

| Operação                    | Custo           |
|-----------------------------|-----------------|
| inserir/remover em qualquer ponta | O(1) amortizado |
| ler qualquer ponta          | O(1)            |

O "amortizado" vem do mesmo argumento da pilha: a duplicação da capacidade
dilui o custo das cópias.

## Uso

```c
Deque *d = deque_criar();
deque_inserir_fim(d, 1);
deque_inserir_inicio(d, 0);   // 0 1

int x;
deque_remover_fim(d, &x);      // x == 1

deque_destruir(d);
```
