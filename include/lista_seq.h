#ifndef LISTA_SEQ_H
#define LISTA_SEQ_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_seq_ LISTA_SEQ;
/**
 * @brief Cria uma lista sequencial na memória com capacidade inicial de 1000 itens
 * 
 * @return Retorna um ponteiro para uma struct do tipo LISTA_SEQ
 */
LISTA_SEQ *lista_seq_criar(void);

/**
 * @brief Insere um elemento na primeira posição livre do vetor do tipo ITEM que representa a lista sequencial
 * 
 * @param lista_ptr Ponteiro para a struct LISTA_SEQ
 * @param item Ponteiro para a struct ITEM
 * 
 * @return Retorna TRUE se a inserção foi realizada com sucesso e FALSE se não
 */
bool lista_seq_inserir(void *lista_ptr, ITEM *item);

/**
 * @brief Apaga uma lista sequencial da memória, com o cliente escolhendo se deseja apagar os itens contidos nela
 * 
 * @param lista Ponteiro duplo apontando para a struct LISTA_SEQ
 * @param apagar_itens Valor booleano que determina se os itens contidos na struct devem ser apagados também 
 */
void lista_seq_apagar(LISTA_SEQ **lista, bool apagar_itens);

/**
 * @brief Função auxiliar que informa o tamanho de uma lista sequencial
 *
 * @param lista Ponteiro para a struct LISTA_SEQ
 * 
 * @return Retorna um valor inteiro que representa o tamanho da lista sequencial
 */
int lista_seq_tamanho(const LISTA_SEQ *lista);

/**
 * @brief Função auxiliar que determina se uma lista sequencial é vazia ou não 
 * 
 * @param lista Ponteiro para a struct LISTA_SEQ
 * 
 * @return Retorna TRUE se a lista for vazia e FALSE se não
 */
bool lista_seq_vazia(const LISTA_SEQ *lista);

/**
 * @brief Função auxiliar que retorna o ITEM contido em determinada posição do array que representa a lista sequencial
 * 
 * @param lista Ponteiro para a struct LISTA_SEQ
 * @param posicao Inteiro que representa a posição do ITEM a ser obtido
 * 
 * @return Retorna um ponteiro para a struct ITEM encontrada na posição indicada 
 */
ITEM *lista_seq_obter(const LISTA_SEQ *lista, int posicao);

// Percurso para busca na base e exportação dos resultados
void lista_seq_percorrer(const LISTA_SEQ *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif