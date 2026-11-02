#ifndef LISTA_ORD_H
#define LISTA_ORD_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_ord_ LISTA_ORD;

LISTA_ORD *lista_ord_criar(void);

/**
 * @brief Configura o criterio de ordenação com o campo desejado e o a direção (crescente ou decrescente)
 * 
 * @param lista Ponteiro para a struct LISTA_ORD
 * @param campo String que contém o nome do campo a ser usado como base para a ordenação
 * @param decrescente Valor booleano que determina a direção da ordenação (TRUE para decrescente e FALSE para crescente)
 */
void lista_ord_configurar_ordem(LISTA_ORD *lista, const char *campo, bool decrescente);

/** 
 * @brief Insere um ITEM diretamente na sua posição correta conforme os critérios de ordenação pré-determinado
 * 
 * @param lista_ptr Ponteiro para a struct LISTA_ORD
 * @param item Ponteiro para a struct ITEM
 * 
 * @return Retorna True se a inserção foi realizada com sucesso e False se não
 */
bool lista_ord_inserir(void *lista_ptr, ITEM *item);

/**
 * @brief Apaga uma lista ordenada da memória, com o cliente escolhendo se os itens contidos nela devem ser apagados ou não
 * 
 * @param lista Ponteiro duplo que aponta para a struct LISTA_ORD
 * @param apagar_itens Valor booleano que determina se os itens contidos na struct devem ser apagados também (TRUE se sim, FALSE se não)
 */
void lista_ord_apagar(LISTA_ORD **lista, bool apagar_itens);

/**
 * @brief Função auxiliar que informa o tamanho de uma lista ordenada
 * 
 * @param lista Ponteiro para a struct LISTA_ORD
 * 
 * @return Retorna um inteiro que representa o tamanho da lista ordenada
 */
int lista_ord_tamanho(const LISTA_ORD *lista);

/**
 * @brief Função auxiliar que determina se uma lista ordenada está vazia ou não
 * 
 * @param lista Ponteiro para a struct LISTA_ORD
 * 
 * @return Retorna TRUE se a lista estiver vazia e FALSE se não
 */
bool lista_ord_vazia(const LISTA_ORD *lista);

// Percurso para busca na base e exportacao do resultado ja ordenado
void lista_ord_percorrer(const LISTA_ORD *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif