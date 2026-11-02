# Busca e Organização de Registros de Asteroides (SCC0202)

![C Language](https://img.shields.io/badge/Language-C-blue.svg)
![USP ICMC](https://img.shields.io/badge/USP-ICMC-red.svg)
![Build](https://img.shields.io/badge/Build-Makefile-brightgreen.svg)

Trabalho prático desenvolvido para a disciplina **SCC0202 - Algoritmos e Estruturas de Dados I** no Instituto de Ciências Matemáticas e de Computação (**ICMC - USP**). 

O objetivo do projeto é implementar um motor de busca e organização de dados altamente eficiente e desacoplado em linguagem **C**, permitindo que a estrutura de armazenamento base (`-base`) e a estrutura de resultados (`-result`) sejam selecionadas dinamicamente via linha de comando sem alterar a lógica das estruturas de dados.

---

## Funcionalidades Principais

- **Leitura e Filtro de Dados:** Leitura de datasets em formato CSV (Aproximação de Asteroides pela NASA) com parsing dinâmico de campos e tratamento de valores ausentes.
- **Processador de Consultas:** Suporte a arquivos de busca estruturados com suporte aos comandos:
  - `FILTER`: Condições obrigatórias de igualdade e comparação numérica (`==`, `!=`, `<=`, `>=`, `<`, `>`).
  - `SIM`: Cálculo de similaridade ponderada ($s(x,q,t) = \max(0, 1 - \frac{|x-q|}{t})$).
  - `ACCEPT`: Limiar mínimo de aceitação para similaridade total.
  - `ORDER`: Definição de ordenação dinâmica.
  - `GROUP` e `CROSS`: Critérios para agrupamento e cruzamento em listas avançadas.
- **Arquitetura Reutilizável (Polimorfismo em C):** Estruturas de dados genéricas utilizando `void *` e ponteiros de função (*callbacks*), permitindo reaproveitamento total para novos leitores/datasets (Parte 2 do projeto).
- **Métricas de Desempenho:** Monitoramento dos tempos de carga, busca, inserção nos resultados e exportação CSV em milissegundos.
- **Gestão Rígida de Memória:** Sem vazamentos de memória (*memory leaks*), testado e validado via `Valgrind`.

---

## Estruturas de Dados Implementadas (TADs)

O projeto suporta 8 Tipos Abstratos de Dados (TADs), implementados do zero sem o uso de bibliotecas externas:

1. **Pilha (`pilha`):** Estrutura LIFO com inserção/remoção em $O(1)$.
2. **Fila (`fila`):** Fila circular dinâmica com redimensionamento automático via `realloc`.
3. **Lista Sequencial (`lista_seq`):** Vetor dinâmico contíguo com acesso indexado em $O(1)$.
4. **Lista Encadeada (`lista_enc`):** Lista encadeada simples com ponteiro para o início e fim.
5. **Lista com Cabeça (`lista_cabeca`):** Lista encadeada utilizando um nó sentinela/cabeça real.
6. **Lista Ordenada (`lista_ord`):** Manutenção da ordem (crescente/decrescente) realizada diretamente no momento da inserção.
7. **Lista Generalizada (`lista_gen`):** Organização de itens em grupos e subgrupos por critérios hierárquicos.
8. **Lista Cruzada (`lista_cruz`):** Encadeamento bidirecional simultâneo sem duplicação de dados na memória RAM.

---

## Arquitetura do Projeto

```text
.
├── include/            # Arquivos de cabeçalho (.h)
│   ├── cli.h
│   ├── consulta.h
│   ├── fila.h
│   ├── item.h
│   ├── lista_cabeca.h
│   ├── lista_enc.h
│   ├── lista_ord.h
│   ├── lista_seq.h
│   ├── pilha.h
│   └── reader_nasa.h
├── src/                # Implementação em C (.c)
│   ├── cli.c
│   ├── consulta.c
│   ├── fila.c
│   ├── item.c
│   ├── lista_cabeca.c
│   ├── lista_enc.c
│   ├── lista_ord.c
│   ├── lista_seq.c
│   ├── main.c
│   ├── pilha.c
│   └── reader_nasa.c
├── dados/              # Datasets de entrada (.csv)
├── consultas/          # Arquivos de consulta (.txt)
├── Makefile            # Automação de compilação
└── README.md
```

---

## Como Compilar e Executar

### Pré-requisitos
- Compilador `gcc` suportando C99/C11.
- Ferramenta `make`.

### Compilação
O projeto utiliza flags rigorosas de compilação (`-Wall -Wextra -pedantic -g`). Para gerar o executável `trabalho`:

```bash
make
```

### Execução Exemplo

```bash
./trabalho \
  --reader nasa \
  --input dados/asteroids.csv \
  --base lista_enc \
  --result lista_ord \
  --query consultas/q1.txt \
  --output resultado.csv \
  --top-k 100 \
  --limit 10000
```

### Parâmetros da CLI

| Argumento | Descrição |
| :--- | :--- |
| `--reader` | Identificador do leitor de dataset (`nasa`, etc.) |
| `--input` | Caminho do arquivo CSV de entrada |
| `--base` | Estrutura de dados usada para armazenar todo o dataset |
| `--result` | Estrutura de dados usada para armazenar o resultado da busca |
| `--query` | Arquivo contendo os critérios da consulta |
| `--output` | Arquivo de saída para exportação dos resultados encontrados |
| `--top-k` | Máximo de registros exportados (0 = todos) |
| `--limit` | Máximo de registros carregados da base (0 = todos) |

---

## Formato de Saída (Terminal)

Ao finalizar a execução, o programa imprime no `stdout`:

```text
STATUS-OK
INPUT RECORDS=10000
MATCHED RECORDS=487
OUTPUT RECORDS=100
LOAD_MS=12.43
SEARCH_MS=3.288
RESULT_INSERT_MS=0.812
WRITE_MS=1.050
```

---

## Documentação Completa

Para uma análise detalhada sobre a complexidade assintótica das operações, benchmarks experimentais de tempo de execução e guias avançados de arquitetura, acesse nossa documentação no **GitBook**.
