---
description: >-
  Estrutura de dados linear que segue o princípio LIFO (Last In, First Out), ou
  seja, em que o último elemento a entrar é o primeiro a sair
icon: layer-group
---

# TAD Pilha

Esta implementação foi desenvolvida utilizando alocação dinâmica e encadeamento simples através de nós ponteiros. O TAD é totalmente genérico e desacoplado do tipo do dado, manipulando elementos do tipo ponteiro para `ITEM`. No escopo do projeto, a pilha pode ser selecionada dinamicamente via linha de comando tanto como estrutura principal de armazenamento (`--base pilha`) quanto como estrutura de resultados (`--result pilha`).

### Estrutura de Dados e Encapsulamento

A representação concreta das estruturas está oculta no arquivo de implementação `pilha.c`, garantindo o encapsulamento dos dados e impedindo o acesso direto aos ponteiros por parte do cliente.

```c
typedef struct no_ NO;

struct no_ {
    ITEM *item; 
    NO *anterior;
};

struct pilha_ {
    NO *topo;
    int tamanho;
};
```

* `NO`: Elemento interno da pilha. Armazena o ponteiro para o `ITEM` e o ponteiro `anterior` para o nó logo abaixo dele na pilha.
* `PILHA`: Estrutura principal que mantém o ponteiro para o nó no `topo` e o contador `tamanho` para consulta direta em tempo constante.

### Resumo de Complexidade Assintótica

<table data-header-hidden data-search="false"><thead><tr><th></th><th></th><th></th><th></th></tr></thead><tbody><tr><td><strong>Operação</strong></td><td><strong>Função</strong></td><td><strong>Complexidade de Tempo</strong></td><td><strong>Complexidade de Espaço</strong></td></tr><tr><td>Criar Pilha</td><td><code>pilha_criar</code></td><td><span class="math">O(1)</span></td><td><span class="math">O(1)</span></td></tr><tr><td>Empilhar</td><td><code>pilha_inserir</code></td><td><span class="math">O(1)</span></td><td><span class="math">O(1)</span></td></tr><tr><td>Desempilhar</td><td><code>pilha_desempilhar</code></td><td><span class="math">O(1)</span></td><td><span class="math">O(1)</span></td></tr><tr><td>Destruir</td><td><code>pilha_apagar</code></td><td><span class="math">O(N)</span></td><td><span class="math">O(1)</span></td></tr><tr><td>Consultar Tamanho</td><td><code>pilha_tamanho</code></td><td><span class="math">O(1)</span></td><td><span class="math">O(1)</span></td></tr><tr><td>Verificar Vazia</td><td><code>pilha_vazia</code></td><td><span class="math">O(1)</span></td><td><span class="math">O(1)</span></td></tr><tr><td>Percorrer</td><td><code>pilha_percorrer</code></td><td><span class="math">O(N)</span></td><td><span class="math">O(1)</span></td></tr></tbody></table>

{% hint style="info" %}
$$N$$ representa o número de itens armazenados na pilha no momento da chamada.
{% endhint %}

### Funções

#### `pilha_criar`

Cria e inicializa uma nova instância de pilha na memória dinâmica.

```c
PILHA *pilha_criar(void);
```

* Parâmetros: Nenhum.
* Retorno: Ponteiro para a estrutura `PILHA` alocada ou `NULL` em caso de falha de alocação de memória.
* Complexidade: $$O(1)$$.

#### `pilha_inserir`

Insere (empilha) um novo elemento no topo da pilha. O ponteiro de entrada é do tipo `void *` para garantir compatibilidade genérica com ponteiros de funções da arquitetura do projeto.

```c
bool pilha_inserir(void *pilha_ptr, ITEM *item);
```

* Parâmetros:
  * `pilha_ptr`: Ponteiro para a instância da pilha (cast interno para `PILHA *`).
  * `item`: Ponteiro para o elemento `ITEM` a ser armazenado.
* Retorno: `true` se a inserção for realizada com sucesso; `false` se qualquer ponteiro de entrada for inválido (`NULL`) ou se a alocação do nó falhar.
* Complexidade: $$O(1)$$.

