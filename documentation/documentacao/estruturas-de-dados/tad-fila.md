---
description: >-
  Estrutura de dados linear baseada na política FIFO (First In, First Out), em
  que o primeiro elemento inserido é o primeiro a ser removido.
icon: rectangle-history
---

# TAD Fila

Esta implementação foi desenvolvida utilizando um vetor circular dinâmico com redimensionamento automático de capacidade. O TAD é totalmente genérico e desacoplado do tipo do dado, manipulando elementos do tipo ponteiro para `ITEM`. No âmbito do projeto, a fila pode ser selecionada dinamicamente via linha de comandos tanto como estrutura principal de armazenamento (`--base fila`) quanto como estrutura de resultados (`--result fila`).

### Estrutura de Dados e Encapsulamento

A representação concreta das estruturas encontra-se oculta no ficheiro de implementação `fila.c`, garantindo o encapsulamento e impedindo o acesso direto aos campos internos por parte do cliente.

C

```
#define CAPACIDADE_INICIAL 100

struct fila_ {
    ITEM **itens;
    int inicio;
    int fim;
    int tamanho;
    int capacidade;
};
```

* `itens`: Ponteiro para um vetor de ponteiros para `ITEM`, alocado dinamicamente na memória.
* `inicio`: Índice que aponta para a posição do primeiro elemento (cabeça da fila).
* `fim`: Índice que aponta para a próxima posição livre de inserção.
* `tamanho`: Contador de elementos armazenados na fila.
* `capacidade`: Capacidade total alocada para o vetor circular.

#### Redimensionamento Dinâmico

Quando o número de elementos atinge a capacidade total do vetor (`tamanho == capacidade`), a função interna `fila_redimensionar` é acionada. A capacidade do vetor é dobrada e os elementos são reorganizados a partir do índice `0`, mantendo a integridade do circuito circular e evitando a fragmentação do espaço alocado.

### Resumo de Complexidade Assintótica

| **Operação**         | **Função**       | **Complexidade de Tempo** | **Complexidade de Espaço** |
| -------------------- | ---------------- | ------------------------- | -------------------------- |
| Criar Fila           | `fila_criar`     | $$O(1)$$                  | $$O(1)$$                   |
| Enfileirar (Inserir) | `fila_inserir`   | $$O(1)$$ amortizado       | $$O(1)$$                   |
| Remover              | `fila_remover`   | $$O(1)$$                  | $$O(1)$$                   |
| Destruir             | `fila_apagar`    | $$O(N)$$                  | $$O(1)$$                   |
| Consultar Tamanho    | `fila_tamanho`   | $$O(1)$$                  | $$O(1)$$                   |
| Verificar Vazia      | `fila_vazia`     | $$O(1)$$                  | $$O(1)$$                   |
| Percorrer            | `fila_percorrer` | $$O(N)$$                  | $$O(1)$$                   |

> $$N$$ representa o número de itens armazenados na fila no momento da chamada. A inserção tem custo de tempo $$O(1)$$ amortizado devido à estratégia de duplicar a capacidade quando o vetor é preenchido ($$O(N)$$ no pior caso de realocação).

### Referência da API

#### `fila_criar`

Cria e inicializa uma nova instância da fila na memória com capacidade inicial de 100 elementos.

```c
FILA *fila_criar(void);
```

* Parâmetros: Nenhum.
* Retorno: Ponteiro para a estrutura `FILA` alocada ou `NULL` em caso de falha de alocação de memória.
* Complexidade: $$O(1)$$.

#### `fila_inserir`

Insere (enfileira) um elemento no fim da fila. O ponteiro de entrada é do tipo `void *` para manter compatibilidade genérica com a arquitetura de ponteiros do projeto.

```c
bool fila_inserir(void *fila_ptr, ITEM *item);
```

* Parâmetros:
  * `fila_ptr`: Ponteiro para a instância da fila (conversão interna para `FILA *`).
  * `item`: Ponteiro para o elemento `ITEM` a ser armazenado.
