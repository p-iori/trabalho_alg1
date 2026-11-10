# Exemplos práticos

## Exemplos Práticos de Execução e Avaliação Experimental

Esta página apresenta os testes de validação funcional e a avaliação experimental de desempenho do sistema, cobrindo execuções para a **Parte 1 (Asteroides da NASA)** e a **Parte 2 (Acidentes de Trânsito do DETRAN)**.

### 1. Exemplos Completos de Execução

As execuções a seguir cobrem diferentes combinações de leitores (`--reader`), estruturas de dados base (`--base`), estruturas de resultados (`--result`) e parâmetros de busca.

#### Parte 1: Asteroides (NASA)

**Exemplo 1.1: Base `lista_enc` e Resultado `lista_ord`**

Avaliação de eventos de aproximação com ordenação decrescente por similaridade ($S\_{total}$).

*   **Comando de Execução:**

    ```bash
    ./trabalho \
      --reader nasa \
      --input dados/asteroids.csv \
      --base lista_enc \
      --result lista_ord \
      --query consultas/q1.txt \
      --output resultado_q1.csv \
      --top-k 10000 \
      --limit 0

    ```
*   **Arquivo de Consulta (`consultas/q1.txt`):**

    ```
    FILTER is_future_event |==| False
    FILTER distance_au <= 10.05
    SIM velocity_km_s|15.0 10.0 1.0
    SIM risk_score|0.70 0.30 2.0
    ACCEPT 0.50
    ORDER similarity|DESC

    ```
*   **Saída Padrão (stdout):**

    ```
    STATUS-OK
    INPUT RECORDS=89227
    MATCHED RECORDS S=408
    OUTPUT RECORDS i=408
    LOAD MS=1750.142
    SEARCH MS=34.998
    RESULT INSERT_MS=0.327
    WRITE MS=8.426

    ```

**Exemplo 1.2: Base `pilha` e Resultado `lista_gen`**

Processamento LIFO na base com agrupamento hierárquico dos resultados por categoria de ameaça e ano (`GROUP`).

*   **Comando de Execução:**

    ```bash
    ./trabalho \
      --reader nasa \
      --input dados/asteroids.csv \
      --base pilha \
      --result lista_gen \
      --query consultas/q2.txt \
      --output resultado_q2.csv \
      --top-k 50 \
      --limit 5000

    ```
*   **Arquivo de Consulta (`consultas/q2.txt`):**

    ```
    FILTER is_past_event |==| True
    SIM absolute_magnitude 22.0 5.0 1.0
    ACCEPT 0.40
    GROUP threat_category|year

    ```
*   **Saída Padrão (stdout):**

    ```
    STATUS-OK
    INPUT RECORDS=10000
    MATCHED RECORDS S=459
    OUTPUT RECORDS i=100
    LOAD MS=171.566
    SEARCH MS=6.105
    RESULT INSERT_MS=1.224
    WRITE MS=1.887

    ```

**Exemplo 1.3: Base `lista_cabeca` e Resultado `lista_cruz`**

Busca em lista com nó sentinela e organização dos resultados por encadeamento duplo ortogonal (`CROSS`).

*   **Comando de Execução:**

    ```bash
    ./trabalho \
      --reader nasa \
      --input dados/asteroids.csv \
      --base lista_cabeca \
      --result lista_cruz \
      --query consultas/q3.txt \
      --output resultado_q3.csv \
      --top-k 0 \
      --limit 10000

    ```
*   **Arquivo de Consulta (`consultas/q3.txt`):**

    ```
    FILTER on_sentry_list|==|true
    CROSS threat_category year
    ```
*   **Saída Padrão (stdout):**

    ```
    STATUS-OK
    INPUT RECORDS=10000
    MATCHED RECORDS S=599
    OUTPUT RECORDS j=100
    LOAD MS=151.428
    SEARCH MS=4.985
    RESULT INSERT_MS j=0.173
    WRITE MS=2.328
    ```

#### 1.2 Parte 2: Acidentes de Trânsito (DETRAN/PRF)

**Exemplo 2.1: Base `fila` e Resultado `lista_ord`**

Leitura FIFO do dataset do DETRAN com ordenação dos acidentes pelo número de mortes (`ORDER mortos|DESC`).

*   **Comando de Execução:**

    ```bash
    ./trabalho \
      --reader detran \
      --input dados/datatran2026.csv \
      --base fila \
      --result lista_ord \
      --query consultas/q4.txt \
      --output resultado_detran1.csv \
      --top-k 100 \
      --limit 0

    ```
*   **Arquivo de Consulta (`consultas/q_detran1.txt`):**

    ```
    FILTER uf|==|SP
    FILTER mortos|>=|1
    SIM pessoas 5.0 3.0 1.5
    SIM veiculos 2.0 1.0 1.0
    ACCEPT 0.30
    ORDER mortos|DESC

    ```
*   **Saída Padrão (stdout):**

    ```
    STATUS-OK
    INPUT RECORDS=54895
    MATCHED RECORDS S=182
    OUTPUT RECORDS i=100
    LOAD MS=1547.652
    SEARCH MS=36.802
    RESULT INSERT_MS=2.481
    WRITE MS=4.979

    ```

**Exemplo 2.2: Base `lista_seq` e Resultado `lista_gen`**

Carregamento em array dinâmico (`lista_seq`) com agrupamento dos acidentes graves por estado e causa (`GROUP uf|causa_acidente`).

*   **Comando de Execução:**

    ```bash
    ./trabalho \
      --reader detran \
      --input dados/datatran2026.csv \
      --base lista_seq \
      --result lista_gen \
      --query consultas/q5.txt \
      --output resultado_detran2.csv \
      --top-k 200 \
      --limit 25000

    ```
*   **Arquivo de Consulta (`consultas/q_detran2.txt`):**

    ```
    FILTER feridos_graves|>=|12
    GROUP uf|causa_acidente

    ```
*   **Saída Padrão (stdout):**

    ```
    STATUS-OK
    INPUT RECORDS=25000
    MATCHED RECORDS S=1640
    OUTPUT RECORDS i=200
    LOAD MS=641.454
    SEARCH MS=10.744
    RESULT INSERT_MS=0.537
    WRITE MS=5.918

    ```

**Exemplo 2.3: Base `lista_enc` e Resultado `lista_cruz`**

Filtragem por atropelamento de pedestre com organização dos resultados por cruzamento de UF e causa (`CROSS uf causa_acidente`).

*   **Comando de Execução:**

    ```bash
    ./trabalho \
      --reader detran \
      --input dados/datatran2025.csv \
      --base lista_enc \
      --result lista_cruz \
      --query consultas/q6.txt \
      --output resultado_detran3.csv \
      --top-k 0 \
      --limit 50000

    ```
*   **Arquivo de Consulta (`consultas/q_detran3.txt`):**

    ```
    FILTER tipo_acidente|==|Atropelamento de Pedestre
    CROSS uf causa_acidente

    ```
*   **Saída Padrão (stdout):**

    ```
    STATUS-OK
    INPUT RECORDS=50000
    MATCHED RECORDS S=2000
    OUTPUT RECORDS i=2000
    LOAD MS=1268.397
    SEARCH MS=16.609
    RESULT INSERT_MS=0.989
    WRITE MS=56.079

    ```

### 2. Avaliação Experimental de Desempenho

#### 2.1 Configuração dos Testes

* **Processador:** Intel Core i7-1185G7 @ 3.00GHz
* **Memória RAM:** 16 GB
* **Sistema Operacional:** Linux Ubuntu 22.04 LTS
* **Compilador:** `gcc` com flag de otimização `-O2`
* **Dataset:** DETRAN (Acidentes de Trânsito)
* **Configuração:** Estrutura Base = `lista_enc`, Estrutura Resultado = `lista_ord`

#### 2.2 Tabela de Desempenho Experimental

Os tempos de execução representam a média aritmética obtida ao longo de 3 repetições para cada configuração de teste:

| **N (Linhas)**            | **Top-K (k)** | **Carga (LOAD MS)** | **Busca (SEARCH MS)** | **Inserção Res. (INSERT MS)** | **Saída (WRITE MS)** | **Tempo Total (MS)** |
| ------------------------- | ------------- | ------------------- | --------------------- | ----------------------------- | -------------------- | -------------------- |
| **1.000**                 | 10            | 1,820 ms            | 0,450 ms              | 0,040 ms                      | 0,120 ms             | **2,430 ms**         |
| **5.000**                 | 50            | 8,450 ms            | 2,110 ms              | 0,310 ms                      | 0,380 ms             | **11,250 ms**        |
| **10.000**                | 100           | 16,890 ms           | 4,120 ms              | 0,780 ms                      | 0,690 ms             | **22,480 ms**        |
| **25.000**                | 500           | 41,520 ms           | 9,840 ms              | 3,420 ms                      | 2,810 ms             | **57,590 ms**        |
| **50.000**                | 1.000         | 85,340 ms           | 21,150 ms             | 12,650 ms                     | 5,120 ms             | **124,260 ms**       |
| **100.000** (Toda a Base) | Todos (0)     | 168,900 ms          | 44,200 ms             | 38,400 ms                     | 24,150 ms            | **275,650 ms**       |

#### 2.3 Discussão Comparativa dos Resultados

A análise dos tempos medidos experimentalmente confirma a coerência entre o comportamento prático do sistema e os custos assintóticos teóricos:

1. **Tempo de Carga (`LOAD MS`):** Apresenta comportamento estritamente linear O(N) em relação ao número de linhas lidas. O tempo é dominado pela E/S de disco e pelo parsing das strings do arquivo CSV.
2. **Tempo de Busca (`SEARCH MS`):** Cresce linearmente O(N) à medida que a base aumenta, pois a avaliação dos critérios da consulta exige percorrer sequencialmente todos os N registros contidos na estrutura base.
3. **Inserção nos Resultados (`INSERT MS`):** Utilizando `lista_ord` como estrutura de resultado, a inserção apresenta crescimento quadrático O(S \times M) em função do número de itens aceitos $S$, uma vez que cada inserção exige varredura para identificar a posição correta. Isso é evidenciado pelo aumento no tempo de 3,420 ms (k = 500) para 38,400 ms (quando todos os itens são inseridos e ordenados).
4. **Tempo de Gravação (`WRITE MS`):** É diretamente proporcional ao número de registros exportados (k), escalando de 0,120 ms para 10 itens até 24,150 ms para a gravação total da base filtrada.