#### `pilha_desempilhar`

Remove e retorna o elemento localizado no topo da pilha. O nó interno da pilha é desalocado, mantendo a memória do `ITEM` intacta.

```c
ITEM *pilha_desempilhar(PILHA *pilha);
```

* Parâmetros:
  * `pilha`: Ponteiro para a pilha.
* Retorno: Ponteiro para o `ITEM` desempilhado, ou `NULL` caso a pilha seja nula ou esteja vazia.
* Complexidade: $$O(1)$$$$ $O(1)$ $$.

#### `pilha_apagar`

Desaloca a pilha e todos os seus nós internos. Permite ao cliente escolher se a memória dos elementos `ITEM` também deve ser liberada.

```c
void pilha_apagar(PILHA **pilha, bool apagar_itens);
```

* Parâmetros:
  * `pilha`: Endereço do ponteiro para a pilha (`PILHA **`). Ao final da execução, `*pilha` é definido como `NULL`.
  * `apagar_itens`: Booleano que define se a função `item_apagar()` será chamada para liberar cada `ITEM` contido nos nós.
* Complexidade: $$O(N)$$ onde $$ $N$ $$$$N$$ é a quantidade de elementos na pilha.

#### `pilha_tamanho`

Retorna a quantidade total de elementos presentes na pilha.

```c
int pilha_tamanho(const PILHA *pilha);
```

* Parâmetros:
  * `pilha`: Ponteiro constante para a pilha.
* Retorno: Valor inteiro indicando o número de itens na pilha. Retorna `0` se a pilha for `NULL`.
* Complexidade: $$ $O(1)$ $$$$O(1)$$.

#### `pilha_vazia`

Verifica se a pilha não possui elementos.

```c
bool pilha_vazia(const PILHA *pilha);
```

* Parâmetros:
  * `pilha`: Ponteiro constante para a pilha.
* Retorno: `true` se a pilha for nula ou se o número de itens for igual a zero; `false` caso contrário.
* Complexidade: $$ $O(1)$ $$$$O(1)$$.

#### `pilha_percorrer`

Percorre a pilha do topo em direção à base (ordem LIFO), aplicando uma função de _callback_ a cada item armazenado. Esta operação é fundamental para as etapas de busca no dataset e de gravação dos dados filtrados no arquivo de saída CSV.

```c
void pilha_percorrer(const PILHA *pilha, void (*visitar)(ITEM *item, void *contexto), void *contexto);
```

* Parâmetros:
  * `pilha`: Ponteiro constante para a pilha.
  * `visitar`: Ponteiro de função que recebe um `ITEM *` e um ponteiro genérico de `contexto`.
  * `contexto`: Ponteiro genérico passado para a função `visitar` (ex: arquivo de saída, contador, acumulador).
* Complexidade: $$O(N)$$.

### Exemplo de Uso

```c
#include <stdio.h>
#include "pilha.h"
#include "item.h"

// Função de callback para exibição dos itens
void imprimir_item(ITEM *item, void *contexto) {
    if (item != NULL) {
        item_imprimir(item);
    }
}

int main(void) {
    // 1. Criação da pilha
    PILHA *p = pilha_criar();
    if (p == NULL) return 1;

    // 2. Inserção (Empilhar)
    ITEM *item1 = item_criar(1, "Asteroide A");
    ITEM *item2 = item_criar(2, "Asteroide B");

    pilha_inserir(p, item1);
    pilha_inserir(p, item2);

    printf("Tamanho da pilha: %d\n", pilha_tamanho(p)); // Saída: 2

    // 3. Percurso (Topo -> Base)
    printf("Conteudo da pilha:\n");
    pilha_percorrer(p, imprimir_item, NULL);

    // 4. Remoção (Desempilhar - Ordem LIFO)
    ITEM *removido = pilha_desempilhar(p);
    if (removido != NULL) {
        printf("Item removido do topo: ");
        item_imprimir(removido);
        item_apagar(&removido);
    }

    // 5. Destruição da pilha e limpeza residual
    pilha_apagar(&p, true);

    return 0;
}
```
