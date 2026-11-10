---
icon: window
---

# TAD CLI

## TAD Interface de Linha de Comando (`cli`)

O módulo `cli` implementa o Tipo Abstrato de Dados (TAD) **CONFIG / CLI**, responsável por capturar, validar, armazenar e disponibilizar os parâmetros de execução fornecidos ao executável do programa (`trabalho`) através da linha de comando.

Este TAD centraliza as opções de configuração do projeto, como a escolha do leitor, arquivo de entrada, estruturas de dados base e de resultado, arquivo de consulta, arquivo de saída e os limites de processamento (`top-k` e `limit`).

### Visão Geral e Arquitetura

O TAD `CONFIG` atua como a primeira camada de execução do programa. Ele parseia `argc` e `argv`, permitindo flexibilidade na sintaxe dos argumentos — aceitando tanto hífens simples (`-reader`, `-input`) quanto hífens duplos (`--reader`, `--input`).

#### Parâmetros Suportados

| **Argumento CLI**      | **Função Interna** | **Descrição**                                                        | **Valor Padrão** |
| ---------------------- | ------------------ | -------------------------------------------------------------------- | ---------------- |
| `-reader` / `--reader` | `reader`           | Nome do leitor do dataset (`nasa`, `detran`, etc.)                   | `NULL`           |
| `-input` / `--input`   | `input`            | Caminho do arquivo CSV de entrada                                    | `NULL`           |
| `-base` / `--base`     | `base`             | Estrutura de dados para carregar a base completa                     | `NULL`           |
| `-result` / `--result` | `result`           | Estrutura de dados para os resultados da busca                       | `NULL`           |
| `-query` / `--query`   | `query`            | Caminho do arquivo de regras de consulta texto                       | `NULL`           |
| `-output` / `--output` | `output`           | Caminho do arquivo CSV de saída                                      | `NULL`           |
| `-top-k` / `--top-k`   | `top_k`            | Máximo de registros exportados ($0 = \text{todos}$)                  | <p>$0$<br></p>   |
| `-limit` / `--limit`   | `limit`            | Máximo de registros carregados da entrada ($0 = \text{toda a base}$) | <p>$0$<br></p>   |

### Definição da Estrutura Interna

A estrutura `struct cli_` (renomeada via `typedef` para `CONFIG`) armazena strings alocadas dinamicamente para os caminhos e tipos, e inteiros para as opções numéricas.

```c
struct cli_ {
    char *reader;   /* Nome do leitor selecionado (ex: "nasa") */
    char *input;    /* Caminho do arquivo de entrada (ex: "dados/asteroids.csv") */
    char *base;     /* Nome do TAD base (ex: "lista_enc") */
    char *result;   /* Nome do TAD resultado (ex: "lista_ord") */
    char *query;    /* Caminho da consulta (ex: "consultas/q1.txt") */
    char *output;   /* Caminho do arquivo de saída (ex: "resultado.csv") */
    int top_k;      /* Limite de registros salvos na saída (0 = sem limite) */
    int limit;      /* Limite de registros lidos da entrada (0 = sem limite) */
};
```

#### Diagrama Estrutural do TAD CONFIG

```mermaid
graph TD
    subgraph ConfigStruct ["struct cli_ (CONFIG)"]
        R["reader: char*"]
        I["input: char*"]
        B["base: char*"]
        RES["result: char*"]
        Q["query: char*"]
        O["output: char*"]
        TK["top_k: int = 100"]
        LM["limit: int = 10000"]
    end

    subgraph MemoryHeap ["Strings na Memória Heap"]
        R_STR["'nasa'"]
        I_STR["'dados/asteroids.csv'"]
        B_STR["'lista_enc'"]
        RES_STR["'lista_ord'"]
        Q_STR["'consultas/q1.txt'"]
        O_STR["'resultado.csv'"]
    end

    R --> R_STR
    I --> I_STR
    B --> B_STR
    RES --> RES_STR
    Q --> Q_STR
    O --> O_STR

    classDef structStyle fill:#1e293b,stroke:#3b82f6,stroke-width:2px,color:#fff;
    classDef heapStyle fill:#0f172a,stroke:#10b981,stroke-width:2px,color:#fff;

    class ConfigStruct structStyle;
    class R_STR,I_STR,B_STR,RES_STR,Q_STR,O_STR heapStyle;

```

#### Fluxograma do Parsing de Argumentos (`cli_parse_args`)

```mermaid
flowchart TD
    A([Início: cli_parse_args]) --> B{argc < 2 ou argv == NULL?}
    B -- Sim --> C[Retorna false]
    B -- Não --> D[Loop: i = 1 até argc - 2]
    
    D --> E{"argv começa com '-'?"}
    E -- Não --> F[Avança i++]
    E -- Sim --> G[Remove hífens iniciais '-' ou '--']
    
    G --> H{Identifica chave em argv}
    H -- "reader" --> I["Desaloca antigo (se houver)<br/>cli->reader = strdup(argv[i+1])<br/>i++"]
    H -- "input" --> J["cli->input = strdup(argv)<br/>i++"]
    H -- "base" --> K["cli->base = strdup(argv)<br/>i++"]
    H -- "result" --> L["cli->result = strdup(argv<br/>i++"]
    H -- "query" --> M["cli->query = strdup(argv)<br/>i++"]
    H -- "output" --> N["cli->output = strdup(argv)<br/>i++"]
    H -- "top-k" --> O["cli->top_k = atoi(argv)<br/>i++"]
    H -- "limit" --> P["cli->limit = atoi(argv)<br/>i++"]
    
    I & J & K & L & M & N & O & P --> Q{Há mais argumentos?}
    F --> Q
    Q -- Sim --> D
    Q -- Não --> R([Retorna true])

```

### Especificação da API

#### `cli_criar`

Aloca e inicializa a estrutura de configuração `CONFIG` zerada via `calloc`.

```c
CONFIG *cli_criar(void);
```

* **Retorno:** Ponteiro para a nova estrutura `CONFIG` criada ou `NULL` em caso de falha de alocação.
* **Complexidade:** O(1) tempo / O(1) espaço.

#### `cli_apagar`

Desaloca iterativamente todas as strings alocadas no `heap` (`reader`, `input`, `base`, `result`, `query`, `output`) e libera a memória da estrutura `CONFIG`.

```c
bool cli_apagar(CONFIG **cli);
```

* **Parâmetros:**
  * `cli`: Endereço do ponteiro para a estrutura `CONFIG`. É alterado para `NULL` ao final.
* **Retorno:** `true` se a desalocação ocorrer com sucesso; `false` se o ponteiro for nulo.
* **Complexidade:** O(1) tempo / O(1) espaço.

#### `cli_parse_args`

Processa o vetor de argumentos `argv` passado na execução do programa e preenche os campos do TAD `CONFIG`.

```c
bool cli_parse_args(CONFIG *cli, int argc, char* argv[]);
```

* **Parâmetros:**
  * `cli`: Ponteiro para o TAD `CONFIG`.
  * `argc`: Quantidade total de argumentos da linha de comando.
  * `argv`: Vetor de strings contendo os argumentos.
* **Retorno:** `true` em caso de sucesso no parsing; `false` se os parâmetros forem inválidos ou insuficientes.
* **Complexidade:** $$O(A \cdot S)$$ tempo, onde $$A = \text{argc}$$ e S é a extensão média da string do argumento.

#### Getters de Configuração

Permitem acesso seguro e em tempo constante $O(1)$ aos parâmetros de configuração:

```c
const char* cli_get_reader(const CONFIG *cli);
const char* cli_get_input(const CONFIG *cli);
const char* cli_get_base(const CONFIG *cli);
const char* cli_get_result(const CONFIG *cli);
const char* cli_get_query(const CONFIG *cli);
const char* cli_get_output(const CONFIG *cli);
int cli_get_top_k(const CONFIG *cli);
int cli_get_limit(const CONFIG *cli);

```

* **Complexidade:** $O(1)$ tempo / $O(1)$ espaço.

#### `cli_imprimir_config`

Função utilitária de depuração para exibir no console a configuração carregada.

```c
void cli_imprimir_config(const CONFIG *cli);
```

* **Complexidade:** $O(1)$ tempo / $O(1)$ espaço.

### Análise de Complexidade Assintótica

Considerando $A$ como o número de argumentos em `argv` ($A = \text{argc}$):

| **Operação**              | **Complexidade Temporal** | **Complexidade Espacial** | **Justificativa**                                                  |
| ------------------------- | ------------------------- | ------------------------- | ------------------------------------------------------------------ |
| **`cli_criar`**           | O(1)                      | O(1)                      | Alocação direta da estrutura.                                      |
| **`cli_parse_args`**      | O(A)                      | O(S\_{total})             | Varredura única de `argv` com alocação das strings dos parâmetros. |
| **`cli_get_*` (Getters)** | O(1)                      | O(1)                      | Acesso direto aos campos da estrutura.                             |
| **`cli_apagar`**          | O(1)                      | O(1)                      | Liberação dos 6 ponteiros de string e da struct.                   |

### Exemplo Prático de Uso

O exemplo a seguir demonstra o ciclo típico de inicialização e leitura de opções no `main.c`:

```c
#include <stdio.h>
#include "cli.h"

int main(int argc, char *argv[]) {
    // 1. Criar a estrutura de configuração CLI
    CONFIG *config = cli_criar();
    if (config == NULL) {
        fprintf(stderr, "Erro ao alocar estrutura de configuração.\n");
        return 1;
    }

    // 2. Realizar o parse dos argumentos fornecidos
    if (!cli_parse_args(config, argc, argv)) {
        fprintf(stderr, "Uso incorreto dos argumentos da CLI.\n");
        cli_apagar(&config);
        return 1;
    }

    // 3. Exibir a configuração para verificação
    cli_imprimir_config(config);

    // 4. Recuperar valores dos parâmetros para uso no fluxo principal
    const char *reader = cli_get_reader(config);
    const char *base = cli_get_base(config);
    int limit = cli_get_limit(config);

    printf("Iniciando leitor '%s' na estrutura '%base' com limite %d...\n", 
           reader, base, limit);

    // 5. Desalocar a memória da CLI ao encerrar
    cli_apagar(&config);

    return 0;
}

```
