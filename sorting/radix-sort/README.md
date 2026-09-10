# Radix sort

Ordena números olhando um "dígito" de cada vez, sem nunca comparar dois
elementos entre si. Esta versão é **LSD** (least significant digit): começa pelo
byte menos significativo e vai até o mais significativo, aplicando em cada
passada um counting sort **estável**.

A estabilidade é o que faz funcionar: ao ordenar pelo byte `k`, a ordem já
estabelecida pelos bytes `0..k-1` é preservada nos empates.

```
byte 0:  ... ordena pelas unidades
byte 1:  ... ordena pelas centenas (mantendo o critério anterior nos empates)
byte 2, byte 3: idem
```

Com base 256, um `int` de 32 bits precisa de exatamente 4 passadas.

## Negativos

Counting sort trabalha com índices não negativos. Em vez de separar os sinais,
invertemos o bit mais significativo de cada valor (`x ^ 0x80000000`): isso mapeia
o intervalo com sinal `INT_MIN..INT_MAX` para `0..UINT32_MAX` **preservando a
ordem**. No fim, desfazemos a transformação.

## Complexidade

Para `w` passadas (aqui `w = 4`) e base `b`:

| Tempo        | Espaço   |
|--------------|----------|
| O(w·(n + b)) | O(n + b) |

Como `w` e `b` são constantes, na prática é O(n). Estável.

## Uso

```c
int v[] = {170, -45, 75, 90, 2, 24};
radix_sort(v, 6);   // {-45, 2, 24, 75, 90, 170}
```
