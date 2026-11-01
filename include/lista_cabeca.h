#ifndef LISTA_CABECA_H
#define LISTA_CABECA_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_cabeca_ LISTA_CABECA;

LISTA_CABECA *lista_cabeca_criar(void);
bool lista_cabeca_inserir(void *lista_ptr, ITEM *item);
void lista_cabeca_apagar(LISTA_CABECA **lista, bool apagar_itens);

int lista_cabeca_tamanho(const LISTA_CABECA *lista);
bool lista_cabeca_vazia(const LISTA_CABECA *lista);

// Percurso ignorando o nó cabeça e visitando apenas nós de dados
void lista_cabeca_percorrer(const LISTA_CABECA *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif