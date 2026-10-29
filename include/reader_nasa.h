#ifndef READER_NASA_H
#define READER_NASA_H

#include <stdbool.h>
#include "item.h"

// Define um tipo de ponteiro de função para a operação de inserção:
// Recebe o ponteiro da estrutura e o ponteiro do ITEM a ser inserido.
typedef bool (*funcao_inserir_fn)(void *estrutura, ITEM *item);

/**
 * @brief Lê o CSV inteiro (respeitando o limite) e insere os registros na estrutura de base.
 * 
 * @param caminho_csv Caminho para o arquivo CSV de entrada.
 * @param limite Máximo de registros a carregar (0 para todos).
 * @param estrutura_base Ponteiro para a estrutura de dados (ex: LISTA_ENC*).
 * @param funcao_inserir Função de inserção correspondente à estrutura escolhida.
 * @return int Número total de registros carregados com sucesso.
 */
int reader_nasa_carregar(const char *caminho_csv, 
                        int limite, 
                        void *estrutura_base, 
                        funcao_inserir_fn funcao_inserir);

#endif