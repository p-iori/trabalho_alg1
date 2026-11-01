#ifndef FILA_H
#define FILA_H

#include <stdbool.h>
#include "item.h"

typedef struct fila_ FILA;

/**
 * @brief Cria uma struct do tipo FILA na memória
 * 
 * @return Retorna um ponteiro para a struct criada
 */
FILA *fila_criar(void);

/**
 * @brief Insere um item no fim de uma fila, respeitando a estrutura circular dela
 * 
 * @param fila_ptr Ponteiro para a struct FILA 
 * @param item Ponteiro para a struct ITEM 
 * 
 * @return Retorna TRUE se a inserção ocorreu e false se não
 */
bool fila_inserir(void *fila_ptr, ITEM *item);

/**
 * @brief Remove o primeiro elemento de uma fila (Apenas o primeiro elemento pode ser removido, seguindo a definição de TAD fila)
 * 
 * @param fila Ponteiro para a struct FILA
 * 
 * @return Retorna o ITEM que acabou de ser removido do início da fila
 */
ITEM *fila_remover(FILA *fila);

/**
 * @brief Apaga uma fila da memória, com o cliente podendo escolher entre apagar os itens que ela continha, ou não
 * 
 * @param fila Ponteiro duplo que aponta para a struct FILA
 * @param apagar_itens Valor booleano que sinaliza se os itens contidos na fila devem ser apagados também (TRUE se sim e FALSE se não)
 */
void fila_apagar(FILA **fila, bool apagar_itens);

/**
 * @brief Função auxiliar que informa se uma determinada fila está vazia ou não
 * 
 * @param fila Ponteiro para a struct FILA
 * 
 * @return Retorna TRUE se a fila estiver vazia e FALSE se não
 */
bool fila_vazia(const FILA *fila);

/**
 * @brief Função auxiliar que informa o tamanho de uma determinada fila
 * 
 * @param fila Ponteiro para a struct FILA
 * 
 * @return Retorna um valor inteiro que corresponde ao tamanho da fila
 */
int fila_tamanho(const FILA *fila);

// Percurso para busca na base e exportação do resultado
void fila_percorrer(const FILA *fila, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif