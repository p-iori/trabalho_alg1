#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>
#include "item.h"

typedef struct pilha_ PILHA;

/**
 * @brief Cria uma pilha na memória 
 * 
 * @return Retorna um ponteiro para uma struct PILHA
 */
PILHA *pilha_criar(void);

/**
 * @brief Insere um ITEM no topo de uma pilha
 * 
 * @param pilha_ptr Ponteiro para uma struct PILHA
 * @param item Ponteiro para uma struct ITEM
 * 
 * @return Retorna TRUE se a inserção foi realizada com sucesso e FALSE se não
 */
bool pilha_inserir(void *pilha_ptr, ITEM *item);

/**
 * @brief Remove o ITEM localizado no topo de uma pilha
 * 
 * @param pilha Ponteiro para uma struct PILHA
 * 
 * @return Retorna o ITEM que foi removido do topo da pilha
 */
ITEM *pilha_desempilhar(PILHA *pilha);

/**
 * @brief Apaga uma pilha da memória, com o cliente escolhendo se os itens contidos nela serão apagados ou não
 * 
 * @param pilha Ponteiro duplo que aponta para a struct PILHA
 * @param apagar_itens Valor booleano que determina se os itens contidos na pilha serão apagados ou não
 */
void pilha_apagar(PILHA **pilha, bool apagar_itens);

/**
 * @brief Função auxiliar que informa o tamanho de uma pilha qualquer
 * 
 * @param pilha Ponteiro para uma struct PILHA
 * 
 * @return Retorna um valor inteiro que representa o tamanho da pilha
 */
int pilha_tamanho(const PILHA *pilha);

/**
 * @brief Função auxiliar que determina se uma pilha está vazia ou não
 * 
 * @param pilha Ponteiro para uma struct PILHA
 * 
 * @return Retorna TRUE se a pilha estiver vazia e FALSE se não
 */
bool pilha_vazia(const PILHA *pilha);

// Percurso da pilha a partir do topo ate à base (usado na busca e na gravacao do CSV)
void pilha_percorrer(const PILHA *pilha, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif