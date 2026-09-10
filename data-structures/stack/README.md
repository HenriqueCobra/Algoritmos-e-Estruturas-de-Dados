# Pilha (stack)

Estrutura **LIFO** (*last in, first out*): só dá para inserir e remover numa
ponta, o "topo". É o que modela chamadas de função, desfazer/refazer, avaliação
de expressões e travessias em profundidade.

Esta implementação usa um **vetor dinâmico**: quando o vetor enche, um `realloc`
dobra a capacidade.

## Operações

| Operação      | Custo             |
|---------------|-------------------|
| `empilhar`    | O(1) amortizado   |
| `desempilhar` | O(1)              |
| `topo`        | O(1)              |

### Por que "amortizado"

Um `empilhar` isolado pode custar O(n), quando dispara a cópia do vetor. Mas
como a capacidade **dobra**, essas cópias ficam cada vez mais raras: inserir n
elementos faz no total ~2n cópias, ou seja O(1) por operação na média. Dobrar
(e não somar um valor fixo) é justamente o que garante isso.

## Alternativa

Dá para implementar a pilha com lista encadeada, empilhando no início: cada
operação é O(1) no pior caso e não há desperdício de capacidade, mas paga-se um
ponteiro por elemento e o acesso à memória fica menos amigável à cache.

## Uso

```c
Pilha *p = pilha_criar();
pilha_empilhar(p, 10);
pilha_empilhar(p, 20);

int x;
pilha_desempilhar(p, &x);   // x == 20

pilha_destruir(p);
```
