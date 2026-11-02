#ifndef PIPELINE_H
#define PIPELINE_H

#include <stdbool.h>
#include "cli.h"

/* 
 * TAD pipeline: Atua como controlador/fachada da aplicação.
 * Responsável por orquestrar a inicialização das estruturas de dados (base e resultado),
 * seleção do leitor apropriado, processamento da consulta, medição de tempo,
 * escrita do resultado e exibição das métricas de desempenho na saída padrão.
 */
typedef struct pipeline_ PIPELINE;

/**
 * @brief Aloca e inicializa a estrutura do pipeline.
 * @return Ponteiro para pipeline ou NULL em caso de falha.
 */
PIPELINE *pipeline_criar(void);

/**
 * @brief Executa o fluxo completo do programa a partir das configurações CLI fornecidas.
 * 
 * Etapas orquestradas:
 * 1. Leitura e parsing do arquivo de consulta (-query).
 * 2. Instanciação da estrutura de dados principal (-base).
 * 3. Seleção do leitor de dataset adequado (-reader) e carga dos dados.
 * 4. Avaliação dos itens e inserção dos aceitos na estrutura de resultados (-result).
 * 5. Exportação dos primeiros top-k registros para o arquivo de saída (-output).
 * 6. Impressão do relatório de status e tempos na saída padrão.
 * 
 * @param exec Ponteiro para o pipeline.
 * @param cli Configurações capturadas da linha de comando.
 * @return true se o fluxo foi concluído com sucesso, false caso contrário.
 */
bool pipeline_run(PIPELINE *exec, const CONFIG *cli);

/**
 * @brief Libera a memória alocada pelo pipeline e por todas as estruturas atreladas a ele.
 * @param exec Endereço do ponteiro do pipeline.
 */
void pipeline_apagar(PIPELINE **exec);

#endif