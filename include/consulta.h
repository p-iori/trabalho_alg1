#ifndef CONSULTA_H
#define CONSULTA_H

#include <stdbool.h>
#include "item.h"

typedef struct consulta_ CONSULTA;

/**
 * @brief Cria e inicializa uma nova estrutura de CONSULTA.
 * 
 * @return Ponteiro para CONSULTA ou NULL em caso de erro.
 */
CONSULTA *consulta_criar(void);

/**
 * @brief Desaloca a memória da CONSULTA e zera o ponteiro do cliente.
 * 
 * @param consulta Ponteiro duplo para o ponteiro da CONSULTA.
 */
void consulta_apagar(CONSULTA **consulta);

/**
 * @brief Carrega as regras da consulta contidas no arquivo fornecido.
 * 
 * @param consulta Ponteiro para a struct CONSULTA.
 * @param caminho_arquivo Caminho do arquivo de texto contendo os comandos.
 * @return true em caso de sucesso na leitura, false caso contrário.
 */
bool consulta_carregar_arquivo(CONSULTA *consulta, const char *caminho_arquivo);

/**
 * @brief Avalia um ITEM em relação aos comandos FILTER e SIM da consulta.
 * 
 * @param consulta Ponteiro para a consulta carregada.
 * @param item Ponteiro para o ITEM a ser testado.
 * @param score_out Ponteiro de saída onde será armazenado a pontuação de similaridade calculada.
 * @return true se o item passou em todos os FILTERs e possui similaridade >= ACCEPT; false caso contrário.
 */
bool consulta_avaliar_item(const CONSULTA *consulta, const ITEM *item, double *score_out);

const char* consulta_get_ordem_campo(const CONSULTA *consulta);
bool consulta_get_ordem_decrescente(const CONSULTA *consulta);
const char* consulta_get_grupo_campo1(const CONSULTA *consulta);
const char* consulta_get_grupo_campo2(const CONSULTA *consulta);
const char* consulta_get_cross_campo1(const CONSULTA *consulta);
const char* consulta_get_cross_campo2(const CONSULTA *consulta);

#endif