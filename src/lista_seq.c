#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lista_seq.h"

#define CAPACIDADE_INICIAL 1000

struct lista_seq_ {
    ITEM **itens;
    int tamanho;
    int capacidade;
};

LISTA_SEQ *lista_seq_criar(void) {
    LISTA_SEQ *lista = (LISTA_SEQ *) malloc(sizeof(LISTA_SEQ));
    if (lista == NULL) return NULL;

    lista->capacidade = CAPACIDADE_INICIAL;
    lista->tamanho = 0;
    lista->itens = (ITEM **) malloc(sizeof(ITEM *) * lista->capacidade);

    if (lista->itens == NULL) {
        free(lista);
        return NULL;
    }

    return lista;
}

// Redimensiona o vetor dinamicamente dobrando sua capacidade
static bool lista_seq_redimensionar(LISTA_SEQ *lista) {
    int nova_capacidade = lista->capacidade * 2;
    ITEM **novos_itens = (ITEM **) realloc(lista->itens, sizeof(ITEM *) * nova_capacidade);
    if (novos_itens == NULL) return false;

    lista->itens = novos_itens;
    lista->capacidade = nova_capacidade;
    return true;
}

bool lista_seq_inserir(void *lista_ptr, ITEM *item) {
    LISTA_SEQ *lista = (LISTA_SEQ *) lista_ptr;
    if (lista == NULL || item == NULL) return false;

    // Se o vetor estiver cheio, realiza o realloc
    if (lista->tamanho == lista->capacidade) {
        if (!lista_seq_redimensionar(lista)) return false;
    }

    lista->itens[lista->tamanho] = item;
    lista->tamanho++;

    return true;
}

void lista_seq_apagar(LISTA_SEQ **lista, bool apagar_itens) {
    if (lista == NULL || *lista == NULL) return;

    if (apagar_itens) {
        for (int i = 0; i < (*lista)->tamanho; i++) {
            if ((*lista)->itens[i] != NULL) {
                item_apagar(&((*lista)->itens[i]));
            }
        }
    }

    free((*lista)->itens);
    free(*lista);
    *lista = NULL;
}

int lista_seq_tamanho(const LISTA_SEQ *lista) {
    return (lista != NULL) ? lista->tamanho : 0;
}

bool lista_seq_vazia(const LISTA_SEQ *lista) {
    return (lista == NULL || lista->tamanho == 0);
}

ITEM *lista_seq_obter(const LISTA_SEQ *lista, int posicao) {
    if (lista == NULL || posicao < 0 || posicao >= lista->tamanho) return NULL;
    return lista->itens[posicao];
}

void lista_seq_percorrer(const LISTA_SEQ *lista, void (*visitar)(ITEM *item, void *contexto), void *contexto) {
    if (lista == NULL || visitar == NULL) return;

    for (int i = 0; i < lista->tamanho; i++) {
        visitar(lista->itens[i], contexto);
    }
}