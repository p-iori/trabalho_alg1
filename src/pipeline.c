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
#include "reader_detran.h"

// Inclusão dos TADs de estruturas de dados
#include "pilha.h"
#include "fila.h"
#include "lista_seq.h"
#include "lista_enc.h"
#include "lista_cabeca.h"
#include "lista_ord.h"
#include "lista_gen.h"
#include "lista_cruz.h"

// Tipos de ponteiros de função para operações genéricas
typedef void (*funcao_percorrer_fn)(const void *estrutura, void (*visitar)(ITEM *item, void *contexto), void *contexto);
typedef void (*funcao_apagar_fn)(void **estrutura, bool apagar_itens);

struct pipeline_ {
    CONSULTA *consulta;
    
    void *estrutura_base;
    funcao_inserir_fn inserir_base;
    funcao_percorrer_fn percorrer_base;
    funcao_apagar_fn apagar_base;
    
    void *estrutura_result;
    funcao_inserir_fn inserir_result;
    funcao_percorrer_fn percorrer_result;
    funcao_apagar_fn apagar_result;

    // Métricas de execução
    int input_records;
    int matched_records;
    int output_records;

    double load_ms;
    double search_ms;
    double result_insert_ms;
    double write_ms;
};

// Estruturas de contexto para os callbacks das funções de percurso
typedef struct {
    PIPELINE *exec;
} CONTEXTO_BUSCA;

typedef struct {
    PIPELINE *exec;
    FILE *f_out;
    int top_k;
} CONTEXTO_ESCRITA;

PIPELINE *pipeline_criar(void) {
    return (PIPELINE*) calloc(1, sizeof(PIPELINE));
}

// Instancia qualquer TAD e mapeia os ponteiros de função de Inserção, Percurso e Liberação
static void *instanciar_estrutura(
    const char *nome, 
    CONSULTA *consulta, 
    funcao_inserir_fn *fn_ins, 
    funcao_percorrer_fn *fn_perc,
    funcao_apagar_fn *fn_apag
) {
    if (nome == NULL || fn_ins == NULL || fn_perc == NULL || fn_apag == NULL) return NULL;

    void *est = NULL;

    if (strcmp(nome, "pilha") == 0) {
        est = pilha_criar();
        *fn_ins  = (funcao_inserir_fn) pilha_inserir;
        *fn_perc = (funcao_percorrer_fn) pilha_percorrer;
        *fn_apag = (funcao_apagar_fn) pilha_apagar;
    } else if (strcmp(nome, "fila") == 0) {
        est = fila_criar();
        *fn_ins  = (funcao_inserir_fn) fila_inserir;
        *fn_perc = (funcao_percorrer_fn) fila_percorrer;
        *fn_apag = (funcao_apagar_fn) fila_apagar;
    } else if (strcmp(nome, "lista_seq") == 0) {
        est = lista_seq_criar();
        *fn_ins  = (funcao_inserir_fn) lista_seq_inserir;
        *fn_perc = (funcao_percorrer_fn) lista_seq_percorrer;
        *fn_apag = (funcao_apagar_fn) lista_seq_apagar;
    } else if (strcmp(nome, "lista_enc") == 0) {
        est = lista_enc_criar();
        *fn_ins  = (funcao_inserir_fn) lista_enc_inserir;
        *fn_perc = (funcao_percorrer_fn) lista_enc_percorrer;
        *fn_apag = (funcao_apagar_fn) lista_enc_apagar;
    } else if (strcmp(nome, "lista_cabeca") == 0) {
        est = lista_cabeca_criar();
        *fn_ins  = (funcao_inserir_fn) lista_cabeca_inserir;
        *fn_perc = (funcao_percorrer_fn) lista_cabeca_percorrer;
        *fn_apag = (funcao_apagar_fn) lista_cabeca_apagar;
    } else if (strcmp(nome, "lista_ord") == 0) {
        est = lista_ord_criar();
        if (consulta != NULL) {
            lista_ord_configurar_ordem(
                est, 
                consulta_get_ordem_campo(consulta), 
                consulta_get_ordem_decrescente(consulta)
            );
        }
        *fn_ins  = (funcao_inserir_fn) lista_ord_inserir;
        *fn_perc = (funcao_percorrer_fn) lista_ord_percorrer;
        *fn_apag = (funcao_apagar_fn) lista_ord_apagar;
    } else if (strcmp(nome, "lista_gen") == 0) {
        est = lista_gen_criar();
        if (consulta != NULL) {
            lista_gen_configurar(
                est, 
                consulta_get_grupo_campo1(consulta), 
                consulta_get_grupo_campo2(consulta)
            );
        }
        *fn_ins  = (funcao_inserir_fn) lista_gen_inserir;
        *fn_perc = (funcao_percorrer_fn) lista_gen_percorrer;
        *fn_apag = (funcao_apagar_fn) lista_gen_apagar;
    } else if (strcmp(nome, "lista_cruz") == 0) {
        est = lista_cruz_criar();
        if (consulta != NULL) {
            lista_cruz_configurar(
                est, 
                consulta_get_cross_campo1(consulta), 
                consulta_get_cross_campo2(consulta)
            );
        }
        *fn_ins  = (funcao_inserir_fn) lista_cruz_inserir;
        *fn_perc = (funcao_percorrer_fn) lista_cruz_percorrer;
        *fn_apag = (funcao_apagar_fn) lista_cruz_apagar;
    }

    return est;
}

