#ifndef LISTA_CABECA_H
#define LISTA_CABECA_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_cabeca_ LISTA_CABECA;

/**
 * @brief Cria uma lista encabeçada na meória
 * 
 * @return Retorna um ponteiro para a struct LISTA_CABECA
 */
LISTA_CABECA *lista_cabeca_criar(void);

/**
 * @brief Insere um elemento no final de uma lista encabeçada
 * 
 * @param lista_ptr Ponteiro que aponta para a struct LISTA_CABECA
 * @param item Ponteiro para a struct ITEM
 * 
 * @return Retorna TRUE se a inserção foi realizada com sucesso e FALSE se não
 */
bool lista_cabeca_inserir(void *lista_ptr, ITEM *item);

/**
 * @brief Apaga uma lista encabeçada da memória, com o cliente podendo escolher se apaga os itens contido nela ou não
 * 
 * @param lista Ponteiro duplo que aponta para a struct LISTA_CABECA
 * @param apagar_itens Valor que define se os itens contidos na lista encabeçada devem ser apagados da memória (TRUE para sim e FALSE para não)
 */
void lista_cabeca_apagar(LISTA_CABECA **lista, bool apagar_itens);

/**
 * @brief Função auxiliar que informa o tamanho de uma determinada lista encabeçada
 * 
 * @param lista Ponteiro para a struct LISTA_CABECA
 * 
 * @return Retorna um inteiro que representa o tamanho da lista encabeçada 
 */
int lista_cabeca_tamanho(const LISTA_CABECA *lista);

/**
 * @brief Função auxiliar que informa se uma lista encabeçada está vazia ou não
 * 
 * @param lista Ponteiro para a struct LISTA_CABECA
 * 
 * @return Retorna TRUE se a lista encabeçada estiver vazia e FALSE se não
 */
bool lista_cabeca_vazia(const LISTA_CABECA *lista);

// Percurso ignorando o nó cabeça e visitando apenas nós de dados
void lista_cabeca_percorrer(const LISTA_CABECA *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif