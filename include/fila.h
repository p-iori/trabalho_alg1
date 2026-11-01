#ifndef FILA_H
#define FILA_H

#include <stdbool.h>
#include "item.h"

typedef struct fila_ FILA;

FILA *fila_criar(void);
bool fila_inserir(void *fila_ptr, ITEM *item);
ITEM *fila_remover(FILA *fila);
void fila_apagar(FILA **fila, bool apagar_itens);

bool fila_vazia(const FILA *fila);
int fila_tamanho(const FILA *fila);

// Percurso para busca na base e exportação do resultado
void fila_percorrer(const FILA *fila, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif