#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "pipeline.h"
#include "cli.h"
#include "consulta.h"
#include "item.h"
#include "reader_nasa.h"

// Inclusão dos TADs das estruturas de dados
#include "pilha.h"
#include "fila.h"
#include "lista_seq.h"
#include "lista_enc.h"
#include "lista_cabeca.h"
#include "lista_ord.h"
#include "lista_gen.h"
#include "lista_cruz.h"

struct pipeline_ {
    CONSULTA *consulta;
    
    void *estrutura_base;
    funcao_inserir_fn inserir_base;
    const char *nome_base;
    
    void *estrutura_result;
    funcao_inserir_fn inserir_result;
    const char *nome_result;

    // Métricas de execução
    int input_records;
    int matched_records;
    int output_records;

    double load_ms;
    double search_ms;
    double result_insert_ms;
    double write_ms;
};

PIPELINE *pipeline_criar(void) {
    PIPELINE *exec = (PIPELINE*) calloc(1, sizeof(PIPELINE));
    if (exec == NULL) return NULL;

    return exec;
}

// Função auxiliar interna para instanciar a estrutura de dados solicitada na CLI
static void *instanciar_estrutura(const char *nome, CONSULTA *consulta, funcao_inserir_fn *fn_inserir_out) {
    if (nome == NULL || fn_inserir_out == NULL) return NULL;

    void *estrutura = NULL;

    if (strcmp(nome, "pilha") == 0) {
        estrutura = pilha_criar();
        *fn_inserir_out = (funcao_inserir_fn) pilha_inserir;
    } else if (strcmp(nome, "fila") == 0) {
        estrutura = fila_criar();
        *fn_inserir_out = (funcao_inserir_fn) fila_inserir;
    } else if (strcmp(nome, "lista_seq") == 0) {
        estrutura = lista_seq_criar();
        *fn_inserir_out = (funcao_inserir_fn) lista_seq_inserir;
    } else if (strcmp(nome, "lista_enc") == 0) {
        estrutura = lista_enc_criar();
        *fn_inserir_out = (funcao_inserir_fn) lista_enc_inserir;
    } else if (strcmp(nome, "lista_cabeca") == 0) {
        estrutura = lista_cabeca_criar();
        *fn_inserir_out = (funcao_inserir_fn) lista_cabeca_inserir;
    } else if (strcmp(nome, "lista_ord") == 0) {
        estrutura = lista_ord_criar();
        if (consulta != NULL) {
            lista_ord_configurar_ordem(
                estrutura, 
                consulta_get_ordem_campo(consulta), 
                consulta_get_ordem_decrescente(consulta)
            );
        }
        *fn_inserir_out = (funcao_inserir_fn) lista_ord_inserir;
    } else if (strcmp(nome, "lista_gen") == 0) {
        estrutura = lista_gen_criar();
        if (consulta != NULL) {
            lista_gen_configurar(
                estrutura, 
                consulta_get_grupo_campo1(consulta), 
                consulta_get_grupo_campo2(consulta)
            );
        }
        *fn_inserir_out = (funcao_inserir_fn) lista_gen_inserir;
    } else if (strcmp(nome, "lista_cruz") == 0) {
        estrutura = lista_cruz_criar();
        if (consulta != NULL) {
            lista_cruz_configurar(
                estrutura, 
                consulta_get_cross_campo1(consulta), 
                consulta_get_cross_campo2(consulta)
            );
        }
        *fn_inserir_out = (funcao_inserir_fn) lista_cruz_inserir;
    }

    return estrutura;
}

// Função auxiliar para libertar a memória da estrutura de dados correspondente
static void destruir_estrutura(void **estrutura_ptr, const char *nome) {
    if (estrutura_ptr == NULL || *estrutura_ptr == NULL || nome == NULL) return;

    if (strcmp(nome, "pilha") == 0) {
        pilha_apagar((PILHA**) estrutura_ptr, true);
    } else if (strcmp(nome, "fila") == 0) {
        fila_apagar((FILA**) estrutura_ptr, true);
    } else if (strcmp(nome, "lista_seq") == 0) {
        lista_seq_apagar((LISTA_SEQ**) estrutura_ptr, true);
    } else if (strcmp(nome, "lista_enc") == 0) {
        lista_enc_apagar((LISTA_ENC**) estrutura_ptr, true);
    } else if (strcmp(nome, "lista_cabeca") == 0) {
        lista_cabeca_apagar((LISTA_CABECA**) estrutura_ptr, true);
    } else if (strcmp(nome, "lista_ord") == 0) {
        lista_ord_apagar((LISTA_ORD**) estrutura_ptr, true);
    } else if (strcmp(nome, "lista_gen") == 0) {
        lista_gen_apagar((LISTA_GEN**) estrutura_ptr, true);
    } else if (strcmp(nome, "lista_cruz") == 0) {
        lista_cruz_apagar((LISTA_CRUZ**) estrutura_ptr, true);
    }

    *estrutura_ptr = NULL;
}

// Imprime as métricas no formato especificado pelo enunciado do projeto
static void imprimir_relatorio(const PIPELINE *exec) {
    if (exec == NULL) return;

    printf("STATUS-OK\n");
    printf("INPUT RECORDS=%d\n", exec->input_records);
    printf("MATCHED RECORDS S=%d\n", exec->matched_records);
    printf("OUTPUT RECORDS i=%d\n", exec->output_records);
    printf("LOAD MS=%.3f\n", exec->load_ms);
    printf("SEARCH MS=%.3f\n", exec->search_ms);
    printf("RESULT INSERT_MS=%.3f\n", exec->result_insert_ms);
    printf("WRITE MS=%.3f\n", exec->write_ms);
}

bool pipeline_run(PIPELINE *exec, const CONFIG *cli) {
    if (exec == NULL || cli == NULL) return false;

    // 1. Carregar Consulta (-query)
    const char *caminho_query = cli_get_query(cli);
    exec->consulta = consulta_criar();
    if (exec->consulta == NULL) return false;

    if (!consulta_carregar_arquivo(exec->consulta, caminho_query)) {
        return false;
    }

    // 2. Instanciar Estrutura Base (-base)
    exec->nome_base = cli_get_base(cli);
    exec->estrutura_base = instanciar_estrutura(exec->nome_base, exec->consulta, &exec->inserir_base);
    if (exec->estrutura_base == NULL) return false;

    // 3. Instanciar Estrutura de Resultados (-result)
    exec->nome_result = cli_get_result(cli);
    exec->estrutura_result = instanciar_estrutura(exec->nome_result, exec->consulta, &exec->inserir_result);
    if (exec->estrutura_result == NULL) return false;

    // 4. Carregar o Dataset pelo Leitor Selecionado (-reader)
    const char *reader_nome = cli_get_reader(cli);
    const char *caminho_input = cli_get_input(cli);
    int limite = cli_get_limit(cli);

    clock_t inicio_load = clock();
    if (strcmp(reader_nome, "nasa") == 0) {
        exec->input_records = reader_nasa_carregar(caminho_input, limite, exec->estrutura_base, exec->inserir_base);
    } else {
        // Ponto de extensão para novos leitores na Parte 2 (ex: compras, contratos)
        return false;
    }
    clock_t fim_load = clock();
    exec->load_ms = ((double)(fim_load - inicio_load) / CLOCKS_PER_SEC) * 1000.0;

    // 5. Execução da Busca na Estrutura Base e Inserção na Estrutura Resultado
    clock_t inicio_search = clock();
    
    /* 
     * Exemplo de iteração sobre a estrutura_base:
     * Para cada item obtido da base:
     *   double score = 0.0;
     *   if (consulta_avaliar_item(exec->consulta, item, &score)) {
     *       exec->matched_records++;
     *       clock_t t0 = clock();
     *       exec->inserir_result(exec->estrutura_result, item);
     *       clock_t t1 = clock();
     *       exec->result_insert_ms += ((double)(t1 - t0) / CLOCKS_PER_SEC) * 1000.0;
     *   }
     */

    clock_t fim_search = clock();
    exec->search_ms = ((double)(fim_search - inicio_search) / CLOCKS_PER_SEC) * 1000.0;

    // 6. Exportação dos Primeiros top-k Registos para o Ficheiro de Saída (-output)
    clock_t inicio_write = clock();
    const char *caminho_output = cli_get_output(cli);
    int top_k = cli_get_top_k(cli);
    
    FILE *f_out = fopen(caminho_output, "w");
    if (f_out != NULL) {
        // Iterar sobre a estrutura_result e escrever até top_k registos com item_print_csv()
        fclose(f_out);
    }
    clock_t fim_write = clock();
    exec->write_ms = ((double)(fim_write - inicio_write) / CLOCKS_PER_SEC) * 1000.0;

    // 7. Apresentação do Relatório do Sistema
    imprimir_relatorio(exec);

    return true;
}

void pipeline_apagar(PIPELINE **exec) {
    if (exec == NULL || *exec == NULL) return;

    if ((*exec)->consulta != NULL) {
        consulta_apagar(&(*exec)->consulta);
    }

    if ((*exec)->estrutura_base != NULL) {
        destruir_estrutura(&(*exec)->estrutura_base, (*exec)->nome_base);
    }

    if ((*exec)->estrutura_result != NULL) {
        destruir_estrutura(&(*exec)->estrutura_result, (*exec)->nome_result);
    }

    free(*exec);
    *exec = NULL;
}