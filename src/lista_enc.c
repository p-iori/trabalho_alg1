#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lista_enc.h"

typedef struct no_ NO;

struct no_ {
    ITEM *item;
    NO *proximo;
};

struct lista_enc_ {
    NO *inicio;
    NO *fim;
    int tamanho;
};

LISTA_ENC *lista_enc_criar(void) {
    LISTA_ENC *lista = (LISTA_ENC *) malloc(sizeof(LISTA_ENC));
    if (lista == NULL) return NULL;

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;

    return lista;
}

bool lista_enc_inserir(void *lista_ptr, ITEM *item) {
    LISTA_ENC *lista = (LISTA_ENC *) lista_ptr;
    if (lista == NULL || item == NULL) return false;

    NO *novo_no = (NO *) malloc(sizeof(NO));
    if (novo_no == NULL) return false;

    novo_no->item = item;
    novo_no->proximo = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novo_no;
    } else {
        lista->fim->proximo = novo_no;
    }

    lista->fim = novo_no;
    lista->tamanho++;

    return true;
}

void lista_enc_apagar(LISTA_ENC **lista, bool apagar_itens) {
    if (lista == NULL || *lista == NULL) return;

    NO *atual = (*lista)->inicio;
    while (atual != NULL) {
        NO *proximo = atual->proximo;

        if (apagar_itens && atual->item != NULL) {
            item_apagar(&(atual->item));
        }

        free(atual);
        atual = proximo;
    }

    free(*lista);
    *lista = NULL;
}

int lista_enc_tamanho(const LISTA_ENC *lista) {
    return (lista != NULL) ? lista->tamanho : 0;
}

void lista_enc_percorrer(const LISTA_ENC *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto) {
    if (lista == NULL || visitar == NULL) return;

    NO *atual = lista->inicio;
    while (atual != NULL) {
        visitar(atual->item, contexto);
        atual = atual->proximo;
    }
}