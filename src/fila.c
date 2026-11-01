#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fila.h"

#define CAPACIDADE_INICIAL 100

struct fila_ {
    ITEM **itens;
    int inicio;
    int fim;
    int tamanho;
    int capacidade;
};

FILA *fila_criar(void) {
    FILA *nova_fila = (FILA *) malloc(sizeof(FILA));
    if (nova_fila == NULL) return NULL;

    nova_fila->capacidade = CAPACIDADE_INICIAL;
    nova_fila->itens = (ITEM **) malloc(sizeof(ITEM *) * nova_fila->capacidade);
    if (nova_fila->itens == NULL) {
        free(nova_fila);
        return NULL;
    }

    nova_fila->inicio = 0;
    nova_fila->fim = 0;
    nova_fila->tamanho = 0;

    return nova_fila;
}

// Redimensiona o vetor circular dobrando a capacidade
static bool fila_redimensionar(FILA *fila) {
    int nova_capacidade = fila->capacidade * 2;
    ITEM **novos_itens = (ITEM **) malloc(sizeof(ITEM *) * nova_capacidade);
    if (novos_itens == NULL) return false;

    // Copia os elementos reordenando a partir do índice 0
    for (int i = 0; i < fila->tamanho; i++) {
        int pos_antiga = (fila->inicio + i) % fila->capacidade;
        novos_itens[i] = fila->itens[pos_antiga];
    }

    free(fila->itens);
    fila->itens = novos_itens;
    fila->inicio = 0;
    fila->fim = fila->tamanho;
    fila->capacidade = nova_capacidade;

    return true;
}

bool fila_inserir(void *fila_ptr, ITEM *item) {
    FILA *fila = (FILA *) fila_ptr;
    if (fila == NULL || item == NULL) return false;

    // Se atingiu a capacidade atual, expande o vetor dinamicamente
    if (fila->tamanho == fila->capacidade) {
        if (!fila_redimensionar(fila)) return false;
    }

    fila->itens[fila->fim] = item;
    fila->fim = (fila->fim + 1) % fila->capacidade;
    fila->tamanho++;

    return true;
}

ITEM *fila_remover(FILA *fila) {
    if (fila == NULL || fila->tamanho == 0) return NULL;

    ITEM *item = fila->itens[fila->inicio];
    fila->itens[fila->inicio] = NULL;
    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    fila->tamanho--;

    return item;
}

void fila_apagar(FILA **fila, bool apagar_itens) {
    if (fila == NULL || *fila == NULL) return;

    if (apagar_itens) {
        for (int i = 0; i < (*fila)->tamanho; i++) {
            int pos = ((*fila)->inicio + i) % (*fila)->capacidade;
            if ((*fila)->itens[pos] != NULL) {
                item_apagar(&((*fila)->itens[pos]));
            }
        }
    }

    free((*fila)->itens);
    free(*fila);
    *fila = NULL;
}

bool fila_vazia(const FILA *fila) {
    return (fila == NULL || fila->tamanho == 0);
}

int fila_tamanho(const FILA *fila) {
    return (fila != NULL) ? fila->tamanho : 0;
}

void fila_percorrer(const FILA *fila, void (*visitar)(ITEM *item, void *contexto), void *contexto) {
    if (fila == NULL || visitar == NULL) return;

    for (int i = 0; i < fila->tamanho; i++) {
        int pos = (fila->inicio + i) % fila->capacidade;
        visitar(fila->itens[pos], contexto);
    }
}