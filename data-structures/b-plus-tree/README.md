# Árvore B+

Uma árvore B com duas mudanças que a tornam o índice padrão de bancos de dados:

1. **Só as folhas guardam dados.** Os nós internos têm apenas chaves de
   roteamento (separadores) e ponteiros — servem só para achar a folha certa.
   Como cada chave interna é uma cópia, uma chave pode aparecer duas vezes: como
   separador e como dado na folha.
2. **As folhas formam uma lista encadeada.** Depois de descer até a primeira
   folha do intervalo, a varredura em ordem e as consultas por faixa
   (`WHERE x BETWEEN a AND b`) seguem os ponteiros `prox` sem voltar à árvore.

```
                 [ 17 | 34 ]
               /     |      \
      [5|11]      [23|29]      [40|52]
      /  |  \      /  |  \      /  |  \
   folhas ligadas: (2 3 5)->(11 13)->(17 19)->(23 27)->(29 31)->(34 37)->...
```

## Operações

| Operação                | Custo         |
|-------------------------|---------------|
| buscar / inserir / remover | O(log n)   |
| varrer tudo em ordem    | O(n), sem recursão |
| intervalo com k resultados | O(log n + k) |

## Inserção

Igual à árvore B, mas com *split* reativo: insere-se na folha e, se ela passa de
`ORDEM-1` chaves, divide-se em duas e **copia-se** a menor chave da metade
direita para o pai. Se o pai transborda, ele também se divide — aí a chave do
meio **sobe** (não é copiada, porque nós internos não guardam dados).

## Remoção

Ao remover de uma folha, se ela fica abaixo do mínimo:

- **irmão com sobra** → empresta uma chave e ajusta o separador no pai;
- **sem sobra** → funde as duas folhas e remove o separador do pai.

Um detalhe: um separador pode virar "fantasma" — continuar apontando o caminho
certo mesmo depois que a menor chave da subárvore à direita foi removida. Isso é
aceito; ele ainda roteia corretamente.

## Implementação

- `ORDEM = 5` (ajustável no `.c`).
- `bplus_intervalo` demonstra a vantagem da lista de folhas.
- `bplus_valida` confere limites por nó, nível uniforme das folhas e o
  intervalo `[separador_esq, separador_dir)` de cada subárvore; os testes a
  chamam após cada operação.

## Uso

```c
ArvoreBMais *t = bplus_criar();
for (int i = 0; i < 1000; i++)
    bplus_inserir(t, i);

long soma = 0;
bplus_intervalo(t, 100, 199, acumular, &soma);   /* 100..199 em O(log n + 100) */

bplus_destruir(t);
```