* Retorno: `true` se a inserção for realizada com sucesso; `false` se qualquer ponteiro de entrada for inválido (`NULL`) ou se o redimensionamento falhar.
* Complexidade: $$O(1)$$ amortizado.

#### `fila_remover`

Remove e retorna o elemento localizado no início da fila (ordem FIFO).

```c
ITEM *fila_remover(FILA *fila);
```

* Parâmetros:
  * `fila`: Ponteiro para a fila.
* Retorno: Ponteiro para o `ITEM` removido, ou `NULL` caso a fila seja nula ou esteja vazia.
* Complexidade: $$O(1)$$.

#### `fila_apagar`

Desaloca a estrutura da fila e o vetor de ponteiros. Permite ao cliente escolher se a memória dos elementos `ITEM` também deve ser liberada.

```c
void fila_apagar(FILA **fila, bool apagar_itens);
```

* Parâmetros:
  * `fila`: Endereço do ponteiro para a fila (`FILA **`). Ao final da execução, `*fila` é definido como `NULL`.
  * `apagar_itens`: Booleano que define se a função `item_apagar()` será invocada para liberar cada `ITEM` restante.
* Complexidade: $$O(N)$$, onde $$N$$ é a quantidade de elementos armazenados na fila.

#### `fila_tamanho`

Retorna a quantidade total de elementos presentes na fila.

```c
int fila_tamanho(const FILA *fila);
```

* Parâmetros:
  * `fila`: Ponteiro constante para a fila.
* Retorno: Valor inteiro indicando o número de itens na fila. Retorna `0` se a fila for `NULL`.
* Complexidade: $$O(1)$$.

#### `fila_vazia`

Verifica se a fila está vazia.

```c
bool fila_vazia(const FILA *fila);
```

* Parâmetros:
  * `fila`: Ponteiro constante para a fila.
* Retorno: `true` se a fila for nula ou se o número de itens for zero; `false` caso contrário.
* Complexidade: $$O(1)$$.

#### `fila_percorrer`

Percorre a fila da cabeça até ao fim (ordem FIFO), aplicando uma função de _callback_ a cada elemento armazenado. Esta função é essencial para as operações de busca no dataset e de gravação dos resultados no ficheiro CSV.

```c
void fila_percorrer(const FILA *fila, void (*visitar)(ITEM *item, void *contexto), void *contexto);
```

* Parâmetros:
  * `fila`: Ponteiro constante para a fila.
  * `visitar`: Ponteiro de função que recebe um `ITEM *` e um ponteiro genérico de `contexto`.
  * `contexto`: Ponteiro genérico passado para a função `visitar` (ex.: ficheiro de saída, contador, acumulador).
* Complexidade: $$O(N)$$.

### Exemplo de Uso

C

```c
#include <stdio.h>
#include "fila.h"
#include "item.h"

// Função de callback para exibição dos itens
void imprimir_item(ITEM *item, void *contexto) {
    if (item != NULL) {
        item_imprimir(item);
    }
}

int main(void) {
    // 1. Criação da fila
    FILA *f = fila_criar();
    if (f == NULL) return 1;

    // 2. Inserção (Enfileirar)
    ITEM *item1 = item_criar(1, "Asteroide A");
    ITEM *item2 = item_criar(2, "Asteroide B");

    fila_inserir(f, item1);
    fila_inserir(f, item2);

    printf("Tamanho da fila: %d\n", fila_tamanho(f)); // Saída: 2

    // 3. Percurso (Início -> Fim em ordem FIFO)
    printf("Conteúdo da fila:\n");
    fila_percorrer(f, imprimir_item, NULL);

    // 4. Remoção (Desenfileirar - Ordem FIFO)
    ITEM *removido = fila_remover(f);
    if (removido != NULL) {
        printf("Item removido do início: ");
        item_imprimir(removido);
        item_apagar(&removido);
    }

    // 5. Destruição da fila e limpeza da memória restante
    fila_apagar(&f, true);

    return 0;
}
```
