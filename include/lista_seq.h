#ifndef LISTA_SEQ_H
#define LISTA_SEQ_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_seq_ LISTA_SEQ;

LISTA_SEQ *lista_seq_criar(void);
bool lista_seq_inserir(void *lista_ptr, ITEM *item);
void lista_seq_apagar(LISTA_SEQ **lista, bool apagar_itens);

int lista_seq_tamanho(const LISTA_SEQ *lista);
bool lista_seq_vazia(const LISTA_SEQ *lista);
ITEM *lista_seq_obter(const LISTA_SEQ *lista, int posicao);

// Percurso para busca na base e exportação dos resultados
void lista_seq_percorrer(const LISTA_SEQ *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif