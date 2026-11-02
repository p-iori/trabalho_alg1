#ifndef LISTA_CRUZ_H
#define LISTA_CRUZ_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_cruz_ LISTA_CRUZ;

LISTA_CRUZ *lista_cruz_criar(void);
void lista_cruz_configurar(LISTA_CRUZ *lista, const char *campo1, const char *campo2);

bool lista_cruz_inserir(void *lista_ptr, ITEM *item);
void lista_cruz_apagar(LISTA_CRUZ **lista, bool apagar_itens);

int lista_cruz_tamanho(const LISTA_CRUZ *lista);
bool lista_cruz_vazia(const LISTA_CRUZ *lista);

// Percurso navegando pelo Eixo 1 (Campo 1) ou Eixo 2 (Campo 2)
void lista_cruz_percorrer(const LISTA_CRUZ *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif