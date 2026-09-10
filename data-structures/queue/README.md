# Fila (queue)

Estrutura **FIFO** (*first in, first out*): insere numa ponta (o fim) e remove na
outra (a frente). É o que modela filas de espera, buffers de produtor/consumidor
e a travessia em largura (BFS) de grafos.

```
desenfileira <- [3] <- [4] <- [5] <- enfileira
                ^              ^
              frente          fim
```

## Implementação

Lista encadeada com ponteiros para a **frente** e o **fim**:

- `enfileirar`: cria um nó e liga em `fim->prox`.
- `desenfileirar`: solta o nó da frente e avança `frente`.
- quando o último elemento sai, `frente` e `fim` voltam a `NULL` juntos.

Todas as operações são **O(1) no pior caso**, sem realocação.

| Operação         | Custo |
|------------------|-------|
| `enfileirar`     | O(1)  |
| `desenfileirar`  | O(1)  |
| `fila_frente`    | O(1)  |

## Alternativa

Com vetor, a fila vira um **buffer circular**: dois índices (`ini`, `fim`) que
avançam módulo a capacidade, evitando deslocar elementos a cada remoção. Ganha
em localidade de cache; em troca precisa redimensionar quando enche.

## Uso

```c
Fila *f = fila_criar();
fila_enfileirar(f, 1);
fila_enfileirar(f, 2);

int x;
fila_desenfileirar(f, &x);   // x == 1

fila_destruir(f);
```
