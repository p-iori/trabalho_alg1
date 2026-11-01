#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lista_cabeca.h"

typedef struct no_ NO;

struct no_ {
    ITEM *item;
    NO *proximo;
};

struct lista_cabeca_ {
    NO *cabeca; // Nó sentinela/cabeça real (não armazena registro)
    NO *fim;
    int tamanho;
};

LISTA_CABECA *lista_cabeca_criar(void) {
    LISTA_CABECA *lista = (LISTA_CABECA *) malloc(sizeof(LISTA_CABECA));
    if (lista == NULL) return NULL;

    // Aloca o nó cabeça (sentinela)
    lista->cabeca = (NO *) malloc(sizeof(NO));
    if (lista->cabeca == NULL) {
        free(lista);
        return NULL;
    }

    lista->cabeca->item = NULL;
    lista->cabeca->proximo = NULL;
    lista->fim = lista->cabeca;
    lista->tamanho = 0;

    return lista;
}

bool lista_cabeca_inserir(void *lista_ptr, ITEM *item) {
    LISTA_CABECA *lista = (LISTA_CABECA *) lista_ptr;
    if (lista == NULL || item == NULL) return false;

    NO *novo_no = (NO *) malloc(sizeof(NO));
    if (novo_no == NULL) return false;

    novo_no->item = item;
    novo_no->proximo = NULL;

    // A inserção não precisa verificar se inicio == NULL, pois lista->fim aponta para cabeca inicialmente
    lista->fim->proximo = novo_no;
    lista->fim = novo_no;
    lista->tamanho++;

    return true;
}

void lista_cabeca_apagar(LISTA_CABECA **lista, bool apagar_itens) {
    if (lista == NULL || *lista == NULL) return;

    // Começa a desalocação no próprio nó cabeça
    NO *atual = (*lista)->cabeca;
    while (atual != NULL) {
        NO *proximo = atual->proximo;

        // Desaloca o item apenas se apagar_itens for true e o item existir (pula o nó cabeça)
        if (apagar_itens && atual->item != NULL) {
            item_apagar(&(atual->item));
        }

        free(atual);
        atual = proximo;
    }

    free(*lista);
    *lista = NULL;
}

int lista_cabeca_tamanho(const LISTA_CABECA *lista) {
    return (lista != NULL) ? lista->tamanho : 0;
}

bool lista_cabeca_vazia(const LISTA_CABECA *lista) {
    return (lista == NULL || lista->tamanho == 0);
}

void lista_cabeca_percorrer(const LISTA_CABECA *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto) {
    if (lista == NULL || visitar == NULL) return;

    // O percurso inicia explicitamente no primeiro nó de dados real (cabeca->proximo)
    NO *atual = lista->cabeca->proximo;
    while (atual != NULL) {
        visitar(atual->item, contexto);
        atual = atual->proximo;
    }
}