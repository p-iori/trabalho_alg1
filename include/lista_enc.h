#ifndef LISTA_ENC_H
#define LISTA_ENC_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_enc_ LISTA_ENC;

/**
 * @brief Cria uma lista encadeada na memória
 * 
 * @return Retorna um ponteiro para a struct LISTA_ENC criada
 */
LISTA_ENC *lista_enc_criar(void);

/**
 * @brief Insere um item no fim de uma lista encadeada
 * 
 * @param lista Ponteiro para a struct LISTA_ENC
 * @param item Ponteiro para a struct ITEM a ser adicionada
 * 
 * @return Retorna TRUE se foi possível fazer a inserção e FALSE se não
 */
bool lista_enc_inserir(void *lista, ITEM *item);

/**
 * @brief Apaga uma lista encadeada inteira da memória, o cliente pode escolher se apaga os itens contidos nela ou não
 * 
 * @param lista Ponteiro duplo apontando para a struct LISTA_ENC
 * @param apagar_itens Valor booleano que indica se os itens contidos na struct LISTA_ENC devem ser apagados ou não
 */
void lista_enc_apagar(LISTA_ENC **lista, bool apagar_itens);

/**
 * @brief Função auxiliar que informa o tamanho de uma determinada lista encadeada
 * 
 * @param lista Ponteiro para a struct LISTA_ENC
 * 
 * @return Retorna um valor inteiro que representa o tamanho da lista encadeada
 */
int lista_enc_tamanho(const LISTA_ENC *lista);

// Função para percurso dos nós (usada na busca e na gravação do CSV)
void lista_enc_percorrer(const LISTA_ENC *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif