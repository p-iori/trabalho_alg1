#ifndef LISTA_ENC_H
#define LISTA_ENC_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_enc_ LISTA_ENC;

LISTA_ENC *lista_enc_criar(void);
bool lista_enc_inserir(void *lista, ITEM *item);
void lista_enc_apagar(LISTA_ENC **lista, bool apagar_itens);

int lista_enc_tamanho(const LISTA_ENC *lista);

// Função para percurso dos nós (usada na busca e na gravação do CSV)
void lista_enc_percorrer(const LISTA_ENC *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif