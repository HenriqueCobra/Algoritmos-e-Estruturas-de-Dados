# Counting sort

Não compara elementos. Descobre o menor e o maior valor, conta quantas vezes
cada valor aparece num vetor de contagens e depois reescreve o vetor original
percorrendo as contagens em ordem.

```
entrada:  3 -1 0 3 2 -1 1        (min = -1, max = 3)
contagem: [-1]=2 [0]=1 [1]=1 [2]=1 [3]=2
saida:    -1 -1 0 1 2 3 3
```

Funciona só para inteiros (ou chaves mapeáveis para inteiros) numa faixa
conhecida. O custo e a memória são O(n + k) com k = max - min + 1, então é
excelente quando os valores são densos (ex.: notas de 0 a 100) e péssimo quando
são esparsos (ex.: alguns inteiros de 32 bits → k na casa dos bilhões).

Esta versão reconstrói o vetor a partir das contagens, o que é simples mas **não
preserva estabilidade**. A variante estável usa soma de prefixos das contagens e
um vetor de saída separado — é essa forma que o radix sort utiliza como passo
interno.

## Complexidade

| Caso  | Tempo    | Espaço |
|-------|----------|--------|
| Todos | O(n + k) | O(k)   |

## Uso

```c
int v[] = {3, 1, 0, 2, 1};
counting_sort(v, 5);   // {0, 1, 1, 2, 3}
```
