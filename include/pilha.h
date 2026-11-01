#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>
#include "item.h"

typedef struct pilha_ PILHA;

PILHA *pilha_criar(void);
bool pilha_inserir(void *pilha_ptr, ITEM *item);
ITEM *pilha_desempilhar(PILHA *pilha);
void pilha_apagar(PILHA **pilha, bool apagar_itens);

int pilha_tamanho(const PILHA *pilha);
bool pilha_vazia(const PILHA *pilha);

// Percurso da pilha a partir do topo ate à base (usado na busca e na gravacao do CSV)
void pilha_percorrer(const PILHA *pilha, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif