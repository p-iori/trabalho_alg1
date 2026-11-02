#ifndef LISTA_GEN_H
#define LISTA_GEN_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_gen_ LISTA_GEN;

LISTA_GEN *lista_gen_criar(void);
void lista_gen_configurar(LISTA_GEN *lista, const char *campo_grupo, const char *campo_subgrupo);

bool lista_gen_inserir(void *lista_ptr, ITEM *item);
void lista_gen_apagar(LISTA_GEN **lista, bool apagar_itens);

int lista_gen_tamanho(const LISTA_GEN *lista);
bool lista_gen_vazia(const LISTA_GEN *lista);

// Percurso hierárquico (Grupo -> Subgrupo -> Itens)
void lista_gen_percorrer(const LISTA_GEN *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto);

#endif