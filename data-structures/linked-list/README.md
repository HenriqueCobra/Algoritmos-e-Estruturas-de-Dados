# Lista simplesmente encadeada

Sequência de nós em que cada um guarda um valor e um ponteiro para o próximo. O
último aponta para `NULL`.

```
inicio -> [10|-] -> [20|-] -> [30|/]
                                 ^
                                fim
```

A estrutura mantém ponteiros para o **início** e o **fim** e o **tamanho**
atual. Com o ponteiro de fim, inserir no fim é O(1); sem ele seria O(n).

## Operações

| Operação                         | Custo |
|----------------------------------|-------|
| inserir no início / no fim       | O(1)  |
| inserir / ler / remover na posição i | O(i)  |
| buscar por valor                 | O(n)  |
| remover por valor                | O(n)  |
| inverter                         | O(n)  |

Remover no fim é O(n) mesmo com o ponteiro de fim, porque é preciso achar o
penúltimo nó — essa é a limitação que a lista **duplamente** encadeada resolve.

## Detalhes de implementação

- `struct no` e `struct lista` ficam no `.c`: o cabeçalho expõe só o tipo opaco
  `Lista`, então o código cliente não depende do layout interno.
- Falha de `malloc` na inserção é tratada sem alterar a lista.
- `lista_destruir` libera todos os nós e depois a estrutura.
- Guarda `int`; para outros tipos, troque o campo `valor` (ou parametrize com
  um `typedef`).

## Uso

```c
Lista *l = lista_criar();
lista_inserir_fim(l, 10);
lista_inserir_inicio(l, 5);       // 5 -> 10

int x;
lista_remover_inicio(l, &x);      // x == 5

lista_destruir(l);
```
