---
description: >-
  Estrutura de dados linear contígua na memória baseada num vetor dinâmico de
  ponteiros.
icon: table-rows
---

# TAD Lista Sequencial

Diferente de estruturas encadeadas, a lista sequencial permite o acesso direto e aleatório às posições por índice em tempo constante $$ $O(1)$ $$. A alocação de memória expande-se automaticamente através da realocação dinâmica à medida que novos elementos são inseridos. O TAD manipula elementos genéricos do tipo ponteiro para `ITEM`. No contexto do projeto, pode ser selecionado via linha de comandos tanto para armazenar o dataset completo (`--base lista_seq`) como para a estrutura de resultados (`--result lista_seq`).

### Estrutura de Dados e Encapsulamento

A representação concreta da estrutura encontra-se oculta no ficheiro de implementação `lista_seq.c`, garantindo a abstração dos dados e impedindo o acesso direto aos campos internos por parte do cliente.

```c
#define CAPACIDADE_INICIAL 1000

struct lista_seq_ {
    ITEM **itens;
    int tamanho;
    int capacidade;
};
```

* `itens`: Ponteiro para um vetor de ponteiros para `ITEM`, alocado contiguamente na memória dinâmica.
* `tamanho`: Quantidade atual de elementos armazenados na lista.
* `capacidade`: Capacidade máxima atual do vetor alocado antes da necessidade de realocação.

#### Redimensionamento Dinâmico

Quando a quantidade de elementos atinge a capacidade limite do vetor (`tamanho == capacidade`), a função estática interna `lista_seq_redimensionar` é invocada. Esta operação utiliza `realloc` para duplicar a capacidade do vetor, garantindo um custo de inserção amortizado eficiente.

### Resumo de Complexidade Assintótica

| **Operação**      | **Função**            | **Complexidade de Tempo** | **Complexidade de Espaço** |
| ----------------- | --------------------- | ------------------------- | -------------------------- |
| Criar Lista       | `lista_seq_criar`     | $$O(1)$$                  | $$O(1)$$                   |
| Inserir no Fim    | `lista_seq_inserir`   | $$O(1)$$ amortizado       | $$O(1)$$                   |
| Obter por Índice  | `lista_seq_obter`     | $$O(1)$$                  | $$O(1)$$                   |
| Destruir          | `lista_seq_apagar`    | $$O(N)$$                  | $$O(1)$$                   |
| Consultar Tamanho | `lista_seq_tamanho`   | $$O(1)$$                  | $$O(1)$$                   |
| Verificar Vazia   | `lista_seq_vazia`     | $$O(1)$$                  | $$O(1)$$                   |
| Percorrer         | `lista_seq_percorrer` | $$O(N)$$                  | $$O(1)$$                   |

> Nota: $$N$$ representa o número de itens armazenados na lista. O acesso indexado via `lista_seq_obter` possui complexidade $$O(1)$$ devido ao endereçamento direto em memória contígua.

### Referência da API

#### `lista_seq_criar`

Cria e inicializa uma nova instância de lista sequencial na memória com capacidade inicial para 1000 elementos.

```c
LISTA_SEQ *lista_seq_criar(void);
```

* Parâmetros: Nenhum.
* Retorno: Ponteiro para a estrutura `LISTA_SEQ` alocada ou `NULL` em caso de falha de alocação de memória.
* Complexidade: $$O(1)$$.

#### `lista_seq_inserir`

Adiciona um novo elemento ao final da lista sequencial. O ponteiro de entrada é do tipo `void *` para manter a compatibilidade polimórfica com a arquitetura do projeto.

```c
bool lista_seq_inserir(void *lista_ptr, ITEM *item);
```

* Parâmetros:
  * `lista_ptr`: Ponteiro para a instância da lista (conversão interna para `LISTA_SEQ *`).
  * `item`: Ponteiro para o elemento `ITEM` a ser armazenado.
* Retorno: `true` se a inserção foi realizada com sucesso; `false` se algum argumento for `NULL` ou se a realocação de memória falhar.
* Complexidade: $$O(1)$$ amortizado ($$O(N)$$ no pior caso de realocação por `realloc`).

#### `lista_seq_obter`

Recupera o ponteiro para o elemento localizado numa posição/índice específico da lista.

```c
ITEM *lista_seq_obter(const LISTA_SEQ *lista, int posicao);
```

* Parâmetros:
  * `lista`: Ponteiro constante para a lista.
  * `posicao`: Índice de acesso baseado em zero ($$0 \le \text{posicao} < \text{tamanho}$$).
* Retorno: Ponteiro para o `ITEM` na posição solicitada, ou `NULL` se a lista for nula ou se o índice for inválido (fora dos limites).
* Complexidade: $$O(1)$$.

#### `lista_seq_apagar`

Desaloca a estrutura da lista sequencial e o seu vetor interno de ponteiros. Permite ao cliente definir se a memória dos itens armazenados também deve ser liberada.

```c
void lista_seq_apagar(LISTA_SEQ **lista, bool apagar_itens);
```

* Parâmetros:
  * `lista`: Endereço do ponteiro para a lista (`LISTA_SEQ **`). Ao final, `*lista` é atribuído como `NULL`.
  * `apagar_itens`: Booleano que indica se a função `item_apagar()` deve ser executada para cada item contido na lista.
* Complexidade: $$O(N)$$, onde $$N$$ é a quantidade de elementos na lista.

#### `lista_seq_tamanho`

Retorna o número de elementos atualmente armazenados na lista.

```c
int lista_seq_tamanho(const LISTA_SEQ *lista);
```

* Parâmetros:
  * `lista`: Ponteiro constante para a lista.
* Retorno: Valor inteiro indicando a quantidade de itens. Retorna `0` se a lista for `NULL`.
* Complexidade: $$O(1)$$.

#### `lista_seq_vazia`

Verifica se a lista sequencial está sem elementos.

```c
bool lista_seq_vazia(const LISTA_SEQ *lista);
```

* Parâmetros:
  * `lista`: Ponteiro constante para a lista.
* Retorno: `true` se a lista for nula ou não contiver itens; `false` caso contrário.
* Complexidade: $$O(1)$$.

#### `lista_seq_percorrer`

Itera sequencialmente do índice `0` até ao último elemento (`tamanho - 1`), aplicando a função de _callback_ especificada a cada `ITEM`. É utilizada para operações de filtragem e exportação dos resultados no ficheiro CSV de saída.

```c
void lista_seq_percorrer(const LISTA_SEQ *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto);
```

* Parâmetros:
  * `lista`: Ponteiro constante para a lista.
  * `visitar`: Ponteiro para a função de _callback_ que processa cada `ITEM *`.
  * `contexto`: Ponteiro genérico para contexto auxiliar (ex.: descritor de ficheiro, contador).
* Complexidade: $$O(N)$$.

### Exemplo de Uso

C

```c
#include <stdio.h>
#include "lista_seq.h"
#include "item.h"

// Função de callback para exibição dos itens durante a varredura
void imprimir_item(ITEM *item, void *contexto) {
    if (item != NULL) {
        item_imprimir(item);
    }
}

int main(void) {
    // 1. Instanciação da lista sequencial
    LISTA_SEQ *lista = lista_seq_criar();
    if (lista == NULL) return 1;

    // 2. Inserção de elementos
    ITEM *item1 = item_criar(101, "Asteroide Eros");
    ITEM *item2 = item_criar(102, "Asteroide Ceres");

    lista_seq_inserir(lista, item1);
    lista_seq_inserir(lista, item2);

    printf("Tamanho atual: %d\n", lista_seq_tamanho(lista)); // Saída: 2

    // 3. Acesso direto O(1) por índice
    ITEM *segundo = lista_seq_obter(lista, 1);
    if (segundo != NULL) {
        printf("Elemento no índice 1: ");
        item_imprimir(segundo);
    }

    // 4. Percurso sequencial
    printf("Conteúdo completo da lista:\n");
    lista_seq_percorrer(lista, imprimir_item, NULL);

    // 5. Destruição da lista e desalocação dos itens
    lista_seq_apagar(&lista, true);

    return 0;
}
```
