# Tabela hash

Dicionário que associa **chaves** a **valores** com custo O(1) em média. Aqui a
chave é `string` e o valor é `int`.

## Como funciona

1. Uma função de hash transforma a chave num número.
2. `número % n_baldes` dá o índice do **balde** onde o par vai.
3. Chaves diferentes podem cair no mesmo balde (**colisão**).

### Tratamento de colisão: encadeamento separado

Cada balde é uma lista ligada de pares. Buscar/inserir/remover percorre só a
lista daquele balde. Se o hash espalha bem e o número de pares por balde é
pequeno, essas listas têm ~1 elemento.

```
baldes
  0 -> ("tres", 3)
  1 -> NULL
  2 -> ("um", 1) -> ("dez", 10)      <- colisão no balde 2
  3 -> ("dois", 22)
```

### Fator de carga e reespalhamento

Fator de carga = pares / baldes. Quando passa de **0,75**, o número de baldes
dobra e todos os pares são redistribuídos. Isso mantém as listas curtas; o custo
O(n) do reespalhamento é raro e se dilui (amortizado O(1) por inserção).

### Função de hash

`FNV-1a`: parte de uma constante, e para cada byte faz `xor` seguido de
multiplicação por um primo. Simples, rápida e com dispersão boa o suficiente
para chaves curtas. (Não é resistente a colisões adversárias — para isso usa-se
algo com semente aleatória, como SipHash.)

## Custos

| Operação            | Médio | Pior (todas as chaves no mesmo balde) |
|---------------------|-------|---------------------------------------|
| `por` / `obter` / `remover` | O(1) | O(n) |

## Detalhes de implementação

- A chave é **copiada** (`copia_str`) na inserção e liberada na remoção/destruição.
- `tabela_por` numa chave existente **atualiza** o valor, não cria duplicata.
- Alternativa a encadeamento: **endereçamento aberto** (guardar os pares no
  próprio vetor de baldes e, na colisão, procurar outro slot). Usa menos memória
  e é mais amigável à cache, mas a remoção fica mais delicada.

## Uso

```c
TabelaHash *t = tabela_criar();
tabela_por(t, "idade", 30);

int v;
tabela_obter(t, "idade", &v);   // v == 30

tabela_destruir(t);
```