// Callback executado para cada item durante o percurso da estrutura_base
static void callback_avaliar_e_inserir(ITEM *item, void *contexto) {
    CONTEXTO_BUSCA *ctx = (CONTEXTO_BUSCA *) contexto;
    PIPELINE *exec = ctx->exec;

    double score = 0.0;
    if (consulta_avaliar_item(exec->consulta, item, &score)) {
        exec->matched_records++;

        clock_t t0 = clock();
        exec->inserir_result(exec->estrutura_result, item);
        clock_t t1 = clock();

        exec->result_insert_ms += ((double)(t1 - t0) / CLOCKS_PER_SEC) * 1000.0;
    }
}

// Callback executado para cada item durante o percurso da estrutura_result
static void callback_escrever_item(ITEM *item, void *contexto) {
    CONTEXTO_ESCRITA *ctx = (CONTEXTO_ESCRITA *) contexto;

    if (ctx->top_k == 0 || ctx->exec->output_records < ctx->top_k) {
        item_print_csv(item, ctx->f_out);
        ctx->exec->output_records++;
    }
}

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

    // 1. Carregar Consulta
    const char *caminho_query = cli_get_query(cli);
    exec->consulta = consulta_criar();
    if (exec->consulta == NULL) return false;

    if (!consulta_carregar_arquivo(exec->consulta, caminho_query)) {
        return false;
    }

    // 2. Instanciar Estrutura Base
    const char *nome_base = cli_get_base(cli);
    exec->estrutura_base = instanciar_estrutura(
        nome_base, 
        exec->consulta, 
        &exec->inserir_base, 
        &exec->percorrer_base, 
        &exec->apagar_base
    );
    if (exec->estrutura_base == NULL) return false;

    // 3. Instanciar Estrutura Resultado
    const char *nome_result = cli_get_result(cli);
    exec->estrutura_result = instanciar_estrutura(
        nome_result, 
        exec->consulta, 
        &exec->inserir_result, 
        &exec->percorrer_result, 
        &exec->apagar_result
    );
    if (exec->estrutura_result == NULL) return false;

    // 4. Carregar Dataset via Leitor Apropriado
    const char *reader_nome = cli_get_reader(cli);
    const char *caminho_input = cli_get_input(cli);
    int limite = cli_get_limit(cli);

    clock_t inicio_load = clock();
    if (strcmp(reader_nome, "nasa") == 0) {
        exec->input_records = reader_nasa_carregar(caminho_input, limite, exec->estrutura_base, exec->inserir_base);
    } else if (strcmp(reader_nome, "detran") == 0) {
        exec->input_records = reader_detran_carregar(caminho_input, limite, exec->estrutura_base, exec->inserir_base);
    } else {
        return false; // Leitor não reconhecido
    }
    clock_t fim_load = clock();
    exec->load_ms = ((double)(fim_load - inicio_load) / CLOCKS_PER_SEC) * 1000.0;

    // 5. Executar Busca na Estrutura Base e Inserção nos Resultados
    clock_t inicio_search = clock();
    
    CONTEXTO_BUSCA ctx_busca = { .exec = exec };
    exec->percorrer_base(exec->estrutura_base, callback_avaliar_e_inserir, &ctx_busca);

    clock_t fim_search = clock();
    exec->search_ms = ((double)(fim_search - inicio_search) / CLOCKS_PER_SEC) * 1000.0;

    // 6. Exportar Resultados para Ficheiro CSV
    clock_t inicio_write = clock();
    const char *caminho_output = cli_get_output(cli);
    int top_k = cli_get_top_k(cli);
    
    FILE *f_out = fopen(caminho_output, "w");
    if (f_out != NULL) {
        // Escreve o cabeçalho exato do dataset correspondente
        if (strcmp(reader_nome, "nasa") == 0) {
            fprintf(f_out, "asteroid_designation,asteroid_fullname,close_approach_date,year,month,distance_au,velocity_km_s,absolute_magnitude,days_until_approach,is_past_event,is_future_event,risk_score,panic_level,threat_category,panic_verdict,on_sentry_list,sentry_impact_prob,sentry_torino_scale,sentry_palermo_scale,sentry_diameter_km\n");
        } else if (strcmp(reader_nome, "detran") == 0) {
            fprintf(f_out, "\"id\";\"data_inversa\";\"dia_semana\";\"horario\";\"uf\";\"br\";\"km\";\"municipio\";\"causa_acidente\";\"tipo_acidente\";\"classificacao_acidente\";\"fase_dia\";\"sentido_via\";\"condicao_metereologica\";\"tipo_pista\";\"tracado_via\";\"uso_solo\";\"pessoas\";\"mortos\";\"feridos_leves\";\"feridos_graves\";\"ilesos\";\"ignorados\";\"feridos\";\"veiculos\";\"latitude\";\"longitude\";\"regional\";\"delegacia\";\"uop\"\n");
        }

        CONTEXTO_ESCRITA ctx_escrita = {
            .exec = exec,
            .f_out = f_out,
            .top_k = top_k
        };

        exec->percorrer_result(exec->estrutura_result, callback_escrever_item, &ctx_escrita);

        fclose(f_out);
    }
    clock_t fim_write = clock();
    exec->write_ms = ((double)(fim_write - inicio_write) / CLOCKS_PER_SEC) * 1000.0;

    // 7. Relatório Final de Métricas
    imprimir_relatorio(exec);

    return true;
}

void pipeline_apagar(PIPELINE **exec) {
    if (exec == NULL || *exec == NULL) return;

    if ((*exec)->consulta != NULL) {
        consulta_apagar(&(*exec)->consulta);
    }

    // A estrutura_result aponta para os mesmos ITENS da base,
    // logo apaga-se a estrutura_result com apagar_itens = false para evitar double free
    if ((*exec)->estrutura_result != NULL && (*exec)->apagar_result != NULL) {
        (*exec)->apagar_result(&(*exec)->estrutura_result, false);
    }

    // A estrutura_base é a dona original da memória dos ITENS alocados no reader
    if ((*exec)->estrutura_base != NULL && (*exec)->apagar_base != NULL) {
        (*exec)->apagar_base(&(*exec)->estrutura_base, true);
    }

    free(*exec);
    *exec = NULL;
}